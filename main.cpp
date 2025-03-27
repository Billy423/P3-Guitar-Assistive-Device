#include "ArduinoJson.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include "SD.h" 
#include <SoftwareSerial.h>

const int pedalPin = 2;
const int backButton = 3;
const int selectButton = 4;
const int nextButton = 5;


DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySerial(10, 11);

String songFiles[] = {
    "/02/song1.json",
    "/02/song2.json",
    "/02/song3.json",
    "/02/song4.json",
    "/02/song5.json"

};
int currentSong = 0;
int totalSongs = 5;
bool songSelected = false;

String currentSongTitle;
String songFile;
int chords[50];
int chordCount = 0;
int currentChord = 0;

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(pedalPin, INPUT_PULLUP);
    pinMode(backButton, INPUT_PULLUP);
    pinMode(selectButton, INPUT_PULLUP);
    pinMode(nextButton, INPUT_PULLUP);

    Serial.println("device initiated!");
    myDFPlayer.volume(20);
    announceCurrentSong();
}

void loop() {
  Serial.println("Back Pressed.");
  delay(1000);
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
    Serial.println(currentSong);

    if (loadSongData(songFiles[currentSong])) {
        myDFPlayer.playFolder(3, currentSong + 1); // Play song name from folder 03
    }
}

void selectSong() {
    Serial.print("Song Selected: ");
    Serial.println(currentSongTitle);

    songSelected = true;
    currentChord = 0;
}

void playNextChord() {
    if (currentChord < chordCount) {
        Serial.print("Playing Chord: ");
        Serial.println(chords[currentChord]);

        myDFPlayer.playFolder(1, chords[currentChord]); // Play from folder 01 (chords)

        currentChord++;
    } else {
        Serial.println("End of song. Restarting...");
        currentChord = 0;
    }
}

bool loadSongData(String filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Failed to open JSON file!");
        return false;
    }

    String jsonString = "";
    while (file.available()) {
        jsonString += (char)file.read();
    }
    file.close();

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.print("JSON Parsing failed: ");
        Serial.println(error.f_str());
        return false;
    }

    currentSongTitle = doc["title"].as<String>();
    songFile = doc["file"].as<String>();

    JsonArray chordArray = doc["chords"].as<JsonArray>();
    chordCount = chordArray.size();
    for (int i = 0; i < chordCount; i++) {
        chords[i] = chordArray[i].as<int>(); // Read chord numbers
    }

    Serial.print("Loaded song: ");
    Serial.println(currentSongTitle);
    Serial.println();
    return true;
}
