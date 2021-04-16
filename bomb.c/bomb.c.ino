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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);

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
}

void read_pins(int n, int * pins, int * outputs){
  for(int i = 0; i < n; ++i){
    outputs[i] = digitalRead(pins[i]);
  }
}

void copy(int n, int * from, int * to){
  for(int i = 0; i <n; ++i){
    to[i] = from[i];
  }
}

bool are_same(int n, int * a, int * b){
  for(int i = 0; i < n; ++i){
    if(a[i] != b[i]){
      return false;
    }
  }
  return true;
}

void print_pins(int n, int * pins){
  for(int i = 0; i < n; ++i){
    Serial.print(pins[i]);
  }
}

int last_outputs_s[5] = {0};
void check_snipping(){
  int pins[5] = {A0, A1, A2, A3, A4}; 
  int outputs[5]; 
  read_pins(5, pins, outputs);
  if(!are_same(5, last_outputs_s, outputs)){
    Serial.print("Snipp: \t");
    print_pins(5, outputs);
    Serial.println();
    copy(5, outputs, last_outputs_s);
  }
}

unsigned long previousMillis_cp = 0;        // will store last time LED was updated
const long interval_cp = 10;           // interval at which to blink (milliseconds)
int i_cp = 0;
unsigned long last_read_cp = 0;
unsigned long last_read_interval_cp = 500;
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
  if (millis() - previousMillis_cp >= interval_cp) {
    previousMillis_cp = millis();

    i_cp++;
    if (i_cp == 4)i_cp = 0;

    switch (i_cp) {
      case 0:
        digitalWrite(CPO1, LOW);
        digitalWrite(CPO2, HIGH);
        digitalWrite(CPO3, HIGH);
        digitalWrite(CPO4, HIGH);
 
        break;
      case 1:
        digitalWrite(CPO1, HIGH);
        digitalWrite(CPO2, LOW);
        digitalWrite(CPO3, HIGH);
        digitalWrite(CPO4, HIGH);
        break;
      case 2:
        digitalWrite(CPO1, HIGH);
        digitalWrite(CPO2, HIGH);
        digitalWrite(CPO3, LOW);
        digitalWrite(CPO4, HIGH);
        break;
      case 3:
        digitalWrite(CPO1, HIGH);
        digitalWrite(CPO2, HIGH);
        digitalWrite(CPO3, HIGH);
        digitalWrite(CPO4, LOW);
        break;
    }
  }
  
  reads_cp[i_cp][0] = digitalRead(CPI1);
  reads_cp[i_cp][1] = digitalRead(CPI2);
  reads_cp[i_cp][2] = digitalRead(CPI3);
  reads_cp[i_cp][3] = digitalRead(CPI4);

  if(last_read_cp + last_read_interval_cp < millis()){
    last_read_cp = millis();
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

unsigned long last_read_m = 0;
unsigned long last_read_interval_m = 500;

void morse(){
  morse_diode();
  if(last_read_m + last_read_interval_m < millis()){
    last_read_m = millis();
    int d1 = digitalRead(M1);
    int d2 = digitalRead(M2);
    int d3 = digitalRead(M3);
    int d4 = digitalRead(M4);
  
    Serial.print("Morse: \t");
    Serial.print(d1);
    Serial.print(d2);
    Serial.print(d3);
    Serial.print(d4);
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 check_snipping();
 connect_pins();
 morse();
}
