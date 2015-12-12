#include <BH1750FVI.h>
#include <Wire.h>
#include <RBL_services.h>
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>

BH1750FVI LightSensor;

#define forward 3  // Pin 3 - Forward
#define backward 2 	// Pin 2 - Backward
#define right 4 //Pin 4 - Right
#define left 5 //Pin 5 - Left
#define trigPin 9 //sensor
#define echoPin 6 //sensor

void setup()
{
  //  
  // For BLE Shield and Blend:
  //   Default pins set to 9 and 8 for REQN and RDYN
  //   Set your REQN and RDYN here before ble_begin() if you need
  //
  
  //ble_set_pins(3, 2);
  
  ble_set_name("My RC Car"); // The name have to be under 10 letters

  Serial.begin(9600);
  LightSensor.begin();

  LightSensor.SetAddress(Device_Address_H);//Address 0x5C
  LightSensor.SetMode(Continuous_H_resolution_Mode);
   
  pinMode(echoPin,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);

  ble_begin();
}

unsigned char val;
unsigned int loop_count = 0;
int light_sen;

void go_forward() {
  digitalWrite(forward, HIGH);
}

void go_backward(){
  digitalWrite(backward, HIGH);
}

void go_right(){
  digitalWrite(right, HIGH);
}

void go_left(){
  digitalWrite(left, HIGH);
}

void stop_right() {
  digitalWrite(right, LOW);
}

void stop_left() {
  digitalWrite(left, LOW);
}

void stop_forward() {
  digitalWrite(forward, LOW);
}

void stop_backward() {
  digitalWrite(backward, LOW);
}

void distance_sensor() {
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000); //pauses the program for 1 millisecond
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print('\n');
  //ble_write(distance); 
}

void light_sensor(){
  uint16_t light_sen_lux = LightSensor.GetLightIntensity();// Get Lux value
  Serial.print("Light: ");
  Serial.print(light_sen_lux);
  Serial.print(" lux");
 // ble_write(light_sen_lux);
}

void loop()
{
  
  if (ble_available())
  {
    val = ble_read(); //receives data from android device
    //ble_write(val); //sends data to android device
    if(val == 'f'){
      go_forward();
    }else if(val == 'b'){
      go_backward();
    }else if(val == 'r'){
      go_right();
    }else if(val == 'l'){
      go_left();
    }else if(val == 'k'){
      stop_forward();
    }else if(val == 'j'){
      stop_right();
    }else if(val == 'h'){
      stop_left();
    }else if(val == 'g'){
      stop_backward();
    } 
  }
  
   // loop_count+=1;
  //Serial.print("loop_counter: ");
  //Serial.print(loop_count);
  //Serial.print(" \n");
  
  ble_do_events();
  
  distance_sensor();
  light_sensor();
}
