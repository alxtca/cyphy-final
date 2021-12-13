#include <iostream>
#include <wiringPi.h> /* for simpler use of gpio pins when using C++, 
* making them behave similar to those of arduino. Includes many auxilary
* functions, similar to those of arduino.
* 
* However...
* 
* This library needs to be manually linked to the compiler environment, 
* or else compilation will fail. 
* 
* In Geany, this is done:
* From top menu, select 'Build' > 'Set Build Commands' > Add '-lwiringPi' 
* to "C++ commands" 1 and 2 (command row). Refer to image if unclear.
* 
* Compile with Terminal: Add command -lwiringPi (See bottom)
* 
* Might have to find simular steps if user wants to compile with VS Code 
* or other environment.
*/

//Global variables for interrupt handling
bool button_pushed{false};	/* will switch to true upon button push.
*  Will reset to false once associated button task has begun 
*/ 
volatile unsigned long last_interrupt{0}; //time of last interrupt (ms)
const int debounce_time {50}; /*time to ignore repeated interrupt
signals after last interrupt: 15 ms*/


//Interrupt routine
void button_interrupt(){
	if(millis() >= last_interrupt + debounce_time){
		last_interrupt = millis(); //update last_interrupt time
		button_pushed = true;
	}
}

void do_something(int LED_pin){
	//change current LED state
	digitalWrite(LED_pin, !digitalRead(LED_pin)); 
}

int main()
{
	const int LED_pin {0}; // LED_pin on GPIO.0, physical pin 11
	const int button_pin {1}; //button_pin on GPIO.1, physical pin 12

	wiringPiSetup();	/* Initiates library, connecting pins. 
	*	WiringPi's millis() function also starts counting here. 
	*/
	pinMode(LED_pin, OUTPUT);		// LED_pin as output
	pinMode(button_pin, INPUT);		// button_pin as input
	
	//Setting up interrupt service routine for button:
	wiringPiISR(button_pin, INT_EDGE_RISING, &button_interrupt);
	//Parameters: int wiringPiISR (int pin, int edgeType,  void (*function)(void))
	
	// Main program loop
	while(1){
		if(button_pushed == true){
			button_pushed = false; //reset button
			do_something(LED_pin);	
		}
	}
	return 0;
}

//g++ Button.cpp -Wall -Wextra -lwiringPi -o button


