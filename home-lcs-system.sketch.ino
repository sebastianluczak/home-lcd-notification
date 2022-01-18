// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int LED_ONE_PIN = 44;
const int LED_TWO_PIN = 46;

bool LED_ONE_ENABLED = false;
bool LED_TWO_ENABLED = false;

int val = 0;  // variable to store the value read
int ANALOG_PIN = A0; // potentiometer wiper (middle terminal) connected to analog pin 3

String x = "";         // a String to hold incoming data
String inputString = "";         // a String to hold incoming data

bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);

  pinMode(LED_ONE_PIN, OUTPUT);
  pinMode(LED_TWO_PIN, OUTPUT);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("-- Stoper --");
}

void loop() {
  if (stringComplete) {
    lcd.setCursor(0, 0);
    lcd.print(inputString);
    lcd.print("           ");
    lcd.setCursor(0, 1);
    // routing logic here
    Serial.println(inputString);
 
    LED_ONE_ENABLED = true;
    analogWrite(LED_ONE_PIN, 255);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  val = analogRead(ANALOG_PIN);
  if (val == 204) {
      lcd.setCursor(0, 0);
      lcd.print("Lap time: ");
      lcd.print(millis()/1000);
      lcd.print(".");
      lcd.print(millis()%1000);
      lcd.setCursor(0, 1);
  } else if (val == 622) {
    LED_ONE_ENABLED = true;
    analogWrite(LED_ONE_PIN, 255);
  } else if (val == 0) {
    LED_ONE_ENABLED = false;
    analogWrite(LED_ONE_PIN, 0);
  } else if (val == 822) {
    LED_TWO_ENABLED = true;
    
  }
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  if (millis()/10000000000 % 2 == 0) {
    if (LED_ONE_ENABLED) {
      float pwrReq = (millis() % 1000) / 10;
      analogWrite(LED_ONE_PIN, pwrReq);
    }
    if (LED_TWO_ENABLED) {
      analogWrite(LED_TWO_PIN, random(100));
      lcd.setCursor(0, 0);
      lcd.print("K6 Dice Roll: ");
      lcd.print(random(6));
      lcd.setCursor(0, 1);
      LED_TWO_ENABLED = false;
    }
  }
  printTimeFromMilliTime();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    lcd.setCursor(0, 0);
    lcd.println(inputString);
    lcd.setCursor(0, 1);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

String printTimeFromMilliTime()
{
  millis();
  lcd.print(".");
  lcd.print(millis() % 1000);
}
