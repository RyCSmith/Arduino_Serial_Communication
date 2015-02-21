# Arduino_Serial_Communication
Overview: Program to perform basic interactions with Arduino. 

Concept: This project was assigned by Dr. Chris Murphy at the University of Pennsylvania and implemented by Ryan Smith, James Park and Rajveer Parikh.

Use: arduino_program.ino is loaded onto Arduino. It creates a timer and sends updates every 10 seconds. Arduino.c establishes a connection with the device and operates separate threads. Thread 1 receives incoming data from the device, constructs the desired message and prints. Thread 2 allows the user to interact with the board.

-Enter 'a' to turn the lights on the Arduino on.

-Enter 'b' to turn the lights on the Arduino off.

-Enter 'q' to close the connection and quit.

Other Info: Makefile included.