
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

  // Message reception and transimition variables
  message_t transmit_msg;
  uint8_t new_message;
  uint8_t my_id;
  uint8_t received_id;
  uint32_t signals;
  uint32_t contador;
  uint8_t current_tick;

} USERDATA;

extern USERDATA *mydata;
