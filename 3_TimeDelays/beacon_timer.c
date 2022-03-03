#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "time_delay.h"



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
  return &mydata->transmit_msg;


}

void update_message() {
  mydata->transmit_msg.data[1] = mydata->my_id;
  mydata->transmit_msg.data[2] = 0;
  mydata->transmit_msg.data[3] = 0;
  mydata->transmit_msg.data[4] = mydata->cycle;
  mydata->transmit_msg.type = NORMAL;

  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}


/*void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->dist = *d;
  mydata->new_message = 1;
  mydata->bot_id = message->data[1];
  mydata->bot_dsite = message->data[2];
  mydata->bot_qtime = message->data[3];
  mydata->bot_cycle = message->data[4];

}
*/





void setup()
{
  // Random different for each bot
  mydata->my_id = kilo_uid;

  // Variable to update time
  mydata->t = 0;

  mydata->cycle = 0;

  set_motion(STOP);

  set_color(RGB(0,0,0));

}

void loop()
{

    mydata->t = mydata->t + 1;
    /*If bot is the beacon, reset time every delta t, and add one delta_t cycle to transmitted cycle variable  */

    if(mydata->t % DELTA_T == DELTA_T - 1){
      //if(kilo_ticks >= (mydata->t + (DELTA_T))){

    mydata->cycle = mydata->cycle + 1;
        //mydata->t = kilo_ticks;

    }


    if(mydata->cycle % 3 == 0){
      set_color(RGB(1,0,0));
    }
    if(mydata->cycle % 3 == 1){
      set_color(RGB(0,1,0));
    }
    if(mydata->cycle % 3 == 2){
      set_color(RGB(0,0,1));
    }
  //}
    update_message();


    #ifdef DEBUG
    //printf("ID: %d\n", kilo_uid);
    //printf("Neighbor ID: %ld\n", mydata->bot_id);
    printf("Contador: %ld\n", mydata->t);
    printf("Ticks: %ld\n", kilo_ticks);
    printf("Ciclos: %d\n", mydata->cycle);
    //printf("t: %ld\n", mydata->t % DELTA_T);
    //printf("Ticks: %ld\n", kilo_ticks);
    //printf("Cycle = %d\n", mydata->cycle);
    #endif
  }


  int16_t circle_barrier(double x, double y, double * dx, double * dy)
  {
    double d = sqrt(x*x + y*y);

    if (d < 200.0)
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
    p += sprintf (p, "ID: %d\n", kilo_uid);
    p += sprintf (p, "cycle: %d\n",mydata->cycle);
  //  p += sprintf (p, "bot_cycle: %d\n", mydata->bot_cycle);
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

    //kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    // register your program
    kilo_start(setup, loop);

    SET_CALLBACK(botinfo, cb_botinfo); // (^_^)
    SET_CALLBACK(reset, setup);
    SET_CALLBACK(json_state, json_state);
    return(0);
  }
