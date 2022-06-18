
// Declare constants
#define N AQUI
#define NEST_TWO 2
#define NEST_ONE 1 /* k number of nests  */
#define NO_DANCE 0 /* constant of no dancing */
#define LAMBDA INTERDEP
#define PRIORI_ONE PI1
#define PRIORI_TWO PI2
#define QUALITY_ONE Q1
#define QUALITY_TWO Q2
#define DELTA_T T
#define IND_SEED IS


typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;


typedef struct {


  // Random motion of Reina et al. 2018
  uint32_t last_motion_ticks;
  uint8_t turning_ticks;
  uint8_t max_turning_ticks; /* allow a maximum rotation of 180 degrees with \omega=\pi/5 */
  uint32_t max_straight_ticks;
  motion_t current_motion_type;

} USERDATA;


extern USERDATA *mydata;
