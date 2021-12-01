#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "bees_nest_v02.h"



#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
//  #define DEBUG     // for printf to serial port
//  #include "debug.h"
#endif


REGISTER_USERDATA(USERDATA);


void smooth_set_motors(uint8_t ccw, uint8_t cw)
{
  // OCR2A = ccw;  OCR2B = cw;
  #ifdef KILOBOT
  uint8_t l = 0, r = 0;
  if (ccw && !OCR2A) // we want left motor on, and it's off
  l = 0xff;
  if (cw && !OCR2B)  // we want right motor on, and it's off
  r = 0xff;
  if (l || r)        // at least one motor needs spin-up
  {
    set_motors(l, r);
    delay(15);
  }
  #endif
  // spin-up is done, now we set the real value
  set_motors(ccw, cw);
}

void set_motion(motion_t new_motion)
{
  if(mydata->current_motion_type != new_motion) {
    switch(new_motion) {
      case STOP:
      default:
      smooth_set_motors(0,0);
      break;
      case FORWARD:
      smooth_set_motors(kilo_straight_left, kilo_straight_right);
      break;
      case LEFT:
      smooth_set_motors(kilo_turn_left, 0);
      break;
      case RIGHT:
      smooth_set_motors(0, kilo_turn_right);
      break;
    }
    mydata->current_motion_type = new_motion;
  }
}




void random_walk(){
  switch(mydata->current_motion_type) {
    case LEFT:
    case RIGHT:
    if(kilo_ticks > mydata->last_motion_ticks + mydata->turning_ticks) {
      /* start moving forward */
      mydata->last_motion_ticks = kilo_ticks;
      set_motion(FORWARD);
      //set_color(RGB(0,1,0));
    }
    break;
    case FORWARD:
    if( kilo_ticks > mydata->last_motion_ticks + mydata->max_straight_ticks) {
      /* perform a radnom turn */
      mydata->last_motion_ticks = kilo_ticks;
      if( rand_soft()%2) {
        set_motion(LEFT);
        //set_color(RGB(0,0,1));
      }
      else {
        set_motion(RIGHT);
        //set_color(RGB(1,0,0));
      }
      mydata->turning_ticks = rand_soft()%mydata->max_turning_ticks + 1;
    }
    break;
    case STOP:
    default:
    set_motion(STOP);
  }
}

message_t *message_tx()
{
  return &mydata->transmit_msg;
}

void update_message() {
  mydata->transmit_msg.data[1] = mydata->my_id;
  mydata->transmit_msg.data[2] = mydata->dancing;
  mydata->transmit_msg.data[3] = mydata->quality_time;
  mydata->transmit_msg.data[4] = mydata->t_mod;
  mydata->transmit_msg.data[5] = mydata->my_cycle;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}

/*        MESSAGE RECEPTION AND TRANSMITTION FUNCTIONS        */

void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->dist = *d;
  mydata->new_message = 1;
  mydata->bot_id = message->data[1];
  mydata->bot_dsite = message->data[2];
  mydata->bot_qtime = message->data[3];
  mydata->bot_t = message->data[4];
  mydata->bot_cycle = message->data[5];



}









void setup()
{
  // Random different for each bot
  rand_seed(kilo_uid + IND_SEED);
  // I start not dancing */
  mydata->my_id = kilo_uid;
  // Random motion variables
  mydata->t = kilo_ticks;
  mydata->t_mod = 0;
  mydata->cycle = 0;
  mydata->my_cycle = 0;
  mydata->bot_cycle = 0;
  mydata->t_mod = 0;
  mydata->bot_t_mod = 0;
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 180;
  mydata->max_straight_ticks = 60;
  mydata->last_motion_ticks = rand_soft() % mydata->max_straight_ticks + 1;

  //set_motion(FORWARD);

}

void loop()
{

  if(kilo_ticks > mydata->t + (DELTA_T - 1)){
    set_color(RGB(0,0,1));
    if(kilo_ticks > )
    mydata->t = kilo_ticks;

  }

}




int16_t circle_barrier(double x, double y, double * dx, double * dy)
{
  double d = sqrt(x*x + y*y);

  if (d < 400.0)
  return 0;

  *dx = -x/d;
  *dy = -y/d;

  return 1;
}

//*************************************** (^_^) ******************************/
#ifdef SIMULATOR
/* provide a text string for the simulator status bar about this bot */
static char botinfo_buffer[10000];
char *cb_botinfo(void)
{
  char *p = botinfo_buffer;
  p += sprintf (p, "my_cycle: %d\n", mydata->my_cycle);
  p += sprintf (p, "bot cycle: %d\n",  mydata->bot_cycle);
  p += sprintf (p, "t: %d\n",mydata->t);


  return botinfo_buffer;
}
#include <jansson.h>
json_t *json_state();
#endif

int main() {

  SET_CALLBACK(obstacles, circle_barrier);
  // initialize hardware
  kilo_init();
  // register message callbacks
  kilo_message_rx = message_rx;
  kilo_message_tx = message_tx;
  // register your program
  kilo_start(setup, loop);

  SET_CALLBACK(botinfo, cb_botinfo); // (^_^)
  SET_CALLBACK(reset, setup);
  SET_CALLBACK(json_state, json_state);
  return(0);
}
