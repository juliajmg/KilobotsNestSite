#define N 50
#define DELTAT 1000
#define IND_SEED 30
#define MIN 10
#define MID 20
#define MAX 30


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

  uint32_t big_turn;
  // Message reception and transimition variables
  message_t transmit_msg;
  uint8_t new_message;
  uint32_t my_id;
  uint32_t received_id;
  distance_measurement_t dist;


  // N of slots = N of bots, within each slot: 0 = no opinion, 1 = option 1, 2 = option 2.
  uint8_t *bots_array;


  uint8_t bots_seen;
  uint32_t last_update;





} USERDATA;

extern USERDATA *mydata;
