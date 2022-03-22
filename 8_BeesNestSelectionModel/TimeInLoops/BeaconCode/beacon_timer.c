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


message_t *message_tx()
{
  return &mydata->transmit_msg;


}

void update_message() {
  mydata->transmit_msg.data[1] = mydata->my_id;
  mydata->transmit_msg.data[2] = 0;
  mydata->transmit_msg.data[3] = mydata->cycle;

  mydata->transmit_msg.type = NORMAL;
  mydata->transmit_msg.crc = message_crc(&mydata->transmit_msg);
}


/*void message_rx(message_t *message, distance_measurement_t *d) {

  mydata->dist = *d;
  mydata->new_message = 1;
  mydata->bot_id = message->data[1];
  mydata->bot_dsite = message->data[2];
  mydata->bot_cycle = message->data[3];

}
*/





void setup()
{

  mydata->my_id = kilo_uid;

  // Variable to update time
  mydata->t = 0;

  mydata->cycle = 0;

  //set_motion(STOP);

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
    if(mydata->cycle == 29){
      set_color(RGB(0,1,0));
    } else{
      if(mydata->cycle == 255){
        set_color(RGB(0,2,1));
      } else {
        set_color(RGB(0,0,0));
      }
    }


    update_message();


    #ifdef DEBUG
    printf("Contador: %ld\n", mydata->t);
    printf("Ticks: %ld\n", kilo_ticks);
    printf("Ciclos: %d\n", mydata->cycle);

    #endif
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
