#include <TM1637Display.h>

#define CLK 4
#define DIO 5
#define HT 60
#define RT 90

TM1637Display display(CLK, DIO);

int coPin = A0;  //Carbon Monoxide
int foPin = A1;  //Flamable and Smoke
int ledPin = 13;
int pin = 9;
int buzzerPin = 10;
int coValue = -999;
int foValue = -998;
int coMean = 0;
int foMean = 0;
int h = 0;
int r = 0;
int st = 0;

void beep(int delayms) {
  analogWrite(buzzerPin, 0);
  delay(delayms);
  analogWrite(buzzerPin, 255); 
}

void setup() {
  analogReference(EXTERNAL);
  pinMode(ledPin, OUTPUT);
  pinMode(pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(coPin, INPUT);
  pinMode(foPin, INPUT);
  Serial.begin(9600);
  display.setBrightness(0x0f);
  beep(50);
}

void loop() {
  if(h < HT){
    Serial.print("Heating 60s...");
    Serial.println(h);
    analogWrite(ledPin, 255);
    analogWrite(pin, 255);
    h++;  
  }else if (h >= HT && r < RT){
    Serial.print("Reading 90s...");
    Serial.println(r);
    analogWrite(ledPin, 79);
    analogWrite(pin, 79);
    r++;
  }else if (h >= HT && r >= RT){
    Serial.println("Collecting 100ms...");
    analogWrite(pin, 255);
    delay(100);
    coValue = map(analogRead(coPin), 0, 1023, 10, 10000);
    foValue = map(analogRead(foPin), 0, 1023, 300, 10000);
    if(coMean == foMean){
      coMean = coValue;
      foMean = foValue;
    }
    coMean = (coMean + coValue)/2;
    foMean = (foMean + foValue)/2;
    r = 0;
    h = 0;
    beep(10);
  }
  Serial.print("CO = ");
  Serial.print(coValue);
  Serial.print(" FO = ");
  Serial.println(foValue);
  if((coValue - coMean) >  (coMean /2) ){
    beep(500); 
  }
  if((foValue - foMean) >  (foMean /2) ){
    beep(500);
  }
  
  if(((h+r)%5) == 0){
    if(st == 0){
      display.showNumberDec(coValue, false);
      st++;
    }else if(st == 1){
      display.showNumberDec(foValue, false);
      st++;
    }else{
      display.showNumberDec(h+r, true);
      st = 0;
    }
  }
  delay(1000);
}
