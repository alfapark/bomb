#include "pins.hpp"

#include "capacitor_module.hpp"
#include "connecting_module.hpp"
#include "display_module.hpp"
#include "morse_module.hpp"
#include "snipping_module.hpp"
#include "switch_module.hpp"

//morse
int LED_M = 13;
int M1 = 14;
int M2 = 15;
int M3 = 16;
int M4 = 17;

Module * capacitor_module = NULL;
Module * connection_module = NULL;
Module * display_module = NULL;
Module * morse_module = NULL;
Module * snipping_module = NULL;
Module * switch_module = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int pins[5] = {A0, A1, A2, A3, A4};
  int mask[5] = {1, 0, 0, 1, 0};
  snipping_module = new SnippingModule(2, 3, pins, mask);

  //morse
  pinMode(LED_M, OUTPUT);
  pinMode(M1, INPUT_PULLUP);
  pinMode(M2, INPUT_PULLUP);
  pinMode(M3, INPUT_PULLUP);
  pinMode(M4, INPUT_PULLUP);

  capacitor_module = new CapacitorModule(4,5, 53,51);
  int input_pins[4] = {42, 44, 46, 48};
  int output_pins[4] = {43, 45, 47, 49};
  int win_mask[4][4] = {
    {1, 1, 1, 1},
    {1, 1, 0, 1},
    {1, 1, 1, 1},
    {0, 1, 1, 1},
  };
  connection_module = new ConnectingModule(8,9, input_pins, output_pins, win_mask);
  display_module = new DisplayModule(6,7, 52, 50, 20);
  int switches[6] = {27,29, 31, 33, 35, 37};
  int leds[6] = {26, 28, 30, 32, 34, 36};
  int switches_to_leds[6][6] = {
    {0,1,0,1,0,1},
    {0,0,1,1,1,0},
    {1,1,1,0,0,0},
    {0,0,0,0,1,1},
    {0,1,0,0,0,1},
    {1,0,1,0,0,0}
  };
  switch_module = new SwitchModule(10, 11, switches, leds, switches_to_leds);

  int morse_sequence[] = {0,1,1,0,1};
  int button_pins[] = {A8, A9, A10, A11};
  int win_sequence[] = {0,1,2,3};
  morse_module = new MorseModule(12, 13, A12, morse_sequence, sizeof(morse_sequence), button_pins, win_sequence, sizeof(win_sequence));
}

void loop() {
   snipping_module->run();
   capacitor_module->run();
   display_module->run();
   connection_module->run();
   switch_module->run();
   morse_module->run();
}
