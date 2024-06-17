const int potPin = 39;
const int ledPin = 26; 

int potValue;
float voltage = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  potValue = analogRead(potPin);
  voltage = (5/4095.0) * potValue +(20)/14 ;
 
  Serial.print("potValue: ");
  Serial.print(potValue);
  
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.println("V");   
  
  if (voltage < 3.4) {
    digitalWrite(ledPin, HIGH); 
    Serial.print("LED on"); 
  } 
  else if (voltage >= 3.4) {
    digitalWrite(ledPin, LOW);
 
  }
  delay(100); 
}
