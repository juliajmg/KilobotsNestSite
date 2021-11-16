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


#define DELTA_T 100


typedef struct {
  uint32_t last_update;
  uint8_t my_cycle;
} USERDATA;

REGISTER_USERDATA(USERDATA);


void setup()
{
  /* 1. Guardo kilo_ticks en la variable last_update */
  mydata->last_update = kilo_ticks;
  mydata->my_cycle = 0;
}

void loop()
{
  /* 2. En el loop, en el instante en el que los kilo_ticks sean mayores a la última actualización + Delta_t, vuelves a guardar los kilo_ticks en la variable last_update.
  Ejemplo:
  En el primer Delta_t (para Delta_t = 100 kilo_ticks) de la simulación, la variable last_update, que era 0, cambia su valor a last_update = 100.
  Para el segundo Delta_t, kilo_ticks = 200 y last_update cambia de valor de 100 a 200.
   */
  if(kilo_ticks >= mydata->last_update + DELTA_T){

    mydata->last_update = kilo_ticks;

    /* Aquí estoy cambiando el ciclo para encender luz roja durante Delta_t, y cambiar a luz verde en el siguente Delta_t */
    if(mydata->my_cycle == 0){
      mydata->my_cycle = 1;
    } else {
      mydata->my_cycle = 0;
    }
  }

  if(mydata->my_cycle == 0){
    set_color(RGB(1,0,0));
  } else {
    set_color(RGB(0,1,0));
  }
}


//*************************************** (^_^) ******************************1
#ifdef SIMULATOR
/* provide a text string for the simulator status bar about this bot */
static char botinfo_buffer[10000];
char *cb_botinfo(void)
{
  char *p = botinfo_buffer;
  p += sprintf(p, "last_update: %d\n", mydata->last_update);


  return botinfo_buffer;
}
#endif
//****************************************** (^_^) ****************************2




int main() {

  // initialize hardware
  kilo_init();

  // register your program
  kilo_start(setup, loop);

  SET_CALLBACK(botinfo, cb_botinfo); // (^_^)
  SET_CALLBACK(reset, setup);

  return(0);
}
