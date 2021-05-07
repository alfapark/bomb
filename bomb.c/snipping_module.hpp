#pragma once

#include "module.hpp"

const int N_SNIPPING_PINS = 5;

class SnippingModule : public Module{

public:
  SnippingModule(int success_led_pin, int fail_led_pin, int wire_pins[N_SNIPPING_PINS], int win_mask[N_SNIPPING_PINS]): 
  Module(success_led_pin, fail_led_pin), debounced(0){
    for(int i = 0; i < N_SNIPPING_PINS; ++i){
      pinMode(wire_pins[i], INPUT_PULLUP); 
    }
    copy_pins(N_SNIPPING_PINS, wire_pins, pins);
    copy_pins(N_SNIPPING_PINS, win_mask, mask);
    set_all_pins_to_value(N_SNIPPING_PINS, last, 1);
  }

  virtual void run();
  
private: 
  unsigned long debounced;
  int pins[N_SNIPPING_PINS];
  int mask[N_SNIPPING_PINS];
  int last[N_SNIPPING_PINS];
};

void SnippingModule::run(){ 
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
      return;
    }
    for(int i = 0; i < N_SNIPPING_PINS; ++i){
      if(mask[i] == 0 && last[i] == 1){
        Serial.println("Snipp: \tfail!");
        fail();
        return;
      }
    }
  Serial.println("Snipp: \tblank!");
  blank_state();
  }
  update_state();
}
