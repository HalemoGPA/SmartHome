#define ldr A0
#define lamb 4
#define fan 7
void setup() {
  pinMode(lamb,OUTPUT);
  pinMode(fan,OUTPUT);
  digitalWrite(fan,HIGH);
  Serial.begin(9600);
  
}

void loop() {
  int reading = analogRead(ldr);
  Serial.println(reading);
  if (reading > 700)
  {
    digitalWrite(lamb,LOW);
  }
  else 
  {
    digitalWrite(lamb,HIGH);
  }

}
