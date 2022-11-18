
// Declare constants
#define N 37
#define NEST_TWO 2
#define NEST_ONE 1 /* k number of nests  */
#define NO_DANCE 0 /* constant of no dancing */
#define LAMBDA 00
#define PRIORI_ONE 30
#define PRIORI_TWO 30
#define QUALITY_ONE 5600
#define QUALITY_TWO 8000
#define DELTA_T 800
#define IND_SEED 195
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
  uint32_t d; /*Dance time variable*/
  uint8_t dflag;
  uint32_t quality_time; /* Time to dance for site depending on assesment of quality*/

  // other bot status variables
  uint32_t bot_id;
  uint32_t bot_dsite; /* Variable to save dancing status of bee */
  uint32_t bot_qtime; /* Copy time of dancing of bot */

  uint32_t signals_total;
  uint32_t signals_one;
  uint32_t f1;
  uint32_t f2;
  uint32_t signals_two;
  uint32_t *bots_dancing;


  uint8_t p_one;
  uint8_t p_two;
  uint8_t p_zero;
  uint8_t p_rand;
  uint8_t p;

  /*Galla variables */
  uint8_t pstop_temp;
  uint8_t p_stop;

  uint32_t update_time;
  uint32_t counter;
  uint32_t t;

} USERDATA;

extern USERDATA *mydata;
