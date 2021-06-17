#pragma once

#include "module.hpp"

const int N_MORSE_BUTTONS = 4;

class MorseModule : public Module{

public:
  /*
   * win presses is sequence of index of the pins that have to be activated, one at the time
   * the same button multiple times in win sequence might be skipped due to wrong release of it
  */
  MorseModule(int success_led_pin, int fail_led_pin, int led_pin, int morse_sequence[], int n_morse_length, int button_pins[N_MORSE_BUTTONS], int win_presses[], int n_win_presses): 
  Module(success_led_pin, fail_led_pin), debounced(0), n(n_win_presses), completed(0), held_button(-1), button_waiting_on_release(false),
  morse_led(led_pin), morse_length(n_morse_length), curent_morse_position(0), curent_morse_start(millis()){
    copy_pins(N_MORSE_BUTTONS, button_pins, pins_buttons);
    set_mode(N_MORSE_BUTTONS, pins_buttons, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);
    copy_pins(n_win_presses, win_presses, win_sequence);
    copy_pins(n_morse_length, morse_sequence, morse_code);
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

  int morse_led;
  int morse_code[100];
  int morse_length;
  int curent_morse_position;
  unsigned long curent_morse_start;

  static const unsigned long SHORT_SIGNAL = 200;
  static const unsigned long LONG_SIGNAL = 700;
  static const unsigned long WAIT = 300;
  void display_morse();
};

// TODO wait after word end
void MorseModule::display_morse(){
  unsigned long signal_time = (morse_code[curent_morse_position]) ? LONG_SIGNAL : SHORT_SIGNAL;
  if(curent_morse_start + WAIT <= millis() && millis() <= curent_morse_start + WAIT + signal_time){
    digitalWrite(morse_led, 1);
  } else if(curent_morse_start + WAIT + signal_time <= millis()){
    
    digitalWrite(morse_led, 0);
    curent_morse_position += 1;
    curent_morse_start = millis();
    if(curent_morse_position == morse_length){
      curent_morse_position = 0;
      curent_morse_start += WAIT;
    }
  }
}

void MorseModule::run(){ 
  display_morse();
  // reading input
  int inputs[N_MORSE_BUTTONS];
  read_pins(N_MORSE_BUTTONS, pins_buttons, inputs);
  for(int i = 0; i < N_MORSE_BUTTONS; ++i){
    if(inputs[i] == 0){
      if(held_button != -1 && held_button != i){
        fail();
        Serial.print("Mor: more than one pressed: ");
        print_pins(N_MORSE_BUTTONS, inputs);
        Serial.println();
        button_waiting_on_release = true;
        completed = 0;
        blank_state();
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
    if(completed < morse_length){
      blank_state();
    }
    debounced = millis();
    return;
  }else if(button_waiting_on_release){
    return;
  }
  
  if(debounced + debounce_interval > millis() || held_button == -1){
    if(completed < morse_length){
      blank_state();
    }
    return;
  }
  // doing progress
  if(held_button == win_sequence[completed]){
    button_waiting_on_release = true;
    ++completed;
    Serial.print("Mor: right button ");
    Serial.println(held_button);
    if(completed == n){
      success();
    }
  } else if(completed != 0 && held_button == win_sequence[completed-1]){
    //debounce from releasing, probably not able to solve easily
    return;
  }else{
    fail();
    completed = 0;
    button_waiting_on_release = true;
    Serial.print("Mor: wrong button ");
    Serial.println(held_button);
  } 
}
