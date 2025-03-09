# 🎸 Assistive Guitar Device – Song Selection & Chord Playback  

This project is an **Arduino-based assistive guitar device** that allows users to **select a song using push buttons** and **play chord progressions using a foot pedal**. The system uses a **DFPlayer Mini** to play **pre-recorded chords and song names** stored on an **SD card**.

---

## SD Card Directory Structure  

Ensure that your **SD card is formatted as FAT32**, and organize the files in the following structure:

```
/01/ (All unique chords stored here)
├── 001.mp3  (C Chord)
├── 002.mp3  (D Chord)
├── 003.mp3  (E Chord)
├── 004.mp3  (F Chord)
├── 005.mp3  (G Chord)
├── 006.mp3  (A Chord)
├── 007.mp3  (B Chord)

/02/ (Each JSON file defines a song's chord sequence)
├── song1.json (Song 1 - You Are My Sunshine)
├── song2.json (Song 2 - Twinkle Twinkle Little Star)
├── song3.json (Song 3 - Happy Birthday)
├── song4.json (Song 4 - Jingle Bells)

/03/ (Plays song names during selection)
├── 001.mp3  (You Are My Sunshine)
├── 002.mp3  (Twinkle Twinkle Little Star)
├── 003.mp3  (Happy Birthday)
├── 004.mp3  (Jingle Bells)

```

---

## How the System Works  

### 1️. Startup  
- The Arduino initializes the **DFPlayer Mini** and **SD card**.  
- Waits for the user to **select a song**.  

### 2️. Song Selection Mode  
- **Press "Next" (⏭️) or "Back" (⏮️) buttons** to navigate through the songs.  
- Each button press **plays the corresponding song name MP3** from `/03/`.  
- **Press "Select" (✅) to confirm a song**.  

### 3️. Chord Playback Mode (After Song Selection)  
- Once a song is selected, **the foot pedal becomes active**.  
- **Each foot pedal press plays the next chord** in the song’s JSON sequence.  
- **Chords are played from the `/01/` directory**, based on the numbers stored in the JSON file.  

### 4️. Looping  
- If the song reaches the last chord, it **automatically restarts** from the first chord.  

---

## JSON File Structure  
Each song has its own **JSON file** stored in `/02/`. The JSON file contains:  

- `"title"` → The name of the song.  
- `"file"` → The MP3 file number for announcing the song name.  
- `"chords"` → The sequence of chords (matching the numbers in `/01/`).  

### Example: **`/02/song1.json` (You Are My Sunshine)**
```json
{
    "title": "You Are My Sunshine",
    "file": "001.mp3",
    "chords": [1, 5, 6, 4, 1, 5, 1]
}
```

### Example: **`/02/song2.json` (Twinkle Twinkle Little Star)**
```json
{
    "title": "Twinkle Twinkle Little Star",
    "file": "002.mp3",
    "chords": [1, 1, 5, 5, 6, 6, 5]
}
```

---

## Hardware Components
- **Arduino Uno** (Main microcontroller)

- **DFPlayer Mini** (Plays MP3 files from the SD card)

- **MicroSD Card** (Formatted as FAT32)

- **Speaker** (Connected to DFPlayer Mini)

- **3 Push Buttons** (Next ⏭️, Back ⏮️, Select ✅)

- **Foot Pedal Switch** (Advances to the next chord)

- **Jumper Wires & Breadboard**