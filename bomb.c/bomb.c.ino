#include "pins.hpp"

#include "capacitor_module.hpp"
#include "connecting_module.hpp"
#include "display_module.hpp"
#include "snipping_module.hpp"

//morse
int LED_M = 13;
int M1 = 14;
int M2 = 15;
int M3 = 16;
int M4 = 17;

Module * capacitor_module = NULL;
Module * connection_module = NULL;
Module * display_module = NULL;
Module * snipping_module = NULL;

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
}

unsigned long previousMillis_m = 0;
int interval_m = 500;
int i_m = 0;
void morse_diode(){
  if(previousMillis_m + interval_m < millis()){
    digitalWrite(LED_M, i_m);
    previousMillis_m = millis();
    i_m = i_m == 0;
  }
}

int last_inputs_morse[4];
void morse(){
  morse_diode();
  int pins[4] = {M1, M2, M3, M4};
  int inputs[4];
  read_pins(4, pins, inputs);
  if(!are_same(4, inputs, last_inputs_morse)){
    Serial.print("Morse: \t");
    print_pins(4, inputs);
    copy_pins(4, inputs, last_inputs_morse);
    Serial.println();
  }
}

void loop() {
   snipping_module->run();
   capacitor_module->run();
   display_module->run();
   connection_module->run();
}
