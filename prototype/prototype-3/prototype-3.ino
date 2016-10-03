void setup() {  
  // put your setup code here, to run once:
  Serial.begin(9200);
  
  pinMode(2, INPUT);
  
  pinMode(3, INPUT);

  pinMode(12, OUTPUT);

  attachInterrupt(0, int2, CHANGE); // 0 is interrupt number, listens at pin 2
  attachInterrupt(1, int3, CHANGE); // 1 is interrupt number, listens at pin 3

  digitalWrite(12, HIGH);
}

int count = 0;
void int2() {
  // Serial.println("Int2");
}

void int3() {
  count++;
  if (count == 360) {
    count = 0;
    digitalWrite(12, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count);
}
