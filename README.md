# 🎸 Assistive Guitar Device – Song Selection & Chord Playback  

This project is an **Arduino-based assistive guitar device** that allows users to **select a song using push buttons** and **play chord progressions using a foot pedal**. The system uses a **DFPlayer Mini** to play **pre-recorded chords and song names** stored on an **SD card**.

---

## SD Card Directory Structure  

Ensure that your **SD card is formatted as FAT32**, and organize the files in the following structure:

```
/01/ (All unique chords stored here)
├── 001.mp3 → A
├── 002.mp3 → Am
├── 003.mp3 → A7sus4
├── 004.mp3 → Am7
├── 005.mp3 → B
├── 006.mp3 → Bm
├── 007.mp3 → C
├── 008.mp3 → C#
├── 009.mp3 → C#dim
├── 010.mp3 → D
├── 011.mp3 → D#
├── 012.mp3 → E
├── 013.mp3 → Em
├── 014.mp3 → F
├── 015.mp3 → F#
├── 016.mp3 → G
├── 017.mp3 → G#
├── 018.mp3 → G over B

/02/ (Plays song names during selection)
├── 001.mp3  (Hotel California)
├── 002.mp3  (Stairwya to Heaven)
├── 003.mp3  (Sweet Child O' Mine)
├── 004.mp3  (Wish You Were Here)
├── 005.mp3  (Blackbird)

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

## Hardware Components
- **Arduino Uno** (Main microcontroller)

- **DFPlayer Mini** (Plays MP3 files from the SD card)

- **MicroSD Card** (Formatted as FAT32)

- **Speaker** (Connected to DFPlayer Mini)

- **3 Push Buttons** (Next ⏭️, Back ⏮️, Select ✅)

- **Foot Pedal Switch** (Advances to the next chord)

- **Jumper Wires & Breadboard**