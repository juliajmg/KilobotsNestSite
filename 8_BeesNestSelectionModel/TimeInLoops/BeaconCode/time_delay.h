

#define IND_SEED 21
#define DELTA_T 800

typedef struct {
  message_t transmit_msg;
  uint8_t new_message;
  distance_measurement_t dist;
  uint32_t my_id;
  uint32_t t;
  uint8_t cycle;


} USERDATA;

extern USERDATA *mydata;
