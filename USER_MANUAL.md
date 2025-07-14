# User Manual: ESP32 RFID-Based Secure Access System

## Flashing the Firmware
1. Connect your ESP32 to your computer via USB.
2. Open a terminal and navigate to the project directory.
3. Open the Arduino IDE or PlatformIO.
4. Select the correct board and port.
5. Open `src/esp32_rfid_access_control.ino`.
6. Click Upload or use the following PlatformIO command:
   ```
   pio run --target upload
   ```

## Running the Demo Version
1. Open `src/demo_rfid_access_control.ino` in the Arduino IDE.
2. Select any Arduino-compatible board (no hardware required).
3. Upload and open the Serial Monitor at 115200 baud.

## Viewing Logs
- Open the Serial Monitor at 115200 baud to view firmware logs, scanned UIDs, and system messages.
- For PlatformIO:
  ```
  pio device monitor -b 115200
  ```

## Adding/Removing UIDs
- **Add UID:**
  1. Scan the master card (first UID in EEPROM or mock list).
  2. When prompted, scan a new card/tag to add it to the authorized list.
- **Remove UID:**
  - Not supported in this version. To remove all UIDs, erase or re-flash EEPROM.

## Expected Outputs
- On boot: Firmware version and system ready message.
- On card scan: UID is printed, access is granted/denied, and LEDs/buzzer respond accordingly.
- On tamper event: "Tamper detected!" and red LED blinks rapidly.

## Troubleshooting
- **No output on Serial Monitor:**
  - Check baud rate (115200).
  - Ensure correct COM port and board selection.
- **RFID not detected:**
  - Check wiring (SPI pins, SS, RST).
  - Ensure MFRC522 library is installed.
- **LEDs/Buzzer not working:**
  - Check GPIO pin assignments and connections.
- **Cannot add new UID:**
  - Ensure master card is present and scanned first.
  - Check EEPROM size and UID limits.

--- 