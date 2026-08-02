#include <LiquidCrystal.h>
#include <Servo.h>

Servo servo;

const int rs = 32, en = 34, d4 = 44, d5 = 46, d6 = 48, d7 = 50;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int button = 49;
int count = 0;
int SW_pin = 52;
int X_pin = A0;
int Y_pin = A1;
double pos = 0;
int servoPin = 2;
int turretLED = 39;
int firstLED = 42;
int buzzer = 28;
int turret = false;

int triggerPin = 26;
int echoPin = 24;
float speedOfSound = 339.2;
float conversionFactor = 0.0001;
unsigned int pingTime = 0;
unsigned int pingTimeAvg = 0;

int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  // 50 ms

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("DISTANCE");
  lcd.setCursor(10, 0);
  lcd.print("MODE");                                                                          
  lcd.setCursor(8, 1);
  lcd.print("PANNING");
  pinMode(button, INPUT_PULLUP);
  pinMode(firstLED, OUTPUT);
  pinMode(turretLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  digitalWrite(firstLED, HIGH);
  digitalWrite(turretLED, LOW);
  digitalWrite(buzzer, LOW);
}

float distance(){
  float dist = 0.0;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pingTime = pulseIn(echoPin, HIGH, 18000);
  float dist2;
  float max = 0;
  int num = 30;
  while (num>0) {
    dist2 =  speedOfSound * conversionFactor * pingTime/2;
    if (dist2 > max) {
      max = dist2;
    }
    num--;

  }
  dist = max;
  return dist;
}


bool buttonPress() {
  bool pressed = false;
  int currentButtonState = digitalRead(button);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      pressed = true;
    }
  }
  lastButtonState = currentButtonState;
  return pressed;
}

float joystickangle = 0;

void turretMode(){
  lcd.setCursor(8, 1);
  lcd.print("TRACKING");
  digitalWrite(firstLED, LOW);
  digitalWrite(turretLED, HIGH);
  if (analogRead(X_pin) > 520 || analogRead(Y_pin) > 520 || analogRead(X_pin) < 490 || analogRead(Y_pin) < 490){
    float xpos = float(analogRead(Y_pin))-515.0;
    float ypos = float(analogRead(X_pin))-507.0;
    float joystickangle = atan(ypos/xpos)*180/PI+90;
    
    if (joystickangle > pos && analogRead(Y_pin)<=510){
      pos = pos + 0.7;
    }
    else if(joystickangle < pos && analogRead(Y_pin)<=510){
      pos = pos - 0.7;
    }
    servo.write(pos);
    // Serial.print(analogRead(Y_pin));
    // Serial.print(",");
    // Serial.print(analogRead(X_pin));
    // Serial.print("\n");
    }
    Serial.print(abs(180-pos));
    
}
void leftRightMode(){
  lcd.setCursor(8, 1);
  lcd.print("PANNING ");
  digitalWrite(firstLED, HIGH);
  digitalWrite(turretLED, LOW);
  if (analogRead(X_pin)-500 > 10 || 500 - analogRead(X_pin) > 20) {
    pos = pos + (500.0 - double(analogRead(X_pin)))/1200.0;
    servo.write(pos);
  }
  Serial.print(abs(180-pos));
}

void loop() {
  for (int i = 0; i >= 0; i++){
    if (turret == false){
    if (buttonPress()) {
      turret = true;
      turretMode();
    } else {
      leftRightMode();
    }
  }else if (turret == true){
    if (buttonPress()){
      turret = false;
      leftRightMode();
    }else {
      turretMode();
    }
  }
  if (pos > 180){
    pos = 180;
  }
  if (pos < 0){
    pos = 0;
  }

      // Serial.print(joystickangle);
  // Serial.print(",");
  // Serial.print(pos);
  Serial.print(",");
  float d = distance();
  Serial.print(d);
  Serial.print("\n");
  delay(10);
  // Serial.print("847987013740342758072380473280568037480173580326804710274801375");
  // Serial.print(turret);
    if (i % 50 == 0){
      lcd.setCursor(1, 1);
      lcd.print(d);
      if (d <= 10.0 && d != 0.00000){
        // Serial.print("ASDLFJADSIOAEWIOAWIOERUIOAEWUIROWUERIOOI");
        digitalWrite(buzzer, HIGH);
      }else{
        digitalWrite(buzzer, LOW);
      }
      }
    }
  }

