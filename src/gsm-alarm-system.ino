#include <SoftwareSerial.h>

SoftwareSerial sim800l(2, 3);

int sensorPir = 8;
int buzzer = 9;
int ledRed = 10;
int ledGreen = 11;

boolean inAlert = true; // This variable works as a flag to know if the system should be aware of movements
boolean intruderDetected = false; // This variable represents whether or not an intruder has been detected.

char messageReceived; // Here the content of the SMS received by the SIM800L will be saved

// If the received messages do not contain "ON" or "OFF", then the next two variables will have the value "-1"
short ON_OK = -1;
short OFF_OK = -1;

void setup() {
  Serial.begin(9600); // // Start serial communication between the Arduino and the IDE (Serial Monitor)
  sim800l.begin(9600); // The serial communication of the Arduino and the SIM800L module is started

  Serial.println("Starting...");

  delay(3000); // Delay to allow the SIM800L module to connect to the network
  configSIM800L(); // This method configures the SIM800L module so that it can receive SMS

  pinMode(sensorPir, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {

  if (inAlert) {
    // If the system is alert and ready to detect movements, the green led turns on
    digitalWrite(ledGreen, HIGH);
  }

  if (digitalRead(sensorPir) == HIGH &&
    inAlert &&
    !intruderDetected) {
    // If the pir sensor detects movement, the system is on alert and no intruders have been previously detected
    // send an alert message, turn on the buzzer and red led
    intruderDetected = true;
    sendMessage("ALERT - Intruder Detected");
    digitalWrite(ledRed, HIGH);
    digitalWrite(buzzer, HIGH);
  }

  String message; // In this variable the content of the sms is saved as String

  while (sim800l.available() > 0) {
    // If the SIM800L received something, it will enter the loop

    messageReceived = sim800l.read(); // "messageReceived" contains the received SMS
    Serial.println(messageReceived); // It is shown in the message received in the Serial Monitor  
    message.concat(messageReceived); // Concatenates "messageReceived" to the empty String "message"

    OFF_OK = message.indexOf("OFF"); // The indexOf function returns an int with the position in which a substring is located, in this case "ON" and "OFF"
    ON_OK = message.indexOf("ON"); // If it returns -1 it would mean that the received message did not contain "ON"/"OFF"
  }

  if (OFF_OK != -1) {
    // If the message contained the text "OFF"

    Serial.println("Alarm functionality is turned off"); // It is displayed in the message in the Serial Monitor

    inAlert = false; // The system is no longer on alert
    intruderDetected = false;

    // The leds and the buzzer go off
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledGreen, LOW);

    sendMessage("Alarm OFF"); // Send an SMS informing that the alarm has gone off
    configSIM800L(); // The SIM800L module is set up to receive SMS

    OFF_OK = -1;
    ON_OK = -1;
  }

  if (ON_OK != -1) {
    //If the message contained the text "ON"

    Serial.print("Alarm functionality is turned on"); //It is displayed in the message in the Serial Monitor

    inAlert = true; // The system is on alert again, waiting to receive signals from the pir sensor.
    intruderDetected = false;

    sendMessage("Alarm ON"); // Send an SMS informing that the alarm has been turned on
    configSIM800L(); // The SIM800L module is set up to receive SMS

    OFF_OK = -1;
    ON_OK = -1;
  }
}

void comSerial() {
  //Auxiliary method used by configSIM800L()

  delay(500);
  while (Serial.available()) {
    sim800l.write(Serial.read());
  }
  while (sim800l.available()) {
    Serial.write(sim800l.read());
  }
}

void configSIM800L() {
  // Configure the SIM800L module so that it can receive SMS

  sim800l.println("AT"); // If everything is ok, it will show "OK" on the serial monitor
  comSerial();
  sim800l.println("AT+CMGF=1"); // Set up the module to send or receive SMS
  comSerial();
  sim800l.println("AT+CNMI=2,2,0,0,0"); // Configure the module so that it can handle received SMS
  comSerial();
}

void sendMessage(String msg) {
  // Method to send messages via sms

  Serial.println("Enviando message..."); // Display a message on the Serial monitor
  sim800l.print("AT+CMGF=1\r"); // Set up the module to send or receive SMS
  delay(100);
  sim800l.print("AT+CMGS=\"+XXXYYYYYYY\"\r"); // Your phone number, you need to include include your country code 
  delay(500);
  sim800l.print(msg); // This String will be sent via SMS
  delay(500);
  sim800l.print((char) 26); // Tells the module that it can send the SMS
  delay(500);
  sim800l.println();
  Serial.println("message enviado."); // Display a message on the Serial monitor
  delay(500);
}
