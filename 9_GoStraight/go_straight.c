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

/*
void random_walk() {
  //uint8_t time_angle[6] = {62, 93, 124, 155, 186, 217};

  if(kilo_ticks % (mydata->time_ticks + 217) == (mydata->time_ticks + 216)) {
    //mydata->time_ticks = time_angle[rand_soft() % 6];
    mydata->time_ticks = rand_soft() % 217;
    mydata->random_motor = rand_soft() % 2;
  } else {
    if(mydata->random_motor == 1) {
      set_motion(RIGHT);
      set_color(RGB(0,0,1));
    } else {
      set_motion(LEFT);
      set_color(RGB(1,0,0));
    }
  }
}
*/

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

void setup()
{
  //set_motion(STOP);


  rand_seed(kilo_uid + 1);
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 192;
  mydata->max_straight_ticks = 60;
  mydata->current_motion_type = STOP;

  set_motion(FORWARD);
  mydata->last_motion_ticks = rand_soft() % mydata->max_straight_ticks + 1;

}

void loop()
{
  set_motion(FORWARD);

  set_color(RGB(0,0,2));
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



int main() {

    SET_CALLBACK(obstacles, circle_barrier);
    // initialize hardware
    kilo_init();
    // register your program
    kilo_start(setup, loop);
    return(0);
}
