// demo_rfid_access_control.ino
// Demo version: Simulates RFID Secure Access System logic with no hardware dependencies
// All outputs are printed to the serial monitor

#define FIRMWARE_VERSION "FW_VER_1.0.0-DEMO"
#define UID_LENGTH 4
#define MAX_UIDS 10
#define MASTER_UID_INDEX 0

// =====================
// Mock UIDs (change as needed)
// =====================
byte authorizedUids[MAX_UIDS][UID_LENGTH] = {
  {0xDE, 0xAD, 0xBE, 0xEF}, // Master
  {0x12, 0x34, 0x56, 0x78},
  {0xAB, 0xCD, 0xEF, 0x01}
};
int authorizedCount = 3;

// =====================
// Function Prototypes
// =====================
void printUID(const byte *uid, byte uidSize);
bool checkUID(const byte *uid, byte uidSize);
void grantAccess();
void denyAccess();
void simulateRFIDRead(const byte *uid, byte uidSize);
void simulateTamper();
void addUidToList(const byte *uid, byte uidSize);

void setup() {
  Serial.begin(115200);
  Serial.println(FIRMWARE_VERSION);
  Serial.println("Demo: RFID Access Control System");
  Serial.println("Simulating RFID reads and tamper events...");

  // Simulate normal and edge-case scenarios
  simulateRFIDRead(authorizedUids[1], UID_LENGTH); // Valid UID
  simulateRFIDRead((byte[]){0x11, 0x22, 0x33, 0x44}, UID_LENGTH); // Invalid UID
  simulateRFIDRead(authorizedUids[2], UID_LENGTH); // Valid UID
  simulateRFIDRead(authorizedUids[1], UID_LENGTH); // Repeated scan
  simulateTamper();
  simulateRFIDRead(authorizedUids[0], UID_LENGTH); // Master card
  simulateRFIDRead((byte[]){0x55, 0x66, 0x77, 0x88}, UID_LENGTH); // Add new UID
  simulateRFIDRead((byte[]){0x55, 0x66, 0x77, 0x88}, UID_LENGTH); // Now valid
}

void loop() {
  // Nothing in loop; all tests run in setup()
}

// =====================
// Print UID
// =====================
void printUID(const byte *uid, byte uidSize) {
  Serial.print("Card UID:");
  for (byte i = 0; i < uidSize; i++) {
    Serial.print(" ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}

// =====================
// Check UID
// =====================
bool checkUID(const byte *uid, byte uidSize) {
  for (int i = 0; i < authorizedCount; i++) {
    bool match = true;
    for (byte j = 0; j < uidSize; j++) {
      if (authorizedUids[i][j] != uid[j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

// =====================
// Grant Access
// =====================
void grantAccess() {
  Serial.println("[GREEN LED ON] [BUZZER ON] Access Granted");
  delay(1000);
  Serial.println("[GREEN LED OFF] [BUZZER OFF]");
}

// =====================
// Deny Access
// =====================
void denyAccess() {
  Serial.println("[RED LED ON] [BUZZER ON] Access Denied");
  delay(1000);
  Serial.println("[RED LED OFF] [BUZZER OFF]");
}

// =====================
// Simulate RFID Read
// =====================
void simulateRFIDRead(const byte *uid, byte uidSize) {
  printUID(uid, uidSize);
  // Check for master card
  bool isMaster = true;
  for (byte i = 0; i < uidSize; i++) {
    if (authorizedUids[MASTER_UID_INDEX][i] != uid[i]) {
      isMaster = false;
      break;
    }
  }
  if (isMaster) {
    Serial.println("Master card detected. Waiting for new card to add...");
    // Simulate adding a new UID
    byte newUid[UID_LENGTH] = {0x55, 0x66, 0x77, 0x88};
    addUidToList(newUid, UID_LENGTH);
    grantAccess();
    delay(2000);
    return;
  }
  if (checkUID(uid, uidSize)) {
    grantAccess();
  } else {
    denyAccess();
  }
  delay(2000);
}

// =====================
// Simulate Tamper Event
// =====================
void simulateTamper() {
  Serial.println("Tamper detected! [RED LED BLINK]");
  for (int i = 0; i < 10; i++) {
    Serial.println("[RED LED ON]");
    delay(100);
    Serial.println("[RED LED OFF]");
    delay(100);
  }
}

// =====================
// Add UID to List
// =====================
void addUidToList(const byte *uid, byte uidSize) {
  if (authorizedCount >= MAX_UIDS) return;
  for (byte j = 0; j < UID_LENGTH; j++) {
    authorizedUids[authorizedCount][j] = uid[j];
  }
  authorizedCount++;
  Serial.println("New UID added to list.");
} 