#pragma once

#include "module.hpp"

const int N_SWITCH_PINS = 6;

class SwitchModule : public Module{

public:
  SwitchModule(int success_led_pin, int fail_led_pin, int switch_pins[N_SWITCH_PINS], int led_pins[N_SWITCH_PINS], int switch_to_leds[N_SWITCH_PINS][N_SWITCH_PINS]): 
  Module(success_led_pin, fail_led_pin){
    copy_pins(N_SWITCH_PINS, switch_pins, pins_switches);
    copy_pins(N_SWITCH_PINS, led_pins, pins_led);
    for(int i = 0; i < N_SWITCH_PINS; ++i){
      copy_pins(N_SWITCH_PINS, switch_to_leds[i], switch_leds[i]);
      pinMode(led_pins[i], OUTPUT);
      pinMode(switch_pins[i], INPUT_PULLUP);
    }
  } 

  virtual void run();
  
private: 
  int pins_switches[N_SWITCH_PINS];
  int pins_led[N_SWITCH_PINS];
  int switch_leds[N_SWITCH_PINS][N_SWITCH_PINS];
  
};

void SwitchModule::run(){ 
  int to_display[N_SWITCH_PINS];
  set_array_to_values(N_SWITCH_PINS, to_display, 0);

  for(int i = 0; i < N_SWITCH_PINS; ++i){
    if(digitalRead(pins_switches[i]) == 0){
      for(int j = 0; j < N_SWITCH_PINS; ++j){
        to_display[j] = to_display[j] ^ switch_leds[i][j];
      }
    }
  }
  bool solved_flag = true;
  for(int i = 0; i < N_SWITCH_PINS; ++i){
    digitalWrite(pins_led[i], to_display[i]);
    solved_flag = solved_flag && to_display[i];
  }
  if(solved_flag){
    success();
  }else{
    fail();
  }
}
