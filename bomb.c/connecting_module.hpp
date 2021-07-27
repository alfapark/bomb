#pragma once

#include "module.hpp"

const int N_CONNECT_PINS = 4;

class ConnectingModule : public Module{

public:
ConnectingModule( int success_led_pin, int fail_led_pin, int input_pins[N_CONNECT_PINS],
    int output_pins[N_CONNECT_PINS], int win_mask[N_CONNECT_PINS][N_CONNECT_PINS]): 
  Module(success_led_pin, fail_led_pin), current_cycle(0), last_measurement(0){
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
  bool check_fail();

  int current_cycle;
  unsigned long last_measurement;
  unsigned long debounced[N_CONNECT_PINS]; //debounce for each cycle separately
  int pins_input[N_CONNECT_PINS];
  int pins_output[N_CONNECT_PINS];
  int last[N_CONNECT_PINS][N_CONNECT_PINS];
  int mask[N_CONNECT_PINS][N_CONNECT_PINS];

  static const unsigned long cycle_check_length = 10;
  static const unsigned long CONN_PENALTY = 120000;
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

bool ConnectingModule::check_fail(){
  // todo add fail when mask is 0 but last is 1
  for(int i = 0; i < N_CONNECT_PINS; ++i){
    for(int j = 0; j < N_CONNECT_PINS; ++j){
      if(last[i][j] == 0 && mask[i][j] == 1){
        return true;
      }
    }
  }
  return false;
}

void ConnectingModule::run(){ 
  if(last_measurement + cycle_check_length > millis()){
    return;
  }
  last_measurement = millis();
  
  //check then set for next one

  if(debounced[current_cycle] + debounce_interval < millis() ){
    int inputs[N_CONNECT_PINS];
    read_pins(N_CONNECT_PINS, pins_input, inputs);
    if(!are_same(N_CONNECT_PINS, inputs, last[current_cycle])){
      debounced[current_cycle] = millis();
      copy_pins(N_CONNECT_PINS, inputs, last[current_cycle]);
      Serial.print("Con ");
      Serial.print(current_cycle);
      Serial.print(": \t");
      print_pins(N_CONNECT_PINS, last[current_cycle]);
      Serial.println();
      if(check_win()){
        Serial.println("Success connections!");
        success();
      }else if(check_fail()){
        set_penalty(CONN_PENALTY);
        Serial.println("Fail connections!");
        fail();
      }else{
        blank_state();
      }
    } else if(!check_win() && !check_fail()) {
      blank_state();
    }
  }

  current_cycle = (current_cycle + 1) % N_CONNECT_PINS;  
  set_kth_pin_low_only(N_CONNECT_PINS, current_cycle, pins_output);
}
