/* Saving bot state as json. Not for use in the real bot, only in the simulator. */
#include <kilombo.h>

#ifdef SIMULATOR

#include <jansson.h>
#include <stdio.h>
#include <string.h>

#include "message_rate.h"

json_t *json_state()
{
  //create the state object we return
  json_t* state = json_object();

  json_t* b = json_integer(mydata->bots_seen);
  json_object_set(state, "BotsSeen", b);

  return state;
}

#endif
