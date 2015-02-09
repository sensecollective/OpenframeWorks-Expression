int incoming;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  while (Serial.available()) {
    char readBytes = Serial.read();
    if (readBytes == '0') {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  }

}
