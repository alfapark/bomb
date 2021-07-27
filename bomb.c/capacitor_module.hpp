#pragma once

#include "module.hpp"

#include <Servo.h>

class CapacitorModule : public Module{

public:
  CapacitorModule(int success_led_pin, int fail_led_pin, int meter_pin, int button_pin, unsigned long seconds_before_start): 
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

  static const unsigned long loss_interval = 1000;
  static const unsigned long gain_interval = 100;
  static const unsigned long max_capacity = 120;
  static const unsigned long threshold = 15;

  unsigned long challenge_start;
  unsigned long grace_period;

  static const unsigned long capacitor_penalty = 5000;
};

void CapacitorModule::run(){ 
  if(challenge_start + grace_period >= millis()){
    success();
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
