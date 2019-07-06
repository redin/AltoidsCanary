#include <TM1637Display.h>

#define CLK 4
#define DIO 5
#define HT 60
#define RT 90


TM1637Display display(CLK, DIO);

int coPin = A0;  //Carbon Monoxide
int foPin = A1;  //Flamable and Smoke
int toPin = A2;  //Temp
int ledPin = 13;
int pin = 9;
int pin2 = 10;
int coValue = -999;
int foValue = -998;
int toValue = -997;
int h = 0;
int r = 0;
int st = 0;


void setup() {
  analogReference(EXTERNAL);
  pinMode(ledPin, OUTPUT);
  pinMode(pin, OUTPUT);
  pinMode(coPin, INPUT);
  pinMode(foPin, INPUT);
  pinMode(toPin, INPUT);
  Serial.begin(9600);
  display.setBrightness(0x0f);
}

void loop() {
  if(h < HT){
    Serial.print("Heating 60s...");
    Serial.println(h);
    analogWrite(ledPin, 255);
    analogWrite(pin, 255);
    analogWrite(pin2, 255);
    h++;  
  }else if (h >= HT && r < RT){
    Serial.print("Reading 90s...");
    Serial.println(r);
    analogWrite(ledPin, 79);
    analogWrite(pin, 79);
    analogWrite(pin2, 79);
    r++;
  }else if (h >= HT && r >= RT){
    Serial.println("Collecting 150ms...");
    analogWrite(pin, 255);
    analogWrite(pin2, 255);
    delay(150);
    coValue = map(analogRead(coPin), 0, 1023, 10, 10000);
    foValue = map(analogRead(foPin), 0, 1023, 10, 10000);
    //toValue = analogRead(toPin);
    r = 0;
    h = 0;  
  }
  Serial.print("CO = ");
  Serial.print(coValue);
  Serial.print(" FO = ");
  Serial.println(foValue);
  Serial.print(" TO = ");
  Serial.print(((h+r)%5));
  Serial.print(" T1 = ");
  Serial.println(st);
  if(((h+r)%5) == 0){
    if(st == 0){
      Serial.println("s0");
      display.showNumberDec(coValue, false);
      st++;
    }else if(st == 1){
      Serial.println("s1");
      display.showNumberDec(foValue, false);
      st++;
    }else{
      Serial.println("s2");
      display.showNumberDec(h+r, true);
      st = 0;
    }
  }
  delay(1000);
}
