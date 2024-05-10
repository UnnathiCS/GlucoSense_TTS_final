#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define RX_PIN 10
#define TX_PIN 11
#define SENSOR_PIN A0

SoftwareSerial df3Serial(RX_PIN, TX_PIN);
DFRobotDFPlayerMini dfPlayer;

void setup() {
  Serial.begin(9600);  // Initialize serial communication for printing values
  df3Serial.begin(9600);
  
  if (!dfPlayer.begin(df3Serial)) {
    Serial.println("DFPlayer Mini not found");
    while (true)
      ;
  }

  dfPlayer.volume(100);  // Set volume
}

void loop() {
  // Read the analog value from the sensor pin
  int sensorValue = analogRead(SENSOR_PIN);
  // Convert analog value to voltage (0-5V)
  float voltage = sensorValue * (5.0 / 1023.0);
  // Calculate glucose level
  float glucose = 0.966 * voltage + 25.8;
  // Round off glucose level to the nearest integer
  int roundedGlucose = round(glucose);

  // Print the glucose value to the serial monitor
  Serial.print("Your glucose level is: ");
  Serial.println(roundedGlucose);
  
  // Play the 13th file

  dfPlayer.play(13);
  delay(2000);  // Delay for 2 seconds



  // Array to store individual digits
  int digits[10];
  int numDigits = 0;  // Number of digits in the glucose level

  // Extract individual digits of the rounded glucose level
  while (roundedGlucose > 0) {
    int digit = roundedGlucose % 10;  // Extract the least significant digit
    digits[numDigits++] = digit;      // Store the digit in the array
    roundedGlucose /= 10;             // Remove the least significant digit
  }

  // Play the corresponding tracks for each digit of the rounded glucose level
  for (int i = numDigits - 1; i >= 0; i--) {

    Serial.println(digits[i] + 1);
    dfPlayer.play(digits[i] + 1);  // Play the corresponding track
    delay(2000);                   // Delay between plays
  }

  // Delay between readings (optional, adjust as needed)
  delay(1000);  // Delay for 1 second
}
