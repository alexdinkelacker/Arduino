/*
Using: 
Arduino Mega 2560 
8 x 220 Ohm Resistors
SN74HC595N Shift Register
8x8 LCD Display

*/




int latchPin = 4;
int clockPin = 5; 
int dataPin = 2;
int pins[8] = {6, 7, 8, 9, 10, 11, 12, 13};
String input = "";
byte A[8] = {   B00000000, 
                B00011000,
                B00100100,
                B01000010,
                B01111110,
                B01000010,
                B01000010,
                B00000000
            };
byte B[8] = {   B00000000, 
                B01111000,
                B01000100,
                B01111100,
                B01000100,
                B01000100,
                B01111000,
                B00000000
            };
byte C[8] = {   B00000000, 
                B00111100,
                B01000000,
                B01000000,
                B01000000,
                B01000000,
                B00111100,
                B00000000
            };
byte D[8] = {   B00000000, 
                B01111000,
                B01000100,
                B01000100,
                B01000100,
                B01000100,
                B01111000,
                B00000000
            };    
byte E[8] = {   B00000000, 
                B01111100,
                B01000000,
                B01111000,
                B01000000,
                B01000000,
                B01111110,
                B00000000
            };
byte F[8] = {   B00000000, 
                B01111100,
                B01000000,
                B01111000,
                B01000000,
                B01000000,
                B01000000,
                B00000000
            };  
byte G[8] = {   B00000000, 
                B00111110,
                B01000000,
                B01000000,
                B01011110,
                B01000010,
                B00111100,
                B00000000
            };  
byte H[8] = {   B00000000, 
                B01000010,
                B01000010,
                B01111110,
                B01000010,
                B01000010,
                B01000010,
                B00000000
            };   
byte I[8] = {   B00000000, 
                B00111110,
                B00001000,
                B00001000,
                B00001000,
                B00001000,
                B00111110,
                B00000000
            };
byte J[8] = {   B00000000, 
                B01111110,
                B00001000,
                B00001000,
                B01001000,
                B01001000,
                B00110000,
                B00000000
            };
byte K[8] = {   B00000000, 
                B01000100,
                B01011000,
                B01100000,
                B01100000,
                B01011000,
                B01000100,
                B00000000
            };              
byte L[8] = {   B00000000, 
                B01000000,
                B01000000,
                B01000000,
                B01000000,
                B01000000,
                B01111110,
                B00000000
            };
byte M[8] = {   B00000000, 
                B01100011,
                B01010101,
                B01001001,
                B01000001,
                B01000001,
                B01000001,
                B00000000
            };
byte N[8] = {   B00000000, 
                B01000010,
                B01100010,
                B01010010,
                B01001010,
                B01000110,
                B01000010,
                B00000000
            };
byte O[8] = {   B00000000, 
                B00111100,
                B01000010,
                B01000010,
                B01000010,
                B01000010,
                B00111100,
                B00000000
            };
byte P[8] = {   B00000000, 
                B01111100,
                B01000010,
                B01111100,
                B01000000,
                B01000000,
                B01000000,
                B00000000
            };
byte Q[8] = {   B00000000, 
                B00111100,
                B01000010,
                B01000010,
                B01010010,
                B01010010,
                B00111100,
                B00000111
            };
byte R[8] = {   B01111100, 
                B01000010,
                B01000010,
                B01111100,
                B01000100,
                B01000010,
                B01000010,
                B00000000
            };
byte S[8] = {   B00000000, 
                B00111100,
                B01000010,
                B00111000,
                B00000100,
                B01000010,
                B00111100,
                B00000000
            };
byte T[8] = {   B00000000, 
                B01111111,
                B00001000,
                B00001000,
                B00001000,
                B00001000,
                B00001000,
                B00000000
            };
byte U[8] = {   B00000000, 
                B01000010,
                B01000010,
                B01000010,
                B01000010,
                B01000010,
                B00111100,
                B00000000
            }; 
byte V[8] = {   B00000000, 
                B01000100,
                B01000100,
                B01000100,
                B00101000,
                B00101000,
                B00010000,
                B00000000
            };
byte W[8] = {   B00000000, 
                B10000010,
                B10000010,
                B10000010,
                B10010010,
                B10101010,
                B01000100,
                B00000000
            };    
byte X[8] = {   B00000000, 
                B01000010,
                B00100100,
                B00011000,
                B00011000,
                B00100100,
                B01000010,
                B00000000
            }; 
byte Y[8] = {   B00000000, 
                B01000010,
                B00100100,
                B00011000,
                B00011000,
                B00011000,
                B00011000,
                B00000000
            };                
byte Z[8] = {   B00000000, 
                B01111110,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01111110,
                B00000000
            }; 
byte EX[8] = {  B00011000, 
                B00011000,
                B00011000,
                B00011000,
                B00011000,
                B00000000,
                B00011000,
                B00011000
            };       
byte HRT[8] = { B00000000, 
                B01101100,
                B11111110,
                B11111110,
                B11111110,
                B01111100,
                B00111000,
                B00010000
            };  
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600); 
  pinMode(latchPin, OUTPUT); 
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 
  for(int i = 0; i < 8; i++) {
    pinMode(pins[i],HIGH);
  }
 
}

// the loop routine runs over and over again forever:
void loop() {
  for (int k = 0; k < 1000; k++) {
    display_char(HRT);

  }
  
  for (int k = 0; k < 1000; k++) { // showing each letter for 1 second
    display_char(A);

  }

  for (int k = 0; k < 1000; k++) {
    display_char(R);

  }
  for (int k = 0; k < 1000; k++) {
    display_char(D);

  }
  for (int k = 0; k < 1000; k++) {
    display_char(U);

  }
  for (int k = 0; k < 1000; k++) { // showing each letter for 1 second
    display_char(I);

  }

  for (int k = 0; k < 1000; k++) {
    display_char(N);

  }

 
}

void display_char(byte ch[8]) { 
  for (int j = 0; j < 8; j++) {
    digitalWrite(latchPin, LOW);
    digitalWrite(pins[j], LOW);

    shiftOut(dataPin, clockPin, LSBFIRST, ch[j]);
    digitalWrite(latchPin, HIGH);
    //add delay() if you want to 
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(pins[j], HIGH);

  }


}
