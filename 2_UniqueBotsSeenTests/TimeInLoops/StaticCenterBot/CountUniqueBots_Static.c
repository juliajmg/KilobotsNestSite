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


void random_walk(){
  uint8_t rand;
  switch(mydata->current_motion_type) {
    case LEFT:
    case RIGHT:
    if(kilo_ticks > mydata->last_motion_ticks + mydata->turning_ticks) {
      /* start moving forward */
      mydata->last_motion_ticks = kilo_ticks;
      set_motion(FORWARD);
    }
    break;
    case FORWARD:
    if( kilo_ticks > mydata->last_motion_ticks + mydata->max_straight_ticks) {
      /* perform a radnom turn */
      mydata->last_motion_ticks = kilo_ticks;
      if( rand_soft()%2) {
        set_motion(LEFT);
      }
      else {
        set_motion(RIGHT);
      }

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
    set_motion(FORWARD);
  }
}

/*        MESSAGE RECEPTION AND TRANSMITTION FUNCTIONS        */

message_t *message_tx()
{
  return &mydata->transmit_msg;
}

void update_message() {
  mydata->transmit_msg.data[1] = mydata->my_id;
  mydata->transmit_msg.data[2] = mydata->cycle;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}

void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->dist = *d;
  mydata->new_message = 1;
  mydata->bot_id = message->data[1];
  mydata->bot_cycle = message->data[2];

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
  for(uint8_t i=0;i<N;i++){
    mydata->bots_dancing[i] = 0;
  }
}


void count_new_bot(){
  if((mydata->bot_id != BEACON_1) && (mydata->bot_id != BEACON_2) && (mydata->bot_id != BEACON_3)){
    mydata->distance = estimate_distance(&mydata->dist);

    if((mydata->bots_dancing[mydata->bot_id]==0) & (mydata->distance <= 200)) {
      mydata->bots_dancing[mydata->bot_id] = 1;
      mydata->signals_total = mydata->signals_total + 1;
    }
  }
}





void setup()
{
  mydata->distance = 200;
  // Random different for each bot
  rand_seed(kilo_uid + IND_SEED);
  mydata->my_id = kilo_uid;

  // Allocate memory for bots seen array.
  mydata->bots_dancing = malloc(N*sizeof(uint8_t));

  reset_parameters();

  mydata->f1 = 0;
  mydata->f2 = 0;

  // Random probabilities variables
  mydata->p = 250;
  mydata->p_rand = 99;
  mydata->p_start = 35;
  mydata->t = 0;

  // Random motion variables
  mydata->last_motion_ticks = 0;
  mydata->turning_ticks = 0;
  mydata->max_turning_ticks = 180;
  mydata->max_straight_ticks = 120;
  mydata->last_motion_ticks = rand_soft() % mydata->max_straight_ticks + 1;


  // Deltat sync variables
  mydata->cycle = 0;
  mydata->bot_cycle = 0;
  mydata->bots_seen = 0;
  // Two beacons down-the-glass to cover arena radius and amplify beacon 1 (clock) signal.

  set_motion(STOP);

  mydata->ptemp_start = rand_soft(); // random number to start dancing initially for a site.

  set_color(RGB(0,0,0));
}

void loop()
{
  /* 10/03/2022: Bots won't start until beacon cycle > 30 */
  if(mydata->bot_cycle >= 1){

    if(mydata->new_message == 1){
      mydata->new_message = 0;
      count_new_bot();
    }


    if(mydata->t % DELTA_T == (DELTA_T - 3)){
      mydata->p = rand_soft();
    }

    while(mydata->p >= (255 - (255 % 100))){
      mydata->p = rand_soft();
    }
    if(mydata->p < 200){
      mydata->p_rand = mydata->p % 100;
    }
    /* if new cycle */
    if(mydata->bot_cycle > mydata->cycle){
      mydata->cycle = mydata->bot_cycle;
      mydata->bots_seen = mydata->signals_total;
      reset_parameters();
    }

  mydata->t = mydata->t + 1;
  }

  update_message();
  if(mydata->my_id != BEACON_2 && mydata->my_id != BEACON_3){
    if(mydata->bots_seen < 3){
      set_color(RGB(0,0,0));
    }
    if((mydata->bots_seen >= 3) & (mydata->bots_seen < 6)){
      set_color(RGB(1,0,0));
    }
    if((mydata->bots_seen >=6) & (mydata->bots_seen < 9)) {
      set_color(RGB(0,1,0));
    }
    if((mydata->bots_seen >= 9) & (mydata->bots_seen < 12)){
      set_color(RGB(0,0,1));
    }
    if(mydata->bots_seen >= 12){
      set_color(RGB(1,1,1));
    }
  }
  #ifdef DEBUG
  printf("ID: %ld\n", mydata->my_id);
  printf("t: %ld\n", mydata->t % DELTA_T);
  printf("cycle: %d\n", mydata->cycle);
  printf("bot_id:%ld\n", mydata->bot_id);
  printf("bots_seen: %ld\n", mydata->bots_seen);
  printf("BotDistance: %d\n", mydata->distance);
  printf("signals_total: %ld\n", mydata->signals_total);
  /*for(uint8_t i=0;i<N;i++){
    printf("Bot %d:", i);
    printf("%d\n", mydata->bots_dancing[i]);
  }*/
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
  p += sprintf (p, "p: %d\n", mydata->p_rand);
  p += sprintf (p, "delta t: %d\n",  mydata->dance_time);
  p += sprintf (p, "t: %d\n", mydata->t);



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
