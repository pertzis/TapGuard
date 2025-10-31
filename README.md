# TapGuard: ESP32 RFID Access Control System

Built with an **ESP32** microcontroller and an **Express web server** written in **TypeScript**, using **EJS templates** and **API endpoints**.  
The ESP32 reads an RFID card, sends their UIDs over WiFi through an `HTTP GET` request to the backend server for validation. The rest API responds with a status code, the name of the registered owner of the access card, and the check-in or check-out time, according to the card's state. Feedback is then provided via LEDs, a buzzer, and an LCD display, handled by the ESP32.

---

## Features

- Reads and processes RFID tags using the RC522 module  
- Connects to the web server via WiFi  
- Displays messages on a 20x4 LCD  
- Implements a buzzer and LED status indicators feedback  
- Uses database to perform CRUD operations on registered cards, access logs, and current card states (checked in, checked out, etc).

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

