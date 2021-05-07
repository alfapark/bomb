#pragma once

#include "module.hpp"

class CapacitorModule : public Module{

public:
  CapacitorModule(int success_led_pin, int fail_led_pin, int meter_pin, int button_pin): 
  Module(success_led_pin, fail_led_pin), last_change(0), pin_meter(meter_pin), pin_button(button_pin), capacity(max_capacity){
  }

  virtual void run();
  
private: 
  unsigned long last_change;
  int pin_meter;
  int pin_button;
  int capacity;

  static const int change_interval = 10;
  static const int max_capacity = 255;
  static const int threshold = 25;
};

void CapacitorModule::run(){ 
  if(last_change + change_interval > millis()){
    return;
  }

  last_change = millis();
  --capacity;
  if (capacity < 0)capacity = 0;
  if (digitalRead(pin_button) == 0){
    capacity = capacity + 10; //Tlacitko na dobijeni casu
  }
  if (capacity > max_capacity){
    capacity = max_capacity;
  }
  analogWrite(pin_meter, capacity);//Output pin - merak
  bool inside_threshold = capacity < threshold || max_capacity -threshold < capacity;
  if(inside_threshold){
    fail();
  }else{
    success();
  }
}
