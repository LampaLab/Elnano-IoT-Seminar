#define LED_PIN D0

void setup() {
  pinMode(LED_PIN, OUTPUT);     // Initialize the LED_PIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_PIN, LOW);   // Turn the LED off
  delay(1000);                      // Wait for a second
  digitalWrite(LED_PIN, HIGH);  // Turn the LED on by making the voltage HIGH
  delay(2000);                      // Wait for two seconds
}
