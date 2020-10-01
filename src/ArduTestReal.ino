#include <Time.h>
#include "Arduino.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Helper.h"
#include "Holder.h"
#include "StateMachine.h"


const float VERSION = 1.06;



const Holder holder;
const StateMachine statemachine;


Adafruit_7segment matrix = Adafruit_7segment();

int movingAvg(int *ptrArrNumbers, long *ptrSum, int pos, int len, int nextNum)
{
  //Subtract the oldest number from the prev sum, add the new number
  *ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
  //Assign the nextNum to the position in the array
  ptrArrNumbers[pos] = nextNum;
  //return the average
  return *ptrSum / len;
}


int panel_arrNumbers[5] = {0};
int panel_pos = 0;
int panel_newAvg = 0;
long panel_sum = 0;
int panel_len = sizeof(panel_arrNumbers) / sizeof(int);
int getPanelValue(int port) {
  int i = (analogRead(port) * -1) + 1000;
  i /= 10;
  int newAvg = movingAvg(panel_arrNumbers, &panel_sum, panel_pos, panel_len, i);
  panel_pos++;
  if (panel_pos >= panel_len) {
    panel_pos = 0;
  }
  return newAvg;
}


int pot1_arrNumbers[20] = {0};
int pot1_pos = 0;
int pot1_newAvg = 0;
long pot1_sum = 0;
int pot1_len = sizeof(pot1_arrNumbers) / sizeof(int);
int getPot1Value(int port) {
  int i = analogRead(port);
  int newAvg = movingAvg(pot1_arrNumbers, &pot1_sum, pot1_pos, pot1_len, i);
  pot1_pos++;
  if (pot1_pos >= pot1_len) {
    pot1_pos = 0;
  }
  return newAvg / 10.24;
}


int pot2_arrNumbers[20] = {0};
int pot2_pos = 0;
int pot2_newAvg = 0;
long pot2_sum = 0;
int pot2_len = sizeof(pot1_arrNumbers) / sizeof(int);
int getPot2Value(int port) {
  int i = analogRead(port);
  int newAvg = movingAvg(pot2_arrNumbers, &pot2_sum, pot2_pos, pot2_len, i);
  pot2_pos++;
  if (pot2_pos >= pot2_len) {
    pot2_pos = 0;
  }
  int j =  newAvg / 170.67; //modi 0..5
  return j + 1; //modi 1..6
}




void setup() {
  matrix.begin(0x70);  // pass in the address
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);

  //init Serial port
  Serial.begin(9600);
  Serial.print("Version");
  Serial.print(VERSION);
  Serial.println();

  //print version number + LED-Test
  matrix.print(VERSION);
  matrix.drawColon(true);
  matrix.writeDisplay();

  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(8, HIGH);
  delay(500);


  //init holder.
  holder.setUsageMode(getPot2Value(1));
  holder.setSensitivity(getPot1Value(0));
  holder.setLightAmount(getPanelValue(7));
  holder.setCurrentMillis(millis());
  holder.setTimer(holder.getCurrentMillis() / 1000);


  //turn off version and other stuff
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);

  matrix.clear();
  matrix.drawColon(false);
  matrix.writeDisplay();
}

Display prevDisplay;
void loop() {
  Serial.println();
  //read values from hardware
  holder.setUsageMode(getPot2Value(1));
  holder.setSensitivity(getPot1Value(0));
  holder.setLightAmount(getPanelValue(7));
  holder.setCurrentMillis(millis());
  holder.setTimer(holder.getCurrentMillis() / 1000);

  //execute the statemachine
  statemachine.run(holder);
  //Serial.print("....");

  //write display values to hardware
  digitalWrite(4, holder.getLED_Status_Red());
  digitalWrite(6, holder.getLED_Status_Yellow());
  digitalWrite(8, holder.getLED_Status_Green());

  const Display& d = holder.getDisplay();
  if (d != prevDisplay) {
    //avoids display flickering
    prevDisplay = d;
    const int x = d.getdisplayMode();
    if (x == Display::SHOW_USAGEMODE) {
      //  Serial.print("case Display::SHOW_USAGEMODE/Value:");
      matrix.print(d.getValue());
      matrix.drawColon(false);
      matrix.writeDisplay();

    } else if (x == Display::SHOW_VALUE) {
      //Serial.print("case Display::SHOW_USAGEMODE/Value:");
      matrix.print(d.getValue());
      matrix.drawColon(false);
      matrix.writeDisplay();

    } else if (x == Display::SHOW_MIN_SEC) {
      //Serial.print("case Display::SHOW_MIN_SEC:");
      matrix.print(float(d.getValue() / 60) + (float(d.getValue() % 60) / 100));
      matrix.drawColon(true);
      matrix.writeDisplay();

    } else if (x == Display::SHOW_SEC_HUN) {
      //Serial.print("case Display::SHOW_SEC_HUN:");
      matrix.print(float(d.getValue()) / 1000 );
      matrix.drawColon(true);
      matrix.writeDisplay();

    } else if (x == Display::SHOW_CLEAR) {
      //Serial.print("case Display::default/clear");
      matrix.clear();
      matrix.drawColon(false);
      matrix.writeDisplay();

    } else if (x == Display::SHOW_SUCCESS_ROUND) {
      //Serial.print("case Display::default/clear");
      int a = d.getValue() / 100;
      int b = d.getValue() - (a * 100);
      float fa = a;
      fa += float(b) / 100;
      matrix.print(fa);
      matrix.drawColon(true);
      matrix.writeDisplay();
    }
  }
  //Serial.print(2);
}
