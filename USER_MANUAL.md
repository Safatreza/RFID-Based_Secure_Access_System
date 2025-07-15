# USER MANUAL: Running the RFID-Based Secure Access System

**Project Repository:** [https://github.com/Safatreza/RFID-Based_Secure_Access_System](https://github.com/Safatreza/RFID-Based_Secure_Access_System)

This guide explains how to run the Python dashboard (main and demo modes) and how to upload the ESP32 firmware. All steps use the VS Code terminal—no Arduino IDE required for the dashboard.

---

## 1. Running the Python Dashboard (Live & Demo Modes)

### Prerequisites
- Python 3.7 or higher installed
- Install dependencies:

  **Terminal (any OS):**
  ```sh
  pip install -r rfid_dashboard/requirements.txt
  ```

### Start the Dashboard (Live Mode)
1. Connect your ESP32 device to your computer (for live mode).
2. Open a terminal in VS Code.
3. Run:
   ```sh
   cd rfid_dashboard
   python app.py
   ```
4. The dashboard will start in **live** mode by default. Open your browser to [http://localhost:5000](http://localhost:5000).

### Switch to Demo Mode
You can switch modes from the dashboard web interface, or by sending a command in the terminal.

#### **For Windows PowerShell:**
```powershell
Invoke-RestMethod -Uri http://localhost:5000/api/mode -Method Post -ContentType 'application/json' -Body '{"mode": "demo"}'
```

#### **For Linux/macOS Terminal:**
```sh
curl -X POST -H "Content-Type: application/json" -d '{"mode": "demo"}' http://localhost:5000/api/mode
```

### Switch Back to Live Mode
#### **For Windows PowerShell:**
```powershell
Invoke-RestMethod -Uri http://localhost:5000/api/mode -Method Post -ContentType 'application/json' -Body '{"mode": "live"}'
```

#### **For Linux/macOS Terminal:**
```sh
curl -X POST -H "Content-Type: application/json" -d '{"mode": "live"}' http://localhost:5000/api/mode
```

---

## 2. Uploading the ESP32 Firmware

You cannot upload Arduino code directly from the VS Code terminal unless you use PlatformIO or Arduino CLI. The standard way is:

1. Open `esp32_rfid_access_control.ino` in the Arduino IDE.
2. Select the correct board and port.
3. Click **Upload**.

*For advanced users: You can use PlatformIO in VS Code to upload firmware. See PlatformIO documentation for details.*

---

**If you have any issues, make sure the dashboard server is running before sending mode switch commands.** 