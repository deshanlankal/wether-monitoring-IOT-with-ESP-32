#define RAIN_SENSOR_PIN 34 // GPIO pin connected to the rain sensor's AO pin

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  Serial.println("Rain Sensor Test - ESP32");
}

void loop() {
  // Read the analog value from the rain sensor
  int rainValue = analogRead(RAIN_SENSOR_PIN);

  // Convert the value to a percentage (assuming 12-bit ADC, 0-4095 range)
  float rainPercentage = (rainValue / 4095.0) * 100.0;

  // Print the readings to the Serial Monitor
  Serial.print("Rain Sensor Value: ");
  Serial.print(rainValue);
  Serial.print(" | Rain Percentage: ");
  Serial.print(rainPercentage);
  Serial.println(" %");

  // Wait for a short delay before reading again
  delay(1000);
}