# MP3-Tag-Reader-and-Editor
A C program to read and edit MP3 ID3v1 tags — lightweight command-line tool for viewing and modifying metadata like Title, Artist, Album, Year, Comment, and Genre without external libraries.
# MP3 Tag Reader and Editor

## 📌 Overview
The **MP3 Tag Reader and Editor** is a C program that allows you to read and edit ID3v1 metadata tags from MP3 files.  
These tags store details like **Title**, **Artist**, **Album**, **Year**, **Comment**, and **Genre**.

---

## 🚀 Features
- Read MP3 metadata (ID3v1)
- Edit MP3 tags directly from the terminal
- Lightweight, no external libraries required
- Simple command-line interface

---

## 📂 Project Structure
```
├── main.c           # Entry point
├── mp3_reader.c     # Functions for reading MP3 tags
├── mp3_editor.c     # Functions for editing MP3 tags
├── mp3_tag.h        # Header file with structures and prototypes
├── README.md        # Project documentation
```

---

## 🛠️ How It Works
**Reading:**
1. Opens the MP3 file in binary mode
2. Reads the last 128 bytes (location of ID3v1 tag)
3. Extracts Title, Artist, Album, Year, Comment, Genre

**Editing:**
1. Reads existing tags
2. Updates fields with user input
3. Writes new tags back to the file

---

## 📥 Compilation & Usage

### Compile
```bash
gcc main.c mp3_reader.c mp3_editor.c -o mp3tag
```

### Run

#### Read MP3 Tag
```bash
./mp3tag -r filename.mp3
```

#### Edit MP3 Tag
```bash
./mp3tag -e filename.mp3
```

---

## 🖼️ Example Output

**Before Editing**
```
Title   : Song Name
Artist  : Artist Name
Album   : Album Name
Year    : 2020
Comment : My Comment
Genre   : Pop
```

**After Editing**
```
Title   : New Song Title
Artist  : New Artist
Album   : Album Name
Year    : 2024
Comment : Updated Comment
Genre   : Rock
```

---

## 📜 Supported Tags
- Title (30 characters)
- Artist (30 characters)
- Album (30 characters)
- Year (4 characters)
- Comment (30 characters)
- Genre (1-byte genre code)

---

## ⚠️ Limitations
- Works only with **ID3v1** tags
- Field sizes are fixed by ID3v1 standard
- Editing overwrites original tag data permanently

---

## 📄 License
This project is licensed under the **MIT License**.
