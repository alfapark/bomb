#pragma once

#include "module.hpp"

#include <Servo.h>

class CapacitorModule : public Module{

public:
  CapacitorModule(int success_led_pin, int fail_led_pin, int meter_pin, int button_pin, int seconds_before_start): 
  Module(success_led_pin, fail_led_pin), last_change_loss(0), last_change_gain(0), pin_meter(meter_pin), pin_button(button_pin), capacity(max_capacity-threshold-1),
  challenge_start(millis()), grace_period(seconds_before_start*1000){
    pinMode(button_pin, INPUT_PULLUP);
    myservo.attach(meter_pin);
  }

  virtual void run();
  
private: 
  Servo myservo;

  unsigned long last_change_loss;
  unsigned long last_change_gain;
  int pin_meter;
  int pin_button;
  int capacity;

  static const int loss_interval = 200;
  static const int gain_interval = 50;
  static const int max_capacity = 120;
  static const int threshold = 15;

  unsigned long challenge_start;
  int grace_period;

  static const int capacitor_penalty = 2000;
};

void CapacitorModule::run(){ 
  if(challenge_start + grace_period >= millis()){
    return;
  }
  
  if(last_change_loss + loss_interval < millis()){
    last_change_loss = millis();
    --capacity;
    if (capacity < 0){
      capacity = 0;
    }
  }

  if(last_change_gain +gain_interval < millis()){
    last_change_gain = millis();
    if (digitalRead(pin_button) == 0){
      ++capacity; //Tlacitko na dobijeni casu
    }
    if (capacity > max_capacity){
      capacity = max_capacity;
    }
    myservo.write(capacity);
  }
  

  
  bool inside_threshold = capacity < threshold || max_capacity -threshold < capacity;
  if(inside_threshold){
    fail();
    set_penalty(capacitor_penalty);
  }else{
    success();
  }
}
