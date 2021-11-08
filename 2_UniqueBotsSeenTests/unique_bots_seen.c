#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "message_rate.h"


#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
//  #define DEBUG     // for printf to serial port
//  #include "debug.h"
#endif

REGISTER_USERDATA(USERDATA)





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
  uint8_t rand;
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
      if( rand_soft()%2 ) {
        set_motion(LEFT);
        //set_color(RGB(0,0,1));
      }
      else {
        set_motion(RIGHT);
        //set_color(RGB(1,0,0));
      }
      mydata->turning_ticks = rand_soft()%mydata->max_turning_ticks + 1;
      /*rand = rand_soft() % 2;
      if(rand == 0){
        mydata->turning_ticks = 90;
      }
      if(rand == 1){
        mydata->turning_ticks = 180;
      }*/

    }
    break;
    case STOP:
    default:
    set_motion(STOP);
  }
}

/*        MESSAGE RECEPTION AND TRANSMITTION FUNCTIONS        */

void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->received_id = message->data[0];


  mydata->dist = *d; // (^_^)

  mydata->new_message = 1;

}

message_t *message_tx()
{
  return &mydata->transmit_msg;
}

void update_message() {
  mydata->transmit_msg.data[0] = mydata->my_id;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}



void setup()
{
  //set_motion(STOP);
  //Message reception and transmittion
  mydata->my_id = kilo_uid;

  mydata->new_message = 0;
  mydata->transmit_msg.type = NORMAL;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);

  rand_seed(kilo_uid + 1);
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 192;
  mydata->max_straight_ticks = 120;
  mydata->current_motion_type = STOP;

  set_motion(FORWARD);
  //mydata->last_motion_ticks = rand_soft() % mydata->max_straight_ticks + 1;
  mydata->bots_seen = 0;
  mydata->bots_array = malloc(N*sizeof(uint8_t));

  for(uint8_t i=0;i<N;i++){
    mydata->bots_array[i] = 0;
  }

  mydata->last_update = kilo_ticks;




}

void loop()
{
  random_walk();
  if(mydata->new_message == 1) {
    mydata->new_message = 0;
    if(mydata->bots_array[mydata->received_id]==0) {

      mydata->bots_seen = mydata->bots_seen + 1;
      mydata->bots_array[mydata->received_id] = 1;

    }

  }

  update_message();
  //if(kilo_ticks > mydata->last_update + DELTAT){
      if((mydata->bots_seen == 0)){
        set_color(RGB(0,0,0));
      }
      if((mydata->bots_seen >= 1) & (mydata->bots_seen < 10)){
        set_color(RGB(1,0,0));
      }
      if((mydata->bots_seen >= 10) & (mydata->bots_seen < 15)){
        set_color(RGB(0,1,0));
      }
      if((mydata->bots_seen >= 15) & (mydata->bots_seen < 20)){
        set_color(RGB(0,0,1));
      }
      if(mydata->bots_seen > 20){
        set_color(RGB(1,1,1));
      }

    //}

}


//*************************************** (^_^) ******************************1
#ifdef SIMULATOR
/* provide a text string for the simulator status bar about this bot */
static char botinfo_buffer[10000];
char *cb_botinfo(void)
{
  char *p = botinfo_buffer;
  p += sprintf(p, "Bots Seen: %d\n", mydata->bots_seen);


  return botinfo_buffer;
}
#include <jansson.h>

json_t *json_state();

#endif
//****************************************** (^_^) ****************************2




int main() {

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
