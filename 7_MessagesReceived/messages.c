#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "messages.h"


#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
#define DEBUG     // for printf to serial port
#include "debug.h"
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
  mydata->new_message = 0;
  mydata->transmit_msg.type = NORMAL;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);

  rand_seed(kilo_uid + 1);
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 180;
  mydata->max_straight_ticks = 120;
  mydata->current_motion_type = STOP;


  mydata->received_id = 0;
  mydata->my_id = kilo_uid;
  mydata->signals = 0;
  mydata->current_tick = 0;
  mydata->contador = 0;

  set_color(RGB(1,0,0));
  set_motion(STOP);


}

void loop()

{

  mydata->contador = mydata->contador + 1;
  set_color(RGB(0,0,0));
  if(mydata->new_message == 1) {
    mydata->signals = mydata->signals + 1;
    if(kilo_ticks % 100 < 98){
      set_color(RGB(0,1,0));
    }
    mydata->new_message = 0;
  }

  update_message();

  if(mydata->contador % 101 == 100){
    mydata->current_tick = kilo_ticks % 100;

  }



  #ifdef DEBUG
  printf("ID: %d\n", kilo_uid);
  printf("Neighbor ID: %d\n", mydata->received_id);
  printf("Contador: %d\n", mydata->contador);
  printf("Ticks: %d\n", kilo_ticks);
  printf("Messages Received: %d\n", mydata->signals);
  #endif

}


int16_t circle_barrier(double x, double y, double * dx, double * dy)
{
  double d = sqrt(x*x + y*y);

  if (d < 1000.0)
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
   p += sprintf (p, "Signals: %d\n", mydata->signals);


   return botinfo_buffer;
 }
 #endif


int main() {


    // initialize hardware
    kilo_init();
    #ifdef DEBUG
      debug_init();
    #endif
    SET_CALLBACK(botinfo, cb_botinfo); // (^_^)

    SET_CALLBACK(obstacles, circle_barrier);
    SET_CALLBACK(reset, setup);
    // register message callbacks
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;

    // register your program
    kilo_start(setup, loop);

    return(0);
}
