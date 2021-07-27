#pragma once

#include "pins.hpp"

class Module{

public:
  Module(int success_led_pin, int fail_led_pin): 
  success_led_pin(success_led_pin), fail_led_pin(fail_led_pin), update_time(0), solved(false), failed(false), penalty(0), penalty_written_at(millis()){
    pinMode(success_led_pin, OUTPUT);  
    pinMode(fail_led_pin, OUTPUT);
  }
  virtual void run() = 0;

  unsigned long get_penalty() const;
  
  virtual bool is_solved() const;
  
  bool is_failed() const;

protected: 
  
  void set_penalty(unsigned long amount);
  void fail(unsigned long penalty_amount = 0);
  void success();
  void blank_state();
  void update_state();

  
  static const unsigned long debounce_interval = 100;
private: 

  void update_leds();
  
  int success_led_pin;
  int fail_led_pin;
  unsigned long update_time;
  bool solved = false;
  bool failed = false;

  unsigned long penalty;
  unsigned long penalty_written_at;
  
  static const unsigned long penalty_cooldown = 1000;
  static const unsigned long min_display_time = 500;
};

bool Module::is_solved(){
  return solved;
}

bool Module::is_failed(){
  return failed;
}

void Module::fail(unsigned long penalty_amount){
  set_penalty(penalty_amount);
  failed = true;
  solved = false;
  update_leds();
  update_time = millis();
}
void Module::success(){
  solved = true;
  failed = false;
  update_leds();
  update_time = millis();
}
void Module::blank_state(){ 
  solved = false;
  failed = false;
  update_state();
}

void Module::update_state(){
  if(update_time + min_display_time < millis()){
    update_leds();
  }
}

void Module::update_leds(){
  digitalWrite(success_led_pin, solved);
  digitalWrite(fail_led_pin, failed);
}

void Module::set_penalty(unsigned long amount){
  if(penalty_written_at + penalty_cooldown<= millis()){
    penalty += amount; 
    penalty_written_at = millis();
  }
}


unsigned long Module::get_penalty(){
  unsigned long tmp = penalty;
  penalty = 0;
  return tmp;
}
