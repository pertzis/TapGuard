# Arduino/ESP32 RFID Access Control System

Built with an **ESP32** microcontroller and a **TypeScript-based web server** using **EJS templates**.  
The ESP32 reads an RFID card, sends their UIDs over WiFi to the backend for verification, and provides feedback via LEDs, a buzzer, and an LCD display.

---

## Features

- Reads and processes RFID tags using the RC522 module  
- Connects to web server via WiFi  
- Displays messages on a 20x4 LCD  
- Buzzer and LED feedback for access  
- Database of registered RFID UIDs  

---

## Hardware Components

**ESP32** | Main controller

**RFID Reader (RC522)** | Reads RFID tag UIDs

**LCD Display (20x4)** | Displays messages

**Buzzer** | Audio feedback

**LEDs (Red & Green)** | Access granted = Green, denied = Red

---

## Code

**Firmware** | C++ (Arduino framework for ESP32) | RFID reading and WiFi communication 

**Backend** | TypeScript | For verifying RFID UIDs 

**Frontend** | EJS | Web interface 

**Database** | MySQL | Stores authorized card UIDs and user info 

---

## Contributors

**P. Giamalakis & S. Kleve**

