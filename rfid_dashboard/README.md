# RFID Access Control Dashboard (Flask)

A web dashboard for monitoring an RFID-based access control system using ESP32 and MFRC522.

## Features
- Live Mode: Reads serial data from ESP32 via USB (pyserial)
- Demo Mode: Simulates RFID scans and access events
- Displays firmware version, last UID, access status, and scan log
- Toggle between live and demo mode
- Auto-refreshes every 2 seconds
- Clean Bootstrap UI

## Setup Instructions

### 1. Clone the Repository
```
git clone <repo-url>
cd rfid_dashboard
```

### 2. Install Dependencies
```
pip install -r requirements.txt
```

### 3. Connect ESP32 (Live Mode)
- Plug in your ESP32 via USB
- Set the serial port (optional):
  - By default, the app tries to auto-detect the port
  - Or set the environment variable:
    ```
    export RFID_SERIAL_PORT=COM3  # Windows example
    export RFID_SERIAL_PORT=/dev/ttyUSB0  # Linux example
    ```

### 4. Run the App
```
python app.py
```
- Open your browser to [http://localhost:5000](http://localhost:5000)

### 5. Usage
- Use the toggle to switch between demo and live mode
- Click refresh or wait for auto-refresh
- View firmware, UID, access status, and scan log

## Notes
- Demo mode does not require hardware
- Live mode requires ESP32 running compatible firmware
- Bootstrap is loaded via CDN

--- 