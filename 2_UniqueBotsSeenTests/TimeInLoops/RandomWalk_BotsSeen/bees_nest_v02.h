
// Declare constants
#define N 50
#define NEST_TWO 2
#define NEST_ONE 1
#define NO_DANCE 0 /* constant of no dancing */
#define LAMBDA 0
#define PRIORI_ONE 30
#define PRIORI_TWO 30
#define QUALITY_ONE 7
#define QUALITY_TWO 10
#define DELTA_T 800
#define IND_SEED 23

#define BEACON_1 17
#define BEACON_2 18
#define BEACON_3 20
#define BEACON_4 31

#define WAIT 1
#define CONTINUE 0
// declare motion variable type
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;


typedef struct {
  message_t transmit_msg;
  uint8_t new_message;
  distance_measurement_t dist;
  uint8_t distance;

  uint8_t time_ticks;
  uint8_t random_motor;

  // Random motion of Reina et al. 2018
  uint32_t last_motion_ticks;
  uint8_t turning_ticks;
  uint8_t max_turning_ticks; /* allow a maximum rotation of 180 degrees with \omega=\pi/5 */
  uint32_t max_straight_ticks;
  motion_t current_motion_type;

  // my status variables
  uint32_t my_id;
  uint32_t dancing; /* variable that indicates if I am dancing */
  uint32_t dance_time; /* If I am dancing, how much time left do I have to keep dancing */


  // other bot status variables
  uint32_t bot_id;
  uint32_t bot_dsite; /* Variable to save dancing status of bee */


  uint32_t signals_total;
  uint32_t bots_seen;
  uint32_t signals_one;
  uint32_t f1;
  uint32_t f2;
  uint32_t signals_two;
  uint8_t *bots_dancing;
  uint32_t color_flag;

  // Random for probabilities variables
  uint8_t p;
  uint32_t t;


  uint32_t p_one;
  uint32_t p_two;
  uint32_t p_zero;
  uint8_t p_rand;

  uint8_t ptemp_start;
  uint8_t p_start;

  uint8_t cycle;
  uint8_t bot_cycle;

  uint8_t current_state;
  uint8_t bot_state;
} USERDATA;


extern USERDATA *mydata;
