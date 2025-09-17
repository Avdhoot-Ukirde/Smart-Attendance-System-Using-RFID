#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(13, OUTPUT);
  Serial.println("RFID test: place tag on coil");
}

void loop() {
  digitalWrite(13, LOW);
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  digitalWrite(13, HIGH); // visual indicator: card detected
  Serial.println("Card presence detected");
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card read failed");
    delay(500);
    return;
  }
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  mfrc522.PICC_HaltA(); // stop reading current card
  delay(500);
}