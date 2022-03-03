#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "bees_nest_v02.h"



#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
#define DEBUG     // for printf to serial port
#include "debug.h"
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



message_t *message_tx()
{
  mydata->transmit_msg.data[1] = mydata->my_id;

  return &mydata->transmit_msg;

}

void update_message() {

  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);

}

/*        MESSAGE RECEPTION AND TRANSMITTION FUNCTIONS        */

void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->dist = *d;
  mydata->new_message = 1;
  mydata->bot_id = message->data[1];


}


void setup()
{
  mydata->distance = 0;
  // Random different for each bot
  rand_seed(kilo_uid + IND_SEED);
  mydata->bots_dancing = malloc(N*sizeof(uint32_t));

  // I start not dancing */
  mydata->my_id = kilo_uid;

  mydata->bot_id = 0;
  //Set all bots opinions to 0

  set_motion(STOP);

  set_color(RGB(0,0,0));
  mydata->t = 0;
  mydata->signals_total = 0;
  // Reiniciar array de bots vistos
 for(uint32_t i=0;i<40;i++){
    mydata->bots_dancing[i] = 0;
  }

}

void loop()
{
  //set_color(RGB(0,1,0));
  if(mydata->new_message == 1){
    mydata->distance = estimate_distance(&mydata->dist);


    if(mydata->distance > 200){
      mydata->distance = 100;
    }
    if(mydata->distance <= 70){
      set_color(RGB(0,0,1));
    } else {
      set_color(RGB(1,0,0));
    }
    if((mydata->bots_dancing[mydata->bot_id]==0) & (mydata->distance <= 70)) {
        mydata->bots_dancing[mydata->bot_id] = 1;
        mydata->signals_total = mydata->signals_total + 1;
      }
    mydata->new_message = 0;
    //set_color(RGB(0,0,0));
  }
  /* Change led color every new cycle with bots seen */
update_message();

if(kilo_ticks % 300 == 299){
  set_color(RGB(0,0,0));
}

  #ifdef DEBUG
  printf("ID: %d\n", kilo_uid);
  printf("Neighbor ID: %d\n", mydata->bot_id);
  printf("Distance: %d\n", mydata->distance);
  printf("Bots seen: %ld\n", mydata->signals_total);
  #endif

mydata->t = (mydata->t + 1) % 400;

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

  p += sprintf (p, "distance: %d\n",  mydata->distance);
  p += sprintf (p, "distance: %d\n",  mydata->bot_id);


  return botinfo_buffer;
}
#include <jansson.h>
json_t *json_state();
#endif

int main() {

  SET_CALLBACK(obstacles, circle_barrier);
  // initialize hardware
  kilo_init();
  #ifdef DEBUG
    debug_init();
  #endif
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
