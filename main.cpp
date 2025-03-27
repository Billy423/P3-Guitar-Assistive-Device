#include "ArduinoJson.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

const int pedalPin = 2;
const int backButton = 3;
const int selectButton = 4;
const int nextButton = 5;

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySerial(10, 11);

struct Song {
    const char* title;
    int titleTrack;
    int chords[50];
    int chordCount;
};

Song songs[] = {
    {
        "Hotel California", 
        1,
        {6, 15, 1, 12, 16, 10, 13, 15, 15, 16, 1, 5, 1, 15, 8, 10, 12, 15, 12, 5},
        20
    },
    {
        "Stairway to Heaven", 
        2,
        {2, 17, 7, 10, 14, 16, 2, 17, 7, 10, 14, 16, 2, 17, 7, 10, 14, 16, 2, 17},
        20
    },
    {
        "Sweet Child O' Mine", 
        3,
        {10, 7, 16, 10, 10, 7, 16, 10, 10, 7, 16, 10, 10, 7, 16, 10, 10, 7, 16, 10, 10, 7},
        22
    },
    {
        "Wish You Were Here", 
        4,
        {13, 16, 13, 16, 13, 3, 13, 3, 13, 16, 13, 16, 13, 3, 13, 3},
        16
    },
    {
        "Blackbird", 
        5,
        {16, 4, 18, 16, 16, 4, 18, 16, 16, 4, 18, 16, 16, 4, 18, 16},
        16
    }
};

int currentSong = 0;
int totalSongs = 5;
bool songSelected = false;
int currentChord = 0;

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);

    pinMode(pedalPin, INPUT_PULLUP);
    pinMode(backButton, INPUT_PULLUP);
    pinMode(selectButton, INPUT_PULLUP);
    pinMode(nextButton, INPUT_PULLUP);

    if (!myDFPlayer.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
        while(true){
            delay(0); // Code to compatible with ESP8266 watch dog.
        }
    }

    Serial.println(F("DFPlayer Mini online."));
    
    myDFPlayer.volume(30);
    announceCurrentSong();
    myDFPlayer.playFolder(3, 1); // play "selecting songs"
}

void loop() {
    if (!songSelected && digitalRead(backButton) == LOW) {
        Serial.println("Back Pressed.");
        previousSong();
        delay(1000);
    }
    if (!songSelected && digitalRead(nextButton) == LOW) {
        Serial.println("Next Pressed.");
        nextSong();
        delay(1000);
    }
    if (!songSelected && digitalRead(selectButton) == LOW) {
        Serial.println("Select Pressed.");
        selectSong();
        delay(1000);
    }
    if (songSelected && digitalRead(pedalPin) == LOW) {
        Serial.println("Pedal Pressed.");
        playNextChord();
        delay(1000);
    }
}
void previousSong() {
    currentSong = (currentSong - 1 + totalSongs) % totalSongs;
    announceCurrentSong();
}

void nextSong() {
    currentSong = (currentSong + 1) % totalSongs;
    announceCurrentSong();
}

void announceCurrentSong() {
    Serial.print("Current Song: ");
    Serial.println(songs[currentSong].title);
    
    // Play announcement from folder 02
    myDFPlayer.playFolder(2, songs[currentSong].titleTrack);
}

void selectSong() {
    Serial.print("Song Selected: ");
    Serial.println(songs[currentSong].title);
    myDFPlayer.playFolder(3, 2); // Play "The current song is ..."
    announceCurrentSong();
    songSelected = true;
    currentChord = 0;
}

void playNextChord() {
    if (currentChord < songs[currentSong].chordCount) {
        Serial.print("Playing Chord: ");
        Serial.println(songs[currentSong].chords[currentChord]);
        
        // Play chord from folder 01
        myDFPlayer.playFolder(1, songs[currentSong].chords[currentChord]);
        currentChord++;
    } else {
        Serial.println("End of song. Restarting...");
        currentChord = 0;
    }
}
