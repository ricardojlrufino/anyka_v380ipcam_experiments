/**
 * Arduino Style Blink
 * by Ricardo JL Rufino
 * More info:
 * https://ricardojlrufino.wordpress.com/2022/02/14/hack-ipcam-anyka-teardown-and-root-access/
 */

#include "platform/Arduino.h"

volatile sig_atomic_t stop;


void inthand(int signum) {
    stop = 1;
}


int main(int argc, char *argv[]){

	signal(SIGINT, inthand); // stop on CTRL+C

    pinMode(13, AK_GPIO_DIR_OUTPUT); // IR_CUT_B (pin1) - 5V LEVEL (~4v)
    pinMode(14, AK_GPIO_DIR_OUTPUT); // IR_CUT_A (pin2) - 5V LEVEL (~4v)

    digitalWrite(13, AK_GPIO_OUT_LOW); // SET AS GND

    while (!stop){

    	digitalWrite(14, AK_GPIO_OUT_HIGH);
		printf("ON \n");
		sleep(1);
		digitalWrite(14, AK_GPIO_OUT_LOW);
		printf("OFF \n");
		sleep(1);
	}

    close(GPIO_FD);
    return 0;
}
