
// Declare constants
#define N AQUI
#define NEST_TWO N2
#define NEST_ONE N1
#define NO_DANCE 0 /* constant of no dancing */
#define LAMBDA INTERDEP
#define PRIORI_ONE PI1
#define PRIORI_TWO PI2
#define QUALITY_ONE Q1
#define QUALITY_TWO Q2
#define DELTA_T T
#define IND_SEED IS

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
  uint32_t i_was_dancing;
  uint32_t dance_time; /* If I am dancing, how much time left do I have to keep dancing */


  // other bot status variables
  uint32_t bot_id;
  uint32_t bot_dsite; /* Variable to save dancing status of bee */


  uint32_t signals_total;
  uint32_t signals_one;
  uint32_t f1;
  uint32_t f2;
  uint32_t signals_two;
  uint32_t *bots_dancing;


  uint32_t p_one;
  uint32_t p_two;
  uint32_t p_zero;
  uint32_t p_rand;

  uint8_t cycle;
  uint8_t bot_cycle;

  uint8_t current_state;
  uint8_t bot_state;
} USERDATA;


extern USERDATA *mydata;
