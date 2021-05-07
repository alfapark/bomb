#include "pins.hpp"

#include "capacitor_module.hpp"
#include "connecting_module.hpp"
#include "display_module.hpp"
#include "snipping_module.hpp"

//snipping cables


//connect pins
int CPO1 = 2;
int CPO2 = 3;
int CPO3 = 4;
int CPO4 = 5;

int CPI1 = 6;
int CPI2 = 7;
int CPI3 = 8;
int CPI4 = 9;

//morse
int LED_M = 13;
int M1 = 14;
int M2 = 15;
int M3 = 16;
int M4 = 17;

// trololo
int Tmeter = 10;
int TButton = 11;

Module * snipping_module = NULL;
Module * capacitor_module = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  int pins[5] = {A0, A1, A2, A3, A4};
  int mask[5] = {1, 0, 0, 1, 0};
  snipping_module = new SnippingModule(2, 3, pins, mask);

  // connect pins
  pinMode(CPO1, OUTPUT);
  pinMode(CPO2, OUTPUT);
  pinMode(CPO3, OUTPUT);
  pinMode(CPO4, OUTPUT);
  
  pinMode(CPI1, INPUT_PULLUP);
  pinMode(CPI2, INPUT_PULLUP);
  pinMode(CPI3, INPUT_PULLUP);
  pinMode(CPI4, INPUT_PULLUP);

  //morse
  pinMode(LED_M, OUTPUT);
  pinMode(M1, INPUT_PULLUP);
  pinMode(M2, INPUT_PULLUP);
  pinMode(M3, INPUT_PULLUP);
  pinMode(M4, INPUT_PULLUP);

  //trololo
  pinMode(TButton, INPUT_PULLUP);
  pinMode(Tmeter, OUTPUT);
  capacitor_module = new CapacitorModule(4,5, 53,51);
}

int last_inputs_s[5] = {0};
void check_snipping(){
  int pins[5] = {A0, A1, A2, A3, A4}; 
  int inputs[5]; 
  read_pins(5, pins, inputs);
  if(!are_same(5, last_inputs_s, inputs)){
    Serial.print("Snipp: \t");
    print_pins(5, inputs);
    Serial.println();
    copy_pins(5, inputs, last_inputs_s);
  }
}

unsigned long previousMillis_cp = 0;        // will store last time LED was updated
const long interval_cp = 10;           // interval at which to blink (milliseconds)
int i_cp = 0;
int reads_cp[4][4] = {0};
void print_conns(){
  Serial.print("conns: \t");
  Serial.println();
  for(int i = 0; i < 4; ++i){
    Serial.print(i);
    Serial.print(": ");
    for(int j = 0; j < 4; ++j){
      Serial.print(reads_cp[i][j]);
    }
  Serial.println();
  }
}

void connect_pins(){
  int output_pins[4] = {CPO1, CPO2, CPO3, CPO4};
  if (millis() - previousMillis_cp >= interval_cp) {
    previousMillis_cp = millis();
    i_cp = (i_cp + 1) % 4;  
    set_kth_pin_low_only(4, i_cp, output_pins);
  }
  int input_pins[4] = {CPI1, CPI2, CPI3, CPI4};
  int inputs[4];
  read_pins(4, input_pins, inputs);
  if(!are_same(4, inputs, reads_cp[i_cp])){
    copy_pins(4, inputs, reads_cp[i_cp]);
    print_conns();
  }
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

unsigned long last_change = 0;
int interval = 100;
int i = 0;
void trololo(){
  if(last_change + interval < millis()){
    last_change = millis();
    i--;
    if (i < 0)i = 0;
    if (digitalRead(TButton) == 0){
      i = i + 10; //Tlacitko na dobijeni casu
    }
    if (i > 255){
      i = 255;
    }
    analogWrite(Tmeter, i);//Output pin - merak
  }
}

void loop() {
  // put your main code here, to run repeatedly:
// check_snipping();
// connect_pins();
// morse();
// trololo();
   snipping_module->run();
   capacitor_module->run();
}
