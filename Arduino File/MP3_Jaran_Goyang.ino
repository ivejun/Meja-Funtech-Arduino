#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

#define outputA 8
#define outputB 9
int counter = 0;
int aState;
int aLastState;
bool playmusikawal = true;
bool playmusik = true;
int penampung[100];
int penambah = 1;
int selisih1 = 0;
int selisih2 = 0;
int selisih3 = 0;
int selisih4 = 0;
bool play = true;

const int merah = 5;
const int biru = 7;
const int hijau = 6;
int pilihanlagu = 1;
void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  pinMode (merah, OUTPUT);
  pinMode (biru, OUTPUT);
  pinMode (hijau, OUTPUT);

  digitalWrite(hijau, HIGH);
  digitalWrite(merah, HIGH);
  digitalWrite(biru, HIGH);

  aLastState = digitalRead(outputA);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);
  myDFPlayer.reset();
  delay(1000);
  myDFPlayer.volume(40);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

void loop() {
  aState = digitalRead(outputA);
  ForceStop();
  
  if (aState != aLastState) {
    Serial.print("Position: ");
    Serial.println(counter);
    if (digitalRead(outputB) != aState) {
      counter ++;
    }
    play = true;
  }

  if (counter > 0 || counter < 0) {
    if (counter == 25 || counter == -25) {
      if (play) {
        myDFPlayer.play(pilihanlagu);
        digitalWrite(merah, LOW);
        play = false;
      }
    }
    else if (counter == 50 || counter == -50) {
      if (play) {
        digitalWrite(merah, HIGH);
        digitalWrite(hijau, LOW);
        play = false;
      }
    }
    else if (counter == 75 || counter == -75) {
      if (play) {
        digitalWrite(hijau, HIGH);
        digitalWrite(biru, LOW);
        play = false;
      }
    }
    else if (counter == 100 || counter == -100) {
      if (play) {
        digitalWrite(hijau, LOW);
        digitalWrite(biru, LOW);
        play = false;
      }
    }
    else if (counter == 125 || counter == -125) {
      if (play) {
        digitalWrite(hijau, HIGH);
        digitalWrite(merah, LOW);
        digitalWrite(biru, LOW);

        play = false;
      }
    }
    else if (counter == 150 || counter == -150) {
      if (play) {
        digitalWrite(hijau, LOW);
        digitalWrite(merah, LOW);
        digitalWrite(biru, HIGH);
        play = false;
      }
    }
    else if (counter == 175 || counter == -175) {
      if (play) {
        digitalWrite(hijau, LOW);
        digitalWrite(merah, LOW);
        digitalWrite(biru, LOW);
        play = false;
      }
    }
  }
  aLastState = aState;
}

void playnext() {
  static unsigned long timer = millis();
  if (millis() - timer > 5000) {
    timer = millis();
    myDFPlayer.next();
    counter = 99;
  }
}
void ForceStop(){
  penampung[penambah] = counter;
  if (penambah == 8) {
    penambah = 1;
    selisih1 = penampung[1] - penampung[2];
    selisih2 = penampung[3] - penampung[4];
    selisih3 = penampung[5] - penampung[6];
    selisih4 = penampung[7] - penampung[8];
    Serial.println("selisih :");
    Serial.println(selisih1);
    Serial.println(selisih2);
    Serial.println(selisih3);
    Serial.println(selisih4);
    if (selisih1 - selisih2 == 0 && selisih3 - selisih4 == 0) {
      counter = 0;
      pilihanlagu = pilihanlagu + 1;
      digitalWrite(hijau, HIGH);
      digitalWrite(merah, HIGH);
      digitalWrite(biru, HIGH);
      if (pilihanlagu > 2) {
        pilihanlagu = 1;
      }
    }
  }
  penambah = penambah + 1;
}
