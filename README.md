# ESP32 RFID-Based Secure Access System

## Overview
This project implements a secure access control system using an ESP32 microcontroller and an MFRC522 RFID module. It supports persistent storage of authorized RFID UIDs, access feedback via LEDs and buzzer, tamper detection, and a master card for adding new UIDs. A demo version and test cases are included for logic simulation and validation.

## Hardware Used
- ESP32 Dev Board
- MFRC522 RFID Module (SPI)
- Green LED (GPIO 16)
- Red LED (GPIO 17)
- Buzzer (GPIO 4)
- Tamper Switch (GPIO 5)
- RFID Cards/Tags (MIFARE Classic, etc.)
- Breadboard, wires, resistors

## Folder Structure
```
.
├── src/
│   ├── esp32_rfid_access_control.ino   # Main firmware
│   └── demo_rfid_access_control.ino    # Demo version (no hardware)
├── test/
│   └── test_rfid_access_control.cpp    # Simulated test cases
├── README.md
└── ...
```

## How to Build and Flash the Firmware
1. Open `src/esp32_rfid_access_control.ino` in the Arduino IDE or PlatformIO.
2. Install the following libraries:
   - MFRC522
   - EEPROM (built-in for ESP32)
3. Select your ESP32 board and correct COM port.
4. Connect the hardware as per the pin assignments above.
5. Click Upload to flash the firmware.

## How to Run the Demo Version
1. Open `src/demo_rfid_access_control.ino` in the Arduino IDE.
2. Select any Arduino-compatible board (no hardware required).
3. Upload and open the Serial Monitor at 115200 baud.
4. Observe simulated RFID reads, access feedback, and tamper events.

## File Descriptions
- `src/esp32_rfid_access_control.ino`: Main firmware for ESP32 + MFRC522
- `src/demo_rfid_access_control.ino`: Demo version, simulates logic with serial output
- `test/test_rfid_access_control.cpp`: Simulated test cases for demo version

## Notes
- Change GPIO pin assignments and mock UIDs as needed in the source files.
- The master card is the first UID in EEPROM or the mock list.
- Tamper detection is triggered by pulling GPIO 5 LOW.

--- # RFID-Based_Secure_Access_System
