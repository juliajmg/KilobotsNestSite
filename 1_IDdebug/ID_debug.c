#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>


#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
//  #define DEBUG     // for printf to serial port
//  #include "debug.h"
#endif



// declare motion variable type
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;


typedef struct {
  uint8_t time_ticks;
  uint8_t random_motor;
  // Random motion of Reina et al. 2018
  uint32_t last_motion_ticks;
  uint8_t turning_ticks;
  uint8_t max_turning_ticks; /* constant to allow a maximum rotation of 180 degrees with \omega=\pi/5 */
  uint32_t max_straight_ticks;
  motion_t current_motion_type;

  // Message variables
  message_t transmit_msg;
  uint8_t new_message;
  distance_measurement_t dist;

  // Choreography variables
  uint32_t ticks;
  uint8_t dflag;
  uint8_t my_id;
} USERDATA;

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



void setup()
{
  mydata->my_id = kilo_uid % 10;

  if(mydata->my_id == 0){
    set_color(RGB(1,1,1));
  }
  if(mydata->my_id == 1) {
    set_color(RGB(0,0,1));
  }
  if(mydata->my_id == 2) {
    set_color(RGB(0,1,0));
  }
  if(mydata->my_id == 3){
    set_color(RGB(1,0,0));
  }
  if(mydata->my_id == 4){
    set_color(RGB(0,1,1));
  }
  if(mydata->my_id == 5){
    set_color(RGB(1,0,1));
  }
  if(mydata->my_id == 6){
    set_color(RGB(1,1,0));
  }
  if(mydata->my_id == 7){
    set_color(RGB(0,1,3));
  }
  if(mydata->my_id == 8){
    set_color(RGB(3, 1, 0));
  }
  if(mydata->my_id == 9){
    set_color(RGB(1,3,0));
  }
}



void loop()
{
  if(mydata->my_id == 0){
    set_color(RGB(1,1,1));
  }
  if(mydata->my_id == 1) {
    set_color(RGB(0,0,1));
  }
  if(mydata->my_id == 2) {
    set_color(RGB(0,1,0));
  }
  if(mydata->my_id == 3){
    set_color(RGB(1,0,0));
  }
  if(mydata->my_id == 4){
    set_color(RGB(0,1,1));
  }
  if(mydata->my_id == 5){
    set_color(RGB(1,0,1));
  }
  if(mydata->my_id == 6){
    set_color(RGB(1,1,0));
  }
  if(mydata->my_id == 7){
    set_color(RGB(0,1,3));
  }
  if(mydata->my_id == 8){
    set_color(RGB(3, 1, 0));
  }
  if(mydata->my_id == 9){
    set_color(RGB(1,3,0));
  }

}

//*************************************** (^_^) ******************************1
#ifdef SIMULATOR
/* provide a text string for the simulator status bar about this bot */
static char botinfo_buffer[10000];
char *cb_botinfo(void)
{
  char *p = botinfo_buffer;
  p += sprintf(p, "My ID: %d\n", mydata->my_id);
  p += sprintf(p, "kilo_uid: %d\n", kilo_uid);

  return botinfo_buffer;
}

#endif

int16_t circle_barrier(double x, double y, double * dx, double * dy)
{
  double d = sqrt(x*x + y*y);

  //if (d < 200.0)
  if(d < 700)
    return 0;

  *dx = -x/d;
  *dy = -y/d;

  return 1;
 }



int main() {

    SET_CALLBACK(obstacles, circle_barrier);
    // initialize hardware
    kilo_init();

    // register your program
    kilo_start(setup, loop);

    SET_CALLBACK(botinfo, cb_botinfo); // (^_^)
    SET_CALLBACK(reset, setup);

    return(0);
}
