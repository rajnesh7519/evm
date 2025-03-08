 #include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h> // Include the DFPlayer Mini library

// Initialize LCD with I2C address 0x27 and a 16x4 display
LiquidCrystal_I2C lcd(0x27, 16, 4);
#define mySerial Serial2 // Use Serial2 for the fingerprint sensor
#define mp3Serial Serial1 // Use Serial1 for DFPlayer Mini

const int buttonPin1 = 4;  
const int buttonPin2 = 5; 
const int buttonPin3 = 6; 
const int buttonPin4 = 7;
const int buttonPin5 = 8;
const int buttonPin6 = 9;
const int buttonPin7 = 10;
const int buttonPin8 = 11;
const int buttonPin9 = 12; // Added pins for 9 candidates
const int buzzer = 13; // Moved buzzer pin to 13

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;
int buttonState8 = 0;
int buttonState9 = 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
DFRobotDFPlayerMini dfPlayer; // Create a DFPlayer Mini object

int id = 0, previous_voter_id = 0, vote_taken = 0;
int party_1_count = 0, party_2_count = 0, party_3_count = 0, party_4_count = 0;
int party_5_count = 0, party_6_count = 0, party_7_count = 0, party_8_count = 0, party_9_count = 0; // Added counts for 9 candidates
String winner_name = "";

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP); // Enable internal pull-up resistors
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP); 
  pinMode(buttonPin5, INPUT_PULLUP); 
  pinMode(buttonPin6, INPUT_PULLUP); 
  pinMode(buttonPin7, INPUT_PULLUP); 
  pinMode(buttonPin8, INPUT_PULLUP); 
  pinMode(buttonPin9, INPUT_PULLUP); 

  lcd.init();  // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  mySerial.begin(57600); // Initialize Serial2 for fingerprint sensor
  mp3Serial.begin(9600); // Initialize Serial1 for DFPlayer Mini
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  
  lcd.clear(); 
  lcd.setCursor(0, 1);  
  lcd.print("  Smart Electronic"); 
  lcd.setCursor(0, 2);
  lcd.print("   Voting Machine");
  
  // Initialize DFPlayer Mini
  if (!dfPlayer.begin(mp3Serial)) {
    Serial.println("DFPlayer Mini not detected or unable to communicate.");
    while (true); // Stop if DFPlayer Mini is not detected
  }
  dfPlayer.volume(30); // Set volume (0-30)

  playMusic(2); // Play a welcome track at the start
  delay(5000); // Play the welcome track for 5 seconds
}

void loop() {
  vote_taken = 0;
  lcd.clear(); 
  lcd.setCursor(0, 1);  
  lcd.print(" Please place your"); 
  lcd.setCursor(0, 2);
  lcd.print("      finger");
  delay(100);

  id = getFingerprintIDez();
  if (id > 0) {
    playMusic(1);
    lcd.clear(); 
    lcd.setCursor(0, 0);  
    lcd.print("Your Voter ID"); 
    lcd.setCursor(0, 1);
    lcd.print(id);
    delay(2000);

    if (id == 10) { // Replace with the admin ID that can see results
      lcd.clear();
      lcd.setCursor(7, 0);  
      lcd.print("Results:");
      lcd.setCursor(0, 1);
      lcd.print("YML:"); lcd.print(party_1_count);
      lcd.setCursor(8, 1);
      lcd.print("NC:"); lcd.print(party_2_count);
      lcd.setCursor(15,1);
      lcd.print("MAO:"); lcd.print(party_3_count);
      lcd.setCursor(0, 2);
      lcd.print("SP:"); lcd.print(party_4_count);
      lcd.setCursor(8, 2);
      lcd.print("SAM:"); lcd.print(party_5_count);
      lcd.setCursor(15, 2);
      lcd.print("UM:"); lcd.print(party_6_count);
      lcd.setCursor(0, 3);
      lcd.print("JAN:"); lcd.print(party_7_count);
      lcd.setCursor(8, 3);
      lcd.print("ID1:"); lcd.print(party_8_count);
      lcd.setCursor(15, 3);
      lcd.print("ID2:"); lcd.print(party_9_count);
      delay(5000); // Display the results for 5 seconds
      
      winner_name = determineWinner();
      
      lcd.clear(); 
      lcd.setCursor(0, 0);  
      lcd.print("    Winner party"); 
      lcd.setCursor(6, 1);
      lcd.print(winner_name);
      while (1);
    }

    if (previous_voter_id != id) {
      do {
        lcd.clear(); 
        lcd.setCursor(5, 0);  
        lcd.print("Vote for:"); 
        lcd.setCursor(0, 1);
        lcd.print("1:YML  2:NC   3:MAO");
        lcd.setCursor(0, 2);
        lcd.print("4:SP   5:SAM  6:UM");
        lcd.setCursor(0, 3);
        lcd.print("7:JAN  8:ID1  9:ID2");
        delay(500);
        previous_voter_id = id;

        buttonState1 = digitalRead(buttonPin1);
        delay(10);
        buttonState2 = digitalRead(buttonPin2);
        delay(10);
        buttonState3 = digitalRead(buttonPin3);
        delay(10);
        buttonState4 = digitalRead(buttonPin4);
        delay(10);
        buttonState5 = digitalRead(buttonPin5);
        delay(10);
        buttonState6 = digitalRead(buttonPin6);
        delay(10);
        buttonState7 = digitalRead(buttonPin7);
        delay(10);
        buttonState8 = digitalRead(buttonPin8);
        delay(10);
        buttonState9 = digitalRead(buttonPin9);
        delay(10);

        if (buttonState1 == LOW) {
          party_1_count++;
          vote_taken = 1;
        } else if (buttonState2 == LOW) {
          party_2_count++;
          vote_taken = 1;
        } else if (buttonState3 == LOW) {
          party_3_count++;
          vote_taken = 1;
        } else if (buttonState4 == LOW) {
          party_4_count++;
          vote_taken = 1;
        } else if (buttonState5 == LOW) {
          party_5_count++;
          vote_taken = 1;
        } else if (buttonState6 == LOW) {
          party_6_count++;
          vote_taken = 1;
        } else if (buttonState7 == LOW) {
          party_7_count++;
          vote_taken = 1;
        } else if (buttonState8 == LOW) {
          party_8_count++;
          vote_taken = 1;
        } else if (buttonState9 == LOW) {
          party_9_count++;
          vote_taken = 1;
        } else {
          vote_taken = 0;
        }

        if (vote_taken == 1) {
          playMusic(5);
          lcd.clear(); 
          lcd.setCursor(0, 0);  
          lcd.print("  Thanks for your"); 
          lcd.setCursor(0, 1);
          lcd.print("      vote");
          delay(5000);
        }
      } while (vote_taken == 0);
    } else {
      // Stop the music when a duplicate vote is detected
      dfPlayer.stop();  // Stop the music

      lcd.clear(); 
      lcd.setCursor(0, 0);  
      lcd.print("   Duplicate Vote"); 
      lcd.setCursor(0, 2);
      lcd.print("  Buzzer ON");
      delay(2000);

      // Trigger the buzzer sound for duplicate vote
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzer, HIGH);
        delay(1000);
        digitalWrite(buzzer, LOW);
        delay(1000);
      }
    }
  }
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}

void playMusic(int trackNumber) {
  dfPlayer.play(trackNumber); // Play the track corresponding to the passed number
}

String determineWinner() {
  // Determine the party with the highest vote count
  int maxVotes = max(party_1_count, max(party_2_count, max(party_3_count, max(party_4_count, max(party_5_count, max(party_6_count, max(party_7_count, max(party_8_count, party_9_count))))))));
  
  if (maxVotes == party_1_count) return "YML";
  else if (maxVotes == party_2_count) return "NC";
  else if (maxVotes == party_3_count) return "MAO";
  else if (maxVotes == party_4_count) return "SP";
  else if (maxVotes == party_5_count) return "SAM";
  else if (maxVotes == party_6_count) return "UM";
  else if (maxVotes == party_7_count) return "JAN";
  else if (maxVotes == party_8_count) return "ID1";
  else return "ID2";
}
