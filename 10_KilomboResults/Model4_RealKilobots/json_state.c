/* Saving bot state as json. Not for use in the real bot, only in the simulator. */
#include <kilombo.h>

#ifdef SIMULATOR

#include <jansson.h>
#include <stdio.h>
#include <string.h>

#include "bees_model=4.h"

json_t *json_state()
{
  //create the state object we return
  json_t* state = json_object();

  json_t* c = json_integer(mydata->signals_one);
  json_object_set(state, "bots1", c);
    // store the dance time
  json_t* d = json_integer(mydata->signals_two);
  json_object_set(state, "bots2", d);

  json_t* f = json_integer(mydata->signals_total);
  json_object_set(state, "BotsSeen", f);

  json_t* e = json_integer(mydata->dancing);
  json_object_set(state, "Dancing", e);

  /*json_t* j = json_integer(mydata->i_wasnt_dancing);
  json_object_set(state, "PreviousDance", j);*/

  json_t* j = json_integer(mydata->t);
  json_object_set(state, "time", j);
  // store the dance time
  json_t* g = json_integer(mydata->dance_time);
  json_object_set(state, "Dance time", g);

  //json_t* h = json_integer(mydata->p_rand);
  //json_object_set(state, "RandomProb", h);

  //json_t* k = json_integer(mydata->p);
  //json_object_set(state, "Prob", k);
  return state;
}

#endif
