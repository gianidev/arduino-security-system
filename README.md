# GSM Arduino-based security system

This is a security system based on Arduino that has the ability to detect motion and send alerts through text messages (SMS). This device also allows users to remotely control it via SMS to activate or deactivate it as needed. In addition, the system includes other electronic components such as LED lights and a buzzer that are activated or deactivated depending on the detection of motion while in operation. 

You can find some photos of my prototye [here](https://imgur.com/a/BZvCLkA).

## Usage

* Upload the `gsm-alarm-system.ino` file to your Arduino board. For this you can use [Arduino IDE](https://www.arduino.cc/en/software).
* Connect the circuits as shown in the diagram below.
* Run the arduino code.

## Requirements
* Arduino UNO
* Protoboard 
* LEDs
* Resistors 220 ohm
* SIM800L GSM/GPRS module 
* SIM Card
* Active Buzzer
* Li-Ion 3.7V 7800 mAh as Power Supply
* Jumper Cables

## Circuit Design
![imagen](https://user-images.githubusercontent.com/36320743/233227254-6ec9f7a0-f780-410f-b711-1d1977b32d5b.png)

## Functionalities

When the device is turned on, a green LED will illuminate to indicate that the system is on alert. Equipped with a SIM800L module capable of sending and receiving messages, this prototype detects motion through its infrared sensor and sends a text message alerting of possible intruders with the message "ALERT - Intruder detected". At the same time, the red LED will light up, and the buzzer will sound until the user turns off the device.

Users can interact with the device using SMS commands, including "OFF" to turn off the buzzer and LEDs. After this, the device will no longer detect movement until the user restarts the Arduino or sends the "ON" command. When the "ON" command is received, the green LED will light up, and the system will be on alert for possible movements.

Upon receiving these commands, the Arduino will send a message informing the user of the current state of the alarm, such as "Alarm on" or "Alarm off".

