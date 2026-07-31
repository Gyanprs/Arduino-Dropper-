#include <Servo.h>

//pins
const int PlayButton = 3;      
const int BuzzerPin = 2;        
const int CalibratePin = 4;    

const int ServoPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
Servo servos[8];

int lastPlayState = LOW;
int lastCalibrateState = HIGH;

void setup() {
  for (int i = 0; i < 8; i++) {
    servos[i].attach(ServoPins[i]);
    if (i < 4)
      servos[i].write(0);
    else
      servos[i].write(180);
  }

  pinMode(BuzzerPin, OUTPUT);      
  pinMode(PlayButton, INPUT);      
  pinMode(CalibratePin, INPUT_PULLUP);

  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  int currentPlay = digitalRead(PlayButton);
  int currentCalibrate = digitalRead(CalibratePin);


  if (currentPlay != lastPlayState) {
    if (currentPlay == HIGH)
      Serial.println("::Game on");
    else
      Serial.println("::Game off");
      beepBuzzer(1);
  }

  if (currentCalibrate != lastCalibrateState) {
    if (currentCalibrate == LOW)
      Serial.println("::Calibrating");
    else
      Serial.println("::Calibration off");
  }

  // main game button
  if (currentPlay == HIGH && lastPlayState == LOW) {
    Serial.println("3");
    beepBuzzer1(1);
    Serial.println("2");
    beepBuzzer1(1);
    Serial.println("1");
    beepBuzzer1(1);

    if (random(0, 2) == 0) {
      Serial.println(">normal Mode");
      runRandomServoSequence(2000, 4000);
    } else {
      Serial.println(">crazy mode");
      runRandomServoSequence(1000, 1500);
    }
  }

  // calibration mode
  if (currentCalibrate == LOW && lastCalibrateState == HIGH) {
    beepBuzzer(1);
    Serial.println("Entering Calibration");
    enterCalibrationMode();
  }

  lastPlayState = currentPlay;
  lastCalibrateState = currentCalibrate;

  delay(15);
}

void beepBuzzer1(int times) {
 for (int i = 0; i < times; i++) {
    tone(BuzzerPin, 800);
    delay(600);
    noTone(BuzzerPin);      
    delay(400);
  }
}

void beepBuzzer(int times) {
 for (int i = 0; i < times; i++) {
    tone(BuzzerPin, 1000);
    delay(300);
    noTone(BuzzerPin);      
    delay(400);
  }
}

void runRandomServoSequence(int minDelay, int maxDelay) {
  int order[8];
  generateRandomOrder(order, 8);

  for (int i = 0; i < 8; i++) {
    int servoIndex = order[i];

    servos[servoIndex].write(90);
    delay(random(minDelay, maxDelay));

    if (servoIndex < 4)
      servos[servoIndex].write(0);
    else
      servos[servoIndex].write(180);

    delay(200);
  }

  resetServos();
}

void enterCalibrationMode() {
  Serial.println("Calibration mode active.");

  while (digitalRead(CalibratePin) == LOW) {
    for (int i = 0; i < 8; i++)
      servos[i].write(90);
    delay(10);
  }

  Serial.println("Calibration mode exited.");
  resetServos();
}

void resetServos() {
  for (int i = 0; i < 8; i++) {
    if (i < 4)
      servos[i].write(0);
    else
      servos[i].write(180);
  }
}

void generateRandomOrder(int *arr, int n) {
  for (int i = 0; i < n; i++) arr[i] = i;

  for (int i = n - 1; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
