#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

// Pin definitions
#define SS_PIN    5   // Default ESP32 SPI SS for MFRC522
#define RST_PIN   22  // Default ESP32 RST for MFRC522
#define GREEN_LED 16
#define RED_LED   17
#define BUZZER    4
#define TAMPER_SWITCH 5

#define FIRMWARE_VERSION "FW_VER_1.0.0"

// EEPROM layout
#define EEPROM_SIZE 128
#define UID_LENGTH 4
#define MAX_UIDS 10
#define MASTER_UID_INDEX 0 // First UID in EEPROM is master

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Array to hold authorized UIDs loaded from EEPROM
byte authorizedUids[MAX_UIDS][UID_LENGTH];
int authorizedCount = 0;

// Function prototypes
void setupRFID();
void loadAuthorizedUids();
bool checkUID(const byte *uid, byte uidSize);
void addUidToEEPROM(const byte *uid, byte uidSize);
void grantAccess();
void denyAccess();
void printUID(const byte *uid, byte uidSize);
void blinkRedLedRapidly(int times);

void setup() {
  Serial.begin(115200);
  Serial.println(FIRMWARE_VERSION);
  Serial.println("RFID Access Control System Initialized");

  // Initialize hardware
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TAMPER_SWITCH, INPUT_PULLUP);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  // Initialize EEPROM and RFID
  EEPROM.begin(EEPROM_SIZE);
  setupRFID();
  loadAuthorizedUids();
}

// Initialize SPI and MFRC522 RFID module
void setupRFID() {
  SPI.begin();
  mfrc522.PCD_Init();
}

// Load authorized UIDs from EEPROM into RAM
void loadAuthorizedUids() {
  authorizedCount = 0;
  for (int i = 0; i < MAX_UIDS; i++) {
    bool empty = true;
    for (int j = 0; j < UID_LENGTH; j++) {
      byte val = EEPROM.read(i * UID_LENGTH + j);
      authorizedUids[i][j] = val;
      if (val != 0xFF) empty = false;
    }
    if (!empty) authorizedCount++;
  }
}

// Check if a UID is in the authorized list
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

// Add a new UID to EEPROM and RAM
void addUidToEEPROM(const byte *uid, byte uidSize) {
  if (authorizedCount >= MAX_UIDS) return;
  for (byte j = 0; j < UID_LENGTH; j++) {
    EEPROM.write(authorizedCount * UID_LENGTH + j, uid[j]);
    authorizedUids[authorizedCount][j] = uid[j];
  }
  authorizedCount++;
  EEPROM.commit();
  Serial.println("New UID added to EEPROM.");
}

// Print a UID to the serial monitor
void printUID(const byte *uid, byte uidSize) {
  Serial.print("Card UID:");
  for (byte i = 0; i < uidSize; i++) {
    Serial.print(" ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}

// Grant access: turn on green LED and buzzer for 1 second
void grantAccess() {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

// Deny access: turn on red LED and buzzer for 1 second
void denyAccess() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

// Blink the red LED rapidly (for tamper alert)
void blinkRedLedRapidly(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(RED_LED, HIGH);
    delay(100);
    digitalWrite(RED_LED, LOW);
    delay(100);
  }
}

void loop() {
  // Check tamper switch
  if (digitalRead(TAMPER_SWITCH) == LOW) {
    Serial.println("Tamper detected!");
    blinkRedLedRapidly(10);
    delay(200); // Avoid spamming
    return;
  }

  // Look for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID for debugging
  printUID(mfrc522.uid.uidByte, mfrc522.uid.size);

  // Check for master card (first UID in EEPROM)
  bool isMaster = true;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (authorizedUids[MASTER_UID_INDEX][i] != mfrc522.uid.uidByte[i]) {
      isMaster = false;
      break;
    }
  }
  if (isMaster) {
    Serial.println("Master card detected. Waiting for new card to add...");
    // Wait for a new card
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
      delay(100);
      if (digitalRead(TAMPER_SWITCH) == LOW) return; // Tamper check during wait
    }
    // Add new UID
    addUidToEEPROM(mfrc522.uid.uidByte, mfrc522.uid.size);
    grantAccess();
    delay(2000); // Debounce
    return;
  }

  // Check UID and provide feedback
  if (checkUID(mfrc522.uid.uidByte, mfrc522.uid.size)) {
    grantAccess();
    Serial.println("Access Granted");
  } else {
    denyAccess();
    Serial.println("Access Denied");
  }

  // Halt PICC and add debounce
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(2000); // 2-second debounce
} 