#pragma once

class Module{

public:
  Module(int success_led_pin, int fail_led_pin): 
  success_led_pin(success_led_pin), fail_led_pin(fail_led_pin), update_time(0), solved(false), failed(false){
    pinMode(success_led_pin, OUTPUT);  
    pinMode(fail_led_pin, OUTPUT);
  }
  //virtual void run() = 0;

  bool is_solved();
  
  bool is_failed();

  void fail();
  void success();
  void blank_state();

private: 

  void update_leds();
  
  int success_led_pin;
  int fail_led_pin;
  unsigned long update_time;
  bool solved = false;
  bool failed = false;
  static const int min_display_time = 500;
};

bool Module::is_solved(){
  return solved;
}

bool Module::is_failed(){
  return failed;
}

void Module::fail(){
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
  if(update_time + min_display_time > millis()){
    digitalWrite(fail_led_pin, 0);
    digitalWrite(success_led_pin, 0);
  }
}

void Module::update_leds(){
  digitalWrite(success_led_pin, solved);
  digitalWrite(fail_led_pin, failed);
}
