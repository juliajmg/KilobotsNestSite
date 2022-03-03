#include <kilombo.h>
#include <avr/eeprom.h>
//#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>

#ifdef SIMULATOR
#include <stdio.h> // for printf
#else
#include <avr/io.h>  // for microcontroller register defs
//  #define DEBUG     // for printf to serial port
//  #include "debug.h"
#endif


//used locations in eeprom
#define ee_OSCCAL 0x001  // rc calibration value in eeprom, to be loaded to OSCCAL at startup
#define ee_SENSOR_LOW 0x20  //low gain sensor calibration data in epromm
#define ee_SENSOR_HIGH 0x50  //high-gain calibration data in epromm

void setup() {
    	static uint32_t Cal_Low[15] = {277,273,266,250,231,213,196,181,171,159,148,141,131,122,113};
	    static uint32_t Cal_High[15] = {1098,1088,1080,1047,1014,960,881,823,777,727,681,645,619,568,531};





    uint8_t i = 0;






	while(i<=14)
		{
		set_color(RGB(0,1,0));
		eeprom_write_byte((uint8_t *)(ee_SENSOR_LOW+i*2),((Cal_Low[i])>>8));
		//_delay_ms(100);
		set_color(RGB(0,0,0));
		eeprom_write_byte((uint8_t *)(ee_SENSOR_LOW+i*2+1),(Cal_Low[i] & 0x00ff));
		//_delay_ms(100);


		set_color(RGB(0,1,0));
		eeprom_write_byte((uint8_t *)(ee_SENSOR_HIGH+i*2),((Cal_High[i])>>8));

		//_delay_ms(100);
		set_color(RGB(0,0,0));
		eeprom_write_byte((uint8_t *)(ee_SENSOR_HIGH+i*2+1),(Cal_High[i] & 0x00ff));
		//_delay_ms(100);
		i++;
		}




}

void loop() {
    // put your main code here, to be run repeatedly
}

int main() {
    // initialize hardware
    kilo_init();
    // start program
    kilo_start(setup, loop);


    return 0;
}
