#pragma once

#include "module.hpp"

const int N_CONNECT_PINS = 4;

class ConnectingModule : public Module{

public:
ConnectingModule( int success_pin, int fail_pin, int input_pins[N_CONNECT_PINS],
    int output_pins[N_CONNECT_PINS], int win_mask[N_CONNECT_PINS][N_CONNECT_PINS]): 
  Module(success_pin, fail_pin), current_cycle(0), last_measurement(0){
    for(int i = 0; i < N_CONNECT_PINS; ++i){
      pinMode(input_pins[i], INPUT_PULLUP); 
      pinMode(output_pins[i], OUTPUT); 
      set_all_pins_to_value(N_CONNECT_PINS, last[i], 0);
      copy_pins(N_CONNECT_PINS, win_mask[i], mask[i]);
      debounced[i] = 0;
    }
    copy_pins(N_CONNECT_PINS, input_pins, pins_input);
    copy_pins(N_CONNECT_PINS, output_pins, pins_output);
  }
  virtual void run();
  
private: 
  bool check_win();

  int current_cycle;
  unsigned long last_measurement;
  unsigned long debounced[N_CONNECT_PINS]; //debounce for each cycle separately
  int pins_input[N_CONNECT_PINS];
  int pins_output[N_CONNECT_PINS];
  int last[N_CONNECT_PINS][N_CONNECT_PINS];
  int mask[N_CONNECT_PINS][N_CONNECT_PINS];

  static const int cycle_check_length = 10;
};


bool ConnectingModule::check_win(){
  // todo add fail when mask is 0 but last is 1
  for(int i = 0; i < N_CONNECT_PINS; ++i){
   if(!are_same(N_CONNECT_PINS, last[i], mask[i])){
    return false;
   }
  }
  return true;
}

void ConnectingModule::run(){ 
  if(debounced[current_cycle] + debounce_interval > millis() ){
    current_cycle = (current_cycle + 1) % N_CONNECT_PINS;  
    return;
  }
  if(last_measurement + cycle_check_length > millis()){
    return;
  }
  last_measurement = millis();
  
  //check then set for next one
  
  int inputs[N_CONNECT_PINS];
  read_pins(N_CONNECT_PINS, pins_input, inputs);
  if(!are_same(N_CONNECT_PINS, inputs, last[current_cycle])){
    debounced[current_cycle] = millis();
    copy_pins(N_CONNECT_PINS, inputs, last[current_cycle]);
    Serial.print("Con ");
    Serial.print(current_cycle);
    Serial.print(": \t");
    print_pins(N_CONNECT_PINS, last[current_cycle]);
    if(check_win()){
      success();
    }else{
      blank_state();
    }
  }

  current_cycle = (current_cycle + 1) % N_CONNECT_PINS;  
  set_kth_pin_low_only(N_CONNECT_PINS, current_cycle, pins_output);
}
