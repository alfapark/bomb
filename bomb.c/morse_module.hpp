#pragma once

#include "module.hpp"

const int N_MORSE_BUTTONS = 4;

class MorseModule : public Module{

public:
  /*
   * win presses is sequence of index of the pins that have to be activated, one at the time
  */
  MorseModule(int success_led_pin, int fail_led_pin, int led_pin, int button_pins[N_MORSE_BUTTONS], int win_presses[], int n_win_presses): 
  Module(success_led_pin, fail_led_pin), debounced(0), n(n_win_presses), completed(0), held_button(-1), button_waiting_on_release(false){
    copy_pins(N_MORSE_BUTTONS, button_pins, pins_buttons);
    set_mode(N_MORSE_BUTTONS, pins_buttons, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);
    copy_pins(n_win_presses, win_presses, win_sequence);
  }

  virtual void run();

  
private: 
  unsigned long debounced;
  int pins_buttons[N_MORSE_BUTTONS];
  int win_sequence [20];
  int n;
  int completed;
  int held_button;
  bool button_waiting_on_release;
};

void MorseModule::run(){ 
  // reading input
  int inputs[N_MORSE_BUTTONS];
  read_pins(N_MORSE_BUTTONS, pins_buttons, inputs);
  for(int i = 0; i < N_MORSE_BUTTONS; ++i){
    if(inputs[i] == 0){
      if(held_button != -1 && held_button != i){
        fail();
        return;
      } else if(held_button == -1){
        debounced = millis();
        held_button = i;
      }
    }
  }
  // releasing the button
  if(button_waiting_on_release && inputs[held_button] == 1){
    held_button = -1;
    button_waiting_on_release = false;
    return;
  }
  if(debounced + debounce_interval > millis()){
    return;
  }
  // doing progress
  if(held_button == win_sequence[completed]){
    button_waiting_on_release = true;
    ++completed;
    if(completed == n){
      success();
    }
  }else{
    fail();  
  } 
}
