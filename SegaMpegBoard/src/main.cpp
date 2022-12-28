#include <Arduino.h>
/*
Sega Mpeg Board emulator
By Sixbynine 2022

Tracks:
Filename  Use
01/001.mp3     Medium Track
01/002.mp3     Beginner Night
01/003.mp3     Beginner Day
01/004.mp3     Extra
01/005.mp3     Expert
01/006.mp3     Selector 
01/007.mp3     Name entry 
01/008.mp3     Goal
01/009.mp3     Ending
01/010.mp3     Intro/Attract
*/

// DFMini setup
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include <EEPROM.h>
SoftwareSerial mp3serial(7,6);
SoftwareSerial midiserial(5,4);
DFRobotDFPlayerMini myDFPlayer;

// HW setup
#define led 1
#define selectA 3
#define selectB 8
#define selectC 9
#define selectD 10

// Globals
byte mididata[3];
unsigned int c=0;
unsigned int folder=0;
unsigned int trackplaying=0;
unsigned long ts=millis();

unsigned int getfolder() {
  // Get jumper settings for folder to play
  int sA = digitalRead(selectA);
  int sB = digitalRead(selectB);
  int sC = digitalRead(selectC);
  int sD = digitalRead(selectD);
  if ( sA == 1 and sB == 1 and sC == 1 and sD == 1 )
    return(1);
  if ( sA == 0 and sB == 1 and sC == 1 and sD == 1 )
    return(2);
  if ( sA == 1 and sB == 0 and sC == 1 and sD == 1 )
    return(3);
  if ( sA == 1 and sB == 1 and sC == 0 and sD == 1 )
    return(4);
  if ( sA == 1 and sB == 1 and sC == 1 and sD == 0 )
    return(5);
  return(1);  
}

void blinkled(int num) {
    for ( int i=0; i < num; i++ ) {
      digitalWrite(led, HIGH);
      delay(200);
      digitalWrite(led, LOW);
      delay(200);
    }
}
void playtrack(byte track) {
  if ( track == 0 )
    track=10;
  mp3serial.listen(); 
  if ( trackplaying != track ) {
    myDFPlayer.playFolder(folder, track);  
  }
  trackplaying=track;
  midiserial.listen();
}

void stoptrack() {
  mp3serial.listen();
  myDFPlayer.pause();
  trackplaying=0;
  midiserial.listen();
}

void setup() {
  // Tune cpu
  byte T = EEPROM.read(0);
  OSCCAL = T;
  midiserial.begin(31250);              
  mp3serial.begin(9600);            
  pinMode(led, OUTPUT);             
  digitalWrite(led, LOW);
  pinMode(selectA, INPUT_PULLUP);
  pinMode(selectB, INPUT_PULLUP);
  pinMode(selectC, INPUT_PULLUP);
  pinMode(selectD, INPUT_PULLUP);
  folder=getfolder();
  blinkled(folder);
  mp3serial.listen();
  myDFPlayer.begin(mp3serial);
  myDFPlayer.volume(20);
  myDFPlayer.enableLoop();
}

void loop() {
  // Check for Midi data
  midiserial.listen();
  if ( midiserial.available() ) {
      // Capture 3 bytes
      byte indata = midiserial.read();
      if ( indata == 0xAF )
        c=0;
      mididata[c++] = indata;
      if ( c == 3 ) {
        // Parse midi data
        if ( mididata[0] == 0xAF and mididata[1] == 0x6F ) {  // Song command
          if ( folder != 5 )
            playtrack(mididata[2]);
          else {
           playtrack(random(1,10));
          }
        }
        if ( mididata[0] == 0xAF and mididata[1] == 0x3 and mididata[2] == 0x18 ) {  // Stop command
          stoptrack();
        }
        c=0;         
      }
  }
}