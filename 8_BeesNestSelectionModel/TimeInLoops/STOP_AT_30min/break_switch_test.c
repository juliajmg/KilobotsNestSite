#include <kilombo.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "time_delay.h"



#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
#endif


REGISTER_USERDATA(USERDATA);


void wait_cycles(){
  switch(mydata->current_state) {
    case WAIT:
    if(kilo_ticks > mydata->last_ticks + mydata->wait_ticks) {

      mydata->current_state = CONTINUE;

    } else {
      set_color(RGB(0,0,2));
    }
    break;
    case CONTINUE:
    if(mydata->cycle == 40) {
      mydata->last_ticks = kilo_ticks;
      mydata->current_state = WAIT;
      mydata->cycle = 41; 
    } else {
      set_color(RGB(0,0,0));
      mydata->t = mydata->t + 1;
      if(mydata->t % DELTA_T == DELTA_T - 1){
      //if(kilo_ticks >= (mydata->t + (DELTA_T))){
        mydata->cycle = mydata->cycle + 1;
      }
    }
    break;
    default:
    mydata->current_state = CONTINUE;
  }
}




void setup()
{
  mydata->t = 0;
  mydata->last_ticks = 0;
  mydata->wait_ticks = 500;
  mydata->cycle = 0;
  mydata->current_state= CONTINUE;

}

void loop()
{
  wait_cycles();



}

//*************************************** (^_^) ******************************/
#ifdef SIMULATOR
/* provide a text string for the simulator status bar about this bot */
static char botinfo_buffer[10000];
char *cb_botinfo(void)
{
  char *p = botinfo_buffer;
  p += sprintf (p, "current state: %d\n", mydata->current_state);
  p += sprintf (p, "delta t: %d\n",  mydata->cycle);
  p += sprintf (p, "t: %d\n", mydata->t);


  return botinfo_buffer;
}
#endif

int main() {

  // initialize hardware
  kilo_init();


  kilo_start(setup, loop);

  SET_CALLBACK(botinfo, cb_botinfo); // (^_^)
  SET_CALLBACK(reset, setup);
  return(0);
}
