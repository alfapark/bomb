#pragma once

#include "module.hpp"

const int N_SWITCH_PINS = 6;

class SwitchModule : public Module{

public:
  SwitchModule(int success_led_pin, int fail_led_pin, int switch_pins[N_SWITCH_PINS], int led_pins[N_SWITCH_PINS], int switch_to_leds[N_SWITCH_PINS][N_SWITCH_PINS]): 
  Module(success_led_pin, fail_led_pin){
    copy_array(N_SWITCH_PINS, switch_pins, pins_switches);
    copy_array(N_SWITCH_PINS, led_pins, pins_led);
    for(int i = 0; i < N_SWITCH_PINS; ++i){
      copy_array(N_SWITCH_PINS, switch_to_leds[i], switch_leds[i]);
    }
    set_mode(N_SWITCH_PINS, led_pins, OUTPUT);
    set_mode(N_SWITCH_PINS, switch_pins, INPUT_PULLUP);
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

  int switches_inputs[N_SWITCH_PINS];
  read_pins(N_SWITCH_PINS, pins_switches, switches_inputs);
  for(int i = 0; i < N_SWITCH_PINS; ++i){
    if(switches_inputs[i] == 0){
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
  int all_turned_off[N_SWITCH_PINS] = {1,1,1,1,1,1};
  
  if(solved_flag){
    success();
  }else if(are_same(N_SWITCH_PINS, switches_inputs, all_turned_off)){
    blank_state();
  } else{
    fail();
  }
}
