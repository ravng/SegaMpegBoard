#include <Arduino.h>
/*
Sega Mpeg Board emulator
By Sixbynine 2022


Tracks:
Filename  Use
0.mp3     Intro/Attract
1.mp3     Medium Track
2.mp3     Beginner Night
3.mp3     Beginner Day
4.mp3     Extra
5.mp3     Expert
6.mp3     Selector 
7.mp3     Name entry 
8.mp3     Goal
9.mp3     Ending

Folder equals selection jumpers in binary:
Examples:
SD:1/0.mp3  = Jumpers(A-D) 0001
SD:2/0.mp3  = Jumpers(A-D) 0010 
and so on.
Folder from 1 to 15
*/

// DFMini setup
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mp3serial(7,6);
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

unsigned int getfolder() {
  int sA = digitalRead(selectA);
  int sB = digitalRead(selectB);
  int sC = digitalRead(selectC);
  int sD = digitalRead(selectD);
  return(1*sA + 2*sB + 4*sC + 8*sD);
}

void playtrack(byte track) {
  digitalWrite(led, LOW);
  if ( trackplaying != track ) {
    myDFPlayer.playFolder(folder, track);  
  }
  trackplaying=track;
  digitalWrite(led, HIGH);
}
void stoptrack() {
  digitalWrite(led, LOW);
  myDFPlayer.pause();
  trackplaying=0;
  digitalWrite(led, HIGH);
}
void setup() {
  Serial.begin(31250);     // Midi serial
  mp3serial.begin(9600);   // Mp3 serial
  pinMode(led, OUTPUT);
  pinMode(selectA, INPUT_PULLUP);
  pinMode(selectB, INPUT_PULLUP);
  pinMode(selectC, INPUT_PULLUP);
  pinMode(selectD, INPUT_PULLUP);
  folder=getfolder();
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  // Init mp3
  myDFPlayer.begin(mp3serial);
  digitalWrite(led, HIGH);
  myDFPlayer.volume(20);
  myDFPlayer.enableLoopAll();
}

void loop() {
  // Check for Midi data
  if ( Serial.available() ) {
      mididata[c++] = Serial.read();
      if ( c == 3 ) {
        // Parse midi data
        if ( mididata[0] == 0xAF and mididata[1] == 0x6F ) {  // Song command
          playtrack(mididata[2]);
        }
        if ( mididata[0] == 0xAF and mididata[1] == 0x3 and mididata[2] == 0x18 ) {  // Stop
          stoptrack();
        }
        c=0;         
      }
  }
}

