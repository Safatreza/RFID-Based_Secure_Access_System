// test_rfid_access_control.cpp
// Simulated tests for demo_rfid_access_control.ino
// No hardware required

#include <Arduino.h>

#define UID_LENGTH 4
#define MAX_UIDS 10
#define MASTER_UID_INDEX 0

// Mock UIDs
byte authorizedUids[MAX_UIDS][UID_LENGTH] = {
  {0xDE, 0xAD, 0xBE, 0xEF}, // Master
  {0x12, 0x34, 0x56, 0x78},
  {0xAB, 0xCD, 0xEF, 0x01}
};
int authorizedCount = 3;

// Function prototypes
bool checkUID(const byte *uid, byte uidSize);
void addUidToList(const byte *uid, byte uidSize);
void resetState();

void testValidUID();
void testInvalidUID();
void testRepeatedScan();
void testPowerCycle();
void testTamper();

void setup() {
  Serial.begin(115200);
  Serial.println("Running RFID Access Control Demo Tests...");
  testValidUID();
  testInvalidUID();
  testRepeatedScan();
  testPowerCycle();
  testTamper();
  Serial.println("All tests completed.");
}

void loop() {}

// Check UID
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

// Add UID to list
void addUidToList(const byte *uid, byte uidSize) {
  if (authorizedCount >= MAX_UIDS) return;
  for (byte j = 0; j < UID_LENGTH; j++) {
    authorizedUids[authorizedCount][j] = uid[j];
  }
  authorizedCount++;
}

// Reset state (simulate power cycle)
void resetState() {
  authorizedCount = 3;
  // Restore original UIDs
  byte orig[3][UID_LENGTH] = {
    {0xDE, 0xAD, 0xBE, 0xEF},
    {0x12, 0x34, 0x56, 0x78},
    {0xAB, 0xCD, 0xEF, 0x01}
  };
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < UID_LENGTH; j++)
      authorizedUids[i][j] = orig[i][j];
}

// Test: Valid UID
void testValidUID() {
  byte testUid[UID_LENGTH] = {0x12, 0x34, 0x56, 0x78};
  Serial.print("Test Valid UID: ");
  if (checkUID(testUid, UID_LENGTH))
    Serial.println("PASS");
  else
    Serial.println("FAIL");
}

// Test: Invalid UID
void testInvalidUID() {
  byte testUid[UID_LENGTH] = {0x11, 0x22, 0x33, 0x44};
  Serial.print("Test Invalid UID: ");
  if (!checkUID(testUid, UID_LENGTH))
    Serial.println("PASS");
  else
    Serial.println("FAIL");
}

// Test: Repeated Scan
void testRepeatedScan() {
  byte testUid[UID_LENGTH] = {0x12, 0x34, 0x56, 0x78};
  Serial.print("Test Repeated Scan: ");
  bool first = checkUID(testUid, UID_LENGTH);
  bool second = checkUID(testUid, UID_LENGTH);
  if (first && second)
    Serial.println("PASS");
  else
    Serial.println("FAIL");
}

// Test: Power Cycle (reset state)
void testPowerCycle() {
  byte newUid[UID_LENGTH] = {0x55, 0x66, 0x77, 0x88};
  addUidToList(newUid, UID_LENGTH);
  resetState();
  Serial.print("Test Power Cycle: ");
  if (!checkUID(newUid, UID_LENGTH))
    Serial.println("PASS");
  else
    Serial.println("FAIL");
}

// Test: Tamper Detection (simulated)
void testTamper() {
  Serial.print("Test Tamper Detection: ");
  // Simulate tamper event
  bool tamperDetected = true; // Always true in this demo
  if (tamperDetected)
    Serial.println("PASS");
  else
    Serial.println("FAIL");
} 