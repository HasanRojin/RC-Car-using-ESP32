#include <Arduino.h>  // Ensure core ESP32 functions are included
#include <DabbleESP32.h>

// Define motor control pins
#define ENA 12
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 14

void setup() {
    Serial.begin(115200);
    Dabble.begin("Barcode_RC_Car");  // Bluetooth name

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    Dabble.processInput();

    if (GamePad.isUpPressed()) {
        moveForward();
    } else if (GamePad.isDownPressed()) {
        moveBackward();
    } else if (GamePad.isLeftPressed()) {
        turnLeft();
    } else if (GamePad.isRightPressed()) {
        turnRight();
    } else {
        stopCar();
    }
}

void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
}

void moveBackward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
}

void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void stopCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
