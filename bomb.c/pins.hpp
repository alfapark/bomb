#pragma once

void read_pins(int n, const int * pins, int * inputs){
  for(int i = 0; i < n; ++i){
    inputs[i] = digitalRead(pins[i]);
  }
}

void copy_array(int n, const int * from, int * to){
  for(int i = 0; i < n; ++i){
    to[i] = from[i];
  }
}

bool are_same(int n, const int * a, const int * b){
  for(int i = 0; i < n; ++i){
    if(a[i] != b[i]){
      return false;
    }
  }
  return true;
}

void print_pins(int n, const int * pins){
  for(int i = 0; i < n; ++i){
    Serial.print(pins[i]);
  }
}

void set_kth_pin_low_only(int n, int k, const int * pins){
  for(int i = 0; i < n; ++i){
    digitalWrite(pins[i], i == k ? LOW : HIGH);
  }
}

void set_all_pins_to_value(int n, const int * pins, int value){
  for(int i = 0; i < n; ++i){
    digitalWrite(pins[i],value);
  }
}

void set_array_to_values(int n, int * arr, int value){
  for(int i = 0; i < n; ++i){
    arr[i] = value;
  }
}

void set_mode(int n, const int * pins, int mode){
  for(int i = 0; i < n; ++i){
      pinMode(pins[i], mode); 
  }
}
