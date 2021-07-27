#pragma once

#include "module.hpp"

const int N_SNIPPING_PINS = 5;

class SnippingModule : public Module{

public:
  SnippingModule(int success_led_pin, int fail_led_pin, int wire_pins[N_SNIPPING_PINS], int win_mask[N_SNIPPING_PINS]): 
  Module(success_led_pin, fail_led_pin), debounced(0), solved(false){
    set_mode(N_SNIPPING_PINS, wire_pins, INPUT_PULLUP);
    copy_pins(N_SNIPPING_PINS, wire_pins, pins);
    copy_pins(N_SNIPPING_PINS, win_mask, mask);
    set_array_to_values(N_SNIPPING_PINS, last, 0);
  }

  virtual void run();
  virtual bool is_solved() const;
  
private: 
  unsigned long debounced;
  int pins[N_SNIPPING_PINS];
  int mask[N_SNIPPING_PINS];
  int last[N_SNIPPING_PINS];

  bool solved;

  const static unsigned long SNIPP_PENALTY = 1200000 ; // 20*60*1000
};

void SnippingModule::run(){ 
  if(solved){
    return;
  }
  if(debounced + debounce_interval > millis()){
    return;
  }
  int inputs[N_SNIPPING_PINS]; 
  read_pins(N_SNIPPING_PINS, pins, inputs);
  if(!are_same(N_SNIPPING_PINS, last, inputs)){
    debounced = millis();
    Serial.print("Snipp: \t");
    print_pins(N_SNIPPING_PINS, inputs);
    Serial.println();
    copy_pins(N_SNIPPING_PINS, inputs, last);
    
    if(are_same(N_SNIPPING_PINS, inputs, mask)){
      Serial.println("Snipp: \tsuccess!");
      success();
      solved = true;
      return;
    }
    for(int i = 0; i < N_SNIPPING_PINS; ++i){
      if(mask[i] == 0 && last[i] == 1){
        Serial.println("Snipp: \tfail!");
        fail(SNIPP_PENALTY);
        solved = true;
        return;
      }
    }
    Serial.println("Snipp: \tblank!");
    blank_state();
  }
  update_state();
}


bool SnippingModule::is_solved() const{
  return solved;
}
