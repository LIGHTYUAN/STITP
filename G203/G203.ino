int buttonpin = 2;
int LED = LED_BUILTIN;
int val;
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  // pinMode(buttonpin, INPUT);
  attachInterrupt(0, Blink, CHANGE);
}

void loop() {
  val = digitalRead(buttonpin);
  if(val == LOW){
    Blink();
  }else{
    digitalWrite(LED, LOW);
  }
}

void Blink()
{
  Serial.println("Hello");
  noInterrupts();
  for(int i=0; i<5; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);   
  }
  Serial.println("World");
}
