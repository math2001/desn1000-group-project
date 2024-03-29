void setup()
{
    Serial.begin(9600);
    Serial.println("Echo server");
    Serial.println("===========");
}

void loop()
{
    while (Serial.available()) {
        Serial.write(Serial.read());
    }
}