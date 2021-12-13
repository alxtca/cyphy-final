/* ---------------------------------------------------------------------------
* This code waits for signal from RP1 to begin measurements. When signal is received, buzzer  
* will beep to indicate it's time to get in position. Measurements start when the high pitch ends. 
* Led turns on, and arduino takes 20 samples with HC-RS04 ultrasonic sonar.
* When sampling complete, it calculates the average and send it back to Serial. LED turns off.
* 
* Includes NewPing.h by Tim Eckel https://www.arduino.cc/reference/en/libraries/newping/
* and TonePlayer by Nick Gammon https://forum.arduino.cc/t/newping-tone-conflict/179755/4 (Because
* arduino native tone() is in conflict with NewPing, both "compete" for timer2)
* ---------------------------------------------------------------------------
*/

#include <NewPing.h>
#include <TonePlayer.h> // because NewPing is in conflict with arduino tone()

#define trigger_pin  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define echo_pin     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define LED_pin 3  // start button
#define tone1 11
#define no_of_samples 20 //Number of samples per measuring session

volatile int samples[no_of_samples]; //array to store distance samples

NewPing sonar(trigger_pin, echo_pin); // Making NewPing sonar object
TonePlayer buzz(TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);  // pin D9 (Uno), D11 (Mega)

void setup() {
  Serial.begin(115200); // Open serial port at 230400 baud rate.
  pinMode(LED_pin, OUTPUT);
  pinMode(11, OUTPUT); //Buzzer pin
}

void loop() {
  //Wait for button to be pushed
  while (Serial.available() > 0){ //waiting for data, 0 means no data
    
    if(Serial.readString() == "start"){
      buzzDown(); // audible signal for user to get in position 
      digitalWrite(LED_pin, HIGH); //LED on, while measurements taken
      take_measurements();  //start measurement routine
    }
  }
}


void buzzDown(){
  int interval{850};
  float multiplier{0.8};
  unsigned int frequency{440}; // 440 Hz A4
  for(int i{0}; i < 2; i++){
    buzz.tone(frequency/2);
    delay(150);
    buzz.noTone();
    delay(850);
  }
 
  for(int i{0}; i < 8; i++){
    buzz.tone(frequency/2); //parameter: frequency
    delay(150*multiplier);
    buzz.noTone();
    interval *= multiplier; //decrease interval
    delay(interval);
    
  }
  
  for(int i {}; i < 4; i++){
    buzz.tone(frequency/2); //parameter: frequency
    delay(150*multiplier);
    buzz.noTone();
    delay(interval);
  }
  
  buzz.tone(2 * frequency); // 880 Hz A5
  delay(750);
  buzz.noTone(); 
}

void take_measurements(){
  //Fill array with measurement samples
  
  for(int i = 0 ; i < no_of_samples; i++){
    delay(50); // wait 50 ms between each sample to avoid cross reading
    samples[i] = sonar.ping_cm(); //ping_cm() returns int sample (distance in cm)
  }
  find_average(); 
}

void find_average(){
  float sum{0}; //sum as float makes division more accurate
  
  //sum of samples
  for(int i = 0 ; i < no_of_samples; i++){
    sum += samples[i];
  }

  
  int average{0}; // to store average distance
  average = round(sum/no_of_samples); //Round float division to nearest whole number. 

  
  //Print average to Serial
  //Serial.write("Average: ");
  Serial.print(average);
  //Serial.write(" cm \n");

  digitalWrite(LED_pin, LOW); //LED off, when measurement sent
}
