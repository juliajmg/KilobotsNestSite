

#define IND_SEED 21
#define DELTA_T 100

#define WAIT 1
#define CONTINUE 0

typedef struct {
  message_t transmit_msg;
  uint8_t new_message;
  distance_measurement_t dist;
  uint32_t my_id;
  uint32_t t;
  uint8_t cycle;
  uint32_t last_ticks;
  uint32_t wait_ticks;
  uint8_t current_state;

} USERDATA;

extern USERDATA *mydata;
