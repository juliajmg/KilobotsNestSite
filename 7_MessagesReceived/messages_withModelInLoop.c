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


/*                        FUNCTION TO UPDATE COLOR          */
void update_color()
{

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

        //mydata->turning_ticks = rand_soft()%mydata->max_turning_ticks + 1;

      rand = rand_soft() % 2;
      if(rand == 0){
        mydata->turning_ticks = 90;
      }
      if(rand == 1){
        mydata->turning_ticks = 180;
      }

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
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}

/*        MESSAGE RECEPTION AND TRANSMITTION FUNCTIONS        */

void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->new_message = 1;
  mydata->bot_id = message->data[1];
  mydata->bot_dsite = message->data[2];
  mydata->bot_qtime = message->data[3];



}


void reset_parameters(){
  mydata->signals_one = 0;
  mydata->signals_two = 0;
  mydata->signals_total = 0;
  mydata->bot_dsite = NO_DANCE;

  mydata->p_one = 0;
  mydata->p_two = 0;
  mydata->p_rand = 0;

  // Reiniciar array de bots vistos
  for(uint32_t i=0;i<N;i++){
    mydata->bots_dancing[i] = 0;
  }
}


void count_new_bot(){ // Junta información

  if(mydata->bots_dancing[mydata->bot_id]==0) {
    mydata->bots_dancing[mydata->bot_id] = 1;
    mydata->signals_total = mydata->signals_total + 1;

    if(mydata->bot_dsite == NEST_ONE){
      mydata->signals_one = mydata->signals_one + 1;
    }
    if(mydata->bot_dsite == NEST_TWO){
      mydata->signals_two = mydata->signals_two + 1;
    }
  }
}


void decide_dance(){
  // If you were dancing:
  if(mydata->i_was_dancing == 1){
    mydata->dancing = NO_DANCE;
    mydata->quality_time = 0;
    mydata->dance_time  = 0;
    mydata->i_was_dancing = 0;
  }
  // If you were not dancing:
  else {
    if(mydata->signals_total > 0){
      mydata->f1 = (mydata->signals_one*100)/mydata->signals_total;
      mydata->f2 = (mydata->signals_two*100)/mydata->signals_total;
    } else {
      mydata->f1 = 0;
      mydata->f2 = 0;
    }

    mydata->p_one = ((((100 - LAMBDA)*PRIORI_ONE)) + (LAMBDA*(mydata->f1)))/100;
    mydata->p_two = ((((100 - LAMBDA)*PRIORI_TWO)) + (LAMBDA*(mydata->f2)))/100;
    //mydata->p_two = 0;
    mydata->p_rand = rand_soft() % 100;

    if(mydata->p_rand < mydata->p_one){
      mydata->dancing = NEST_ONE;
      mydata->dance_time = kilo_ticks;
      mydata->d = QUALITY_ONE;
    }

    if((mydata->p_rand > mydata->p_one) & (mydata->p_rand <= mydata->p_one + mydata->p_two)){
      mydata->dancing = NEST_TWO;
      mydata->d = QUALITY_TWO;
      mydata->dance_time = kilo_ticks;


    }
    if((mydata->p_rand > mydata->p_one + mydata->p_two)){
      mydata->dancing = NO_DANCE;
      mydata->d = 0;
      mydata->dance_time  = 0;
    }
  }

}


void setup()
{
  // Random different for each bot
  rand_seed(kilo_uid + IND_SEED);
  // I start not dancing */
  mydata->my_id = kilo_uid;
  mydata->signals = 0;
  mydata->contador = 0;
  mydata->received_id = 0;
  //Set all bots opinions to 0
  mydata->bots_dancing = malloc(N*sizeof(uint32_t));

  reset_parameters();

  mydata->dancing = NO_DANCE;
  mydata->quality_time = 0;
  mydata->d = 0;
  mydata->t = kilo_ticks;
  mydata->i_was_dancing = 0;
  mydata->dance_time = 0;
  mydata->f1 = 0;
  mydata->f2 = 0;

  mydata->cycle = 0;

  // Random motion variables
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 180;
  mydata->max_straight_ticks = 60;
  mydata->last_motion_ticks = rand_soft() % mydata->max_straight_ticks + 1;

  set_motion(FORWARD);

}

void loop()
{
  mydata->contador = mydata->contador + 1;

  random_walk();
  if(mydata->new_message == 1){
    mydata->new_message = 0;
    mydata->signals = mydata->signals + 1;
    if(kilo_ticks % 100 < 98){
      set_color(RGB(0,1,0));
    }
    count_new_bot();

  }


    // NEW: Different time units to calculate probabilities and dance duration.


      if(mydata->dancing == NO_DANCE){
        if(kilo_ticks > mydata->t + (DELTA_T - 1)){
          mydata->t = kilo_ticks;
          decide_dance();
          reset_parameters();
        }
    }

    if(mydata->dancing == NEST_ONE) {
      mydata->cycle = 0;
      //mydata->d = mydata->d-1;
      set_color(RGB(1,0,0));
    }

    // If I am dancing for a site keep dancing
    if(mydata->dancing == NEST_TWO) {
      //mydata->d = mydata->N;
      mydata->cycle = 1;
      set_color(RGB(0,1,0));
    }

    if(mydata->dancing == NO_DANCE){
      set_color(RGB(0,0,1));
      mydata->cycle = 2;
      mydata->d = 0;
    }
    // If I danced "long enough", stop dancing
    if((mydata->dancing != NO_DANCE) && (kilo_ticks > mydata->t + (mydata->d - 2))){
      mydata->dancing = NO_DANCE;
      mydata->i_was_dancing = 1;
      //mydata->dance_time = kilo_ticks;
      reset_parameters();

    }

    //mydata->t = mydata->t + 1;

  update_message();


    #ifdef DEBUG
    printf("ID: %d\n", kilo_uid);
    printf("Neighbor ID: %d\n", mydata->bot_id);
    printf("Contador: %ld\n", mydata->contador);
    printf("Ticks: %ld\n", kilo_ticks);
    printf("Messages Received: %ld\n", mydata->signals);
    #endif
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
  p += sprintf (p, "t: %d\n", mydata->t);
  p += sprintf (p, "t + d: %d\n", mydata->t + (mydata->d - 1));
  p += sprintf (p, "d: %d\n", mydata->d);


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
