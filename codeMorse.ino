// ─────────────────────────────────────────────
// Morse Code Generator 
// LED + Buzzer + Button
// ─────────────────────────────────────────────

// Pin definitions
int ledPin = 10;      // LED connected to digital pin 10
int buzzer = 6;       // Buzzer connected to pin 6
int buttonPin = 2;    // Push button connected to pin 2
#define SDA A4
#define SCL A5
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   //Library

LiquidCrystal_I2C lcd(0x27,16,2);  //set the LCD address to 0x27 for a 16 chars and 2 line display

// Message to send in Morse code
String message = "SOS";

// Morse code for letters A-Z
String morseAlphabet[26] = {
".-",   //A
"-...", //B
"-.-.", //C
"-..",  //D
".",    //E
"..-.", //F
"--.",  //G
"....", //H
"..",   //I
".---", //J
"-.-",  //K
".-..", //L
"--",   //M
"-.",   //N
"---",  //O
".--.", //P
"--.-", //Q
".-.",  //R
"...",  //S
"-",    //T
"..-",  //U
"...-", //V
".--",  //W
"-..-", //X
"-.--", //Y
"--.."  //Z
};

// Morse code for numbers 0-9
String morseNumbers[10] = {
"-----",//0
".----",//1
"..---",//2
"...--",//3
"....-",//4
".....",//5
"-....",//6
"--...",//7
"---..",//8
"----."//9
};

// Short signal (dot)
void dot() {
digitalWrite(ledPin, 1);  // Turn LED ON
tone(buzzer,0 );          // Start buzzer sound
delay(150);             // Keep ON for duration
digitalWrite(ledPin, 0);   // Turn LED OFF
noTone(buzzer);              // Stop buzzer
delay(100);                  // Gap between signals ;               // 150 ms ON time
}

// Long signal (dash)
void dash() {
digitalWrite(ledPin, 1);  // Turn LED ON
tone(buzzer,0 );          // Start buzzer sound
delay(400);             // Keep ON for duration
digitalWrite(ledPin, 0);   // Turn LED OFF
noTone(buzzer);              // Stop buzzer
delay(100);                  // Gap between signals (PDF standard);               // 400 ms ON time
}

// ─── PLAY MORSE ───
// Plays a Morse sequence like ".-"
void playMorse(String code) {
for (int i = 0; i < code.length(); i++) {
if (code[i] == '.') {
dot();                   // Short signal
} else if (code[i] == '-') {
dash();                  // Long signal
}
}
}
// ─── SEND CHARACTER ───
// Converts one character to Morse and plays it
void sendCharacter(char c) {
// Space between words
if (c == ' ') {
delay(700);                // Word gap (PDF)
return;
}
// Letters A-Z
if (c >= 'A' && c <= 'Z') {
playMorse(morseAlphabet[c - 'A']);
}

// Numbers 0-9
else if (c >= '0' && c <= '9') {
playMorse(morseNumbers[c - '0']);
}
delay(100);                  // Gap between letters (PDF)
}
// ─── SETUP ───
// Runs once at startup
void setup() {
pinMode(ledPin, OUTPUT);        // Set LED as output
pinMode(buzzer, OUTPUT);        // Set buzzer as output
pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up

Serial.begin(9600);             // Start serial communication
Serial.println("Enter message:");
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  // Clear the LCD screen
lcd.clear();

// Move cursor to first row, first column
lcd.setCursor(0, 0);

// Display message on LCD
lcd.print("System Ready");
}

// ─── LOOP ───
// Runs continuously
void loop() {

  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {

    Serial.println("Sending: " + message);

    // Show "Sending" on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sending:");

    // Show message on LCD
    lcd.setCursor(0, 1);
    lcd.print(message);

    // Send message one character at a time
    for (int i = 0; i < message.length(); i++) {
      sendCharacter(message[i]);
    }

    // Wait 1 seconds
    delay(1000);
  }

  // Check if Serial data is available
  if (Serial.available() > 0) {

    // Read message until new line
    message = Serial.readStringUntil('\n');

    // Remove spaces
    message.trim();

    // Make text uppercase
    message.toUpperCase();

    Serial.println("New message: " + message);

    // Show new message on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New Msg:");

    lcd.setCursor(0, 1);
    lcd.print(message);
  }
}