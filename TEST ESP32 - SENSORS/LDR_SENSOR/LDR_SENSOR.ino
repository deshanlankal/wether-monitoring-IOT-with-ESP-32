#define LDR_PIN 26 

void setup() {
  
  Serial.begin(9600);
  Serial.println("Testing Digital LDR Sensor on GPIO 26");

  
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  // Read the state of the LDR sensor
  int ldrState = digitalRead(LDR_PIN);

  bool isLightDetected = (ldrState == HIGH); 

  // Print the result to the Serial Monitor
  Serial.print("Light Detected: ");
  Serial.println(isLightDetected ? "False" : "True");

  delay(500); 
}
