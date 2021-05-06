#pragma once

#include "module.hpp"

const int N_DISPLAY_PINS = 8; //TODO

class DisplayModule : public Module{

// todo adopt other modules?
public:
  DisplayModule(int success_led_pin, int fail_led_pin, int diplay_pins[N_DISPLAY_PINS], int duration_in_seconds): 
  Module(success_led_pin, fail_led_pin), start(millis()), last_refresh(millis()), duration(duration_in_seconds){
     copy_pins(N_DISPLAY_PINS, diplay_pins, pins);
     set_mode(N_DISPLAY_PINS, pins, OUTPUT);
  }

  virtual void run();
  
private: 
  unsigned long start;
  unsigned long last_refresh;
  int duration;
  int pins[N_DISPLAY_PINS];
};

void DisplayModule::run(){ 
  if(start + duration < millis()){
    fail();
    // flicker with diplay
  } else if(last_refresh + 1000 < millis()){
    last_refresh = millis();
    //refresh display
  }
}
