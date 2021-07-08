#pragma once

#include "module.hpp"

#include <Arduino.h>
#include <TM1637Display.h>

class DisplayModule : public Module{

// todo adopt other modules?
public:
  DisplayModule(int success_led_pin, int fail_led_pin, int clk_pin, int dio_pin, unsigned long duration_in_seconds): 
  Module(success_led_pin, fail_led_pin), start(millis()), last_refresh(millis()), duration(duration_in_seconds*1000), segment_display(clk_pin, dio_pin){
    segment_display.setBrightness(0x0f);
  }

  virtual void run();

  void penalize(unsigned long amount);
  
private: 
  unsigned long start;
  unsigned long last_refresh;
  unsigned long duration;
  TM1637Display segment_display;

  void refresh_display();
};

void DisplayModule::refresh_display(){
  last_refresh = millis();
  uint8_t data[4];
  unsigned long remaining = (start + duration - millis())/1000;
  int to_display = remaining % 60 + (remaining/60)*100;
  int show_colon = remaining %2 == 1 ? 0x40 :0x80;
  segment_display.showNumberDecEx(to_display,  show_colon, true, 4, 0);
  segment_display.setSegments(data);
}

void DisplayModule::run(){ 
  if(start + duration < millis()){
    fail();
    const uint8_t SEG_BUCH[] = {
      SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
      SEG_C | SEG_D | SEG_E,
      SEG_D | SEG_E | SEG_G, 
      SEG_C | SEG_E | SEG_F | SEG_G
    };
    segment_display.setSegments(SEG_BUCH);
  } else if(last_refresh + 1000 < millis()){
    refresh_display();
  }
}

void DisplayModule::penalize(unsigned long amount){
  duration -= amount;
  refresh_display();
}
