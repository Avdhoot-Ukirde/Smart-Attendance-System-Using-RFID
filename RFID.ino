#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

#define GREEN_LED 4
#define RED_LED 5
#define BUZZER 2

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Stored UIDs
byte avdhootUID[4] = {0x43, 0xE7, 0x5C, 0xF5}; // Avdhoot
byte ramUID[4]     = {0x02, 0x74, 0xAD, 0xAB}; // Ram
byte shambhuUID[4] = {0x83, 0xEE, 0x83, 0x39}; // Shambhu

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(2, 0);
  lcd.print("SHOW YOUR");
  lcd.setCursor(3, 1);
  lcd.print("ID CARD");

  Serial.println("Scan your RFID card...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  lcd.clear();

  if (checkUID(rfid.uid.uidByte, avdhootUID)) {
    Serial.println("AVDHOOT is present");
    lcd.setCursor(0, 0);
    lcd.print("AVDHOOT");
    lcd.setCursor(0, 1);
    lcd.print("PRESENT");
    successSignal();
  }
  else if (checkUID(rfid.uid.uidByte, ramUID)) {
    Serial.println("RAM is present");
    lcd.setCursor(0, 0);
    lcd.print("RAM");
    lcd.setCursor(0, 1);
    lcd.print("PRESENT");
    successSignal();
  }
  else if (checkUID(rfid.uid.uidByte, shambhuUID)) {
    Serial.println("SHAMBHU is present");
    lcd.setCursor(0, 0);
    lcd.print("SHAMBHU");
    lcd.setCursor(0, 1);
    lcd.print("PRESENT");
    successSignal();
  }
  else {
    Serial.println("Unauthorized Access");
    lcd.setCursor(0, 0);
    lcd.print("UNAUTHORIZED");
    lcd.setCursor(0, 1);
    lcd.print("ACCESS");
    failSignal();
  }

  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("SHOW YOUR");
  lcd.setCursor(3, 1);
  lcd.print("ID CARD");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool checkUID(byte *scannedUID, byte *storedUID) {
  for (byte i = 0; i < 4; i++) {
    if (scannedUID[i] != storedUID[i]) {
      return false;
    }
  }
  return true;
}

void successSignal() {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  tone(BUZZER, 1000);
  delay(200);
  noTone(BUZZER);
  delay(500);
  digitalWrite(GREEN_LED, LOW);
}

void failSignal() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  tone(BUZZER, 500);
  delay(1000);
  noTone(BUZZER);
  digitalWrite(RED_LED, LOW);
}