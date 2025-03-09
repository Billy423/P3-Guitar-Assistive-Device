#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

// Pin assignments
const int pedalPin = 2;
const int backButton = 3;
const int selectButton = 4;
const int nextButton = 5;
const int chipSelect = 10;

SoftwareSerial mySerial(11, 12); // RX, TX for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;

String songFiles[] = { "02/song1.json", "02/song2.json", "02/song3.json", "02/song4.json" };
int currentSong = 0;
int totalSongs = 4;
bool songSelected = false;

String currentSongTitle;
String songFile;
int chords[10]; // Stores chord sequence
int chordCount = 0;
int currentChord = 0;

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);

    pinMode(pedalPin, INPUT_PULLUP);
    pinMode(backButton, INPUT_PULLUP);
    pinMode(selectButton, INPUT_PULLUP);
    pinMode(nextButton, INPUT_PULLUP);

    if (!myDFPlayer.begin(mySerial)) {
        Serial.println("DFPlayer not detected!");
        while (true);
    }
    myDFPlayer.volume(20);
    Serial.println("DFPlayer Mini Ready.");

    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card initialization failed!");
        while (true);
    }
    Serial.println("SD Card Ready.");
}

void loop() {
    if (digitalRead(backButton) == LOW) {
        previousSong();
        delay(300);
    }
    if (digitalRead(nextButton) == LOW) {
        nextSong();
        delay(300);
    }
    if (digitalRead(selectButton) == LOW) {
        selectSong();
        delay(300);
    }
    if (songSelected && digitalRead(pedalPin) == LOW) {
        playNextChord();
        delay(300);
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
        myDFPlayer.playFolder(3, currentSong + 1); // Plays 001.mp3, 002.mp3, etc. from folder 03
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
        Serial.println("Failed to open file!");
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
        chords[i] = chordArray[i].as<int>();
    }

    Serial.print("Loaded song: ");
    Serial.println(currentSongTitle);
    Serial.println();
    return true;
}
