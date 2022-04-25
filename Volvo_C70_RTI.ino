//pins definition
int ignitionPin = A7; // 10k/4k7 devider
int rpiPower = 3; // MOSFET
int rpiShutdown = 13; // Send shutdown signal to Rpi 
int displayOpened = 5; // Yellow cable
int displayClosed = 6; // Green cable
int openDisplay = 7; // Violet cable - IN2
int closeDisplay = 8; // Grey cable - IN1

int ledPin = 13;

//variables definition
int ignitionValue = 0;

int timerOn = 0;
int timerOff = 0;

void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(rpiPower, OUTPUT);
  pinMode(rpiShutdown, OUTPUT);
  pinMode(openDisplay, OUTPUT);
  pinMode(closeDisplay, OUTPUT);
  pinMode(displayClosed, INPUT_PULLUP);
  pinMode(displayOpened, INPUT_PULLUP);
  

  Serial.begin(9600);
}

void loop() {
  ignitionValue = analogRead(ignitionPin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = ignitionValue* (5.0 / 1023.0 * 3.0176); //
  Serial.print("Battery voltage: ");
  Serial.println(voltage);

  if ((digitalRead(displayClosed) == LOW) && (timerOn >= 1)) 
  {
    Serial.println("Openning dispaly");
    digitalWrite(rpiPower, HIGH);
    digitalWrite(openDisplay, HIGH);
    Serial.print("openDispalay: ");
    Serial.println(digitalRead(openDisplay));
    while (digitalRead(displayOpened) == HIGH)
    {
      delay(50);
    }
    delay(100);
    digitalWrite(openDisplay, LOW);
    Serial.print("openDisplay: ");
    Serial.println(digitalRead(openDisplay));
  } 
  if (((digitalRead(displayOpened) == LOW) || ((digitalRead(displayOpened) == HIGH) && (digitalRead(displayClosed) == HIGH))) && (timerOff >= 5))
  {
    Serial.println("Closing display");
    analogWrite(closeDisplay, 130);
    //digitalWrite(closeDisplay, HIGH);
    Serial.print("closeDisplay: ");
    Serial.println(digitalRead(closeDisplay));
    while (digitalRead(displayClosed) == HIGH)
    {
      delay(50);
    }
    delay(100);
    digitalWrite(closeDisplay, LOW);
    Serial.print("closeDisplay: ");
    Serial.println(digitalRead(closeDisplay));
    digitalWrite(rpiShutdown, LOW);
  } 
  



  Serial.print("end closeDisplay: ");
  Serial.println(digitalRead(closeDisplay));
  Serial.print("end openDisplay: ");
  Serial.println(digitalRead(openDisplay));
  Serial.print("end_displayClosed: ");
  Serial.println(digitalRead(displayClosed));
  Serial.print("end_displayOpened: ");
  Serial.println(digitalRead(displayOpened));
  
  if (voltage > 4)
  {
    timerOn++;
    timerOff = 0;
  }
  else
  {
    timerOff++;
    timerOn = 0;
  }

  if ((timerOff >= 20) && (timerOff < 25))
  {
    digitalWrite(rpiShutdown, HIGH); 
  }
  else if (timerOff >= 60)
  {
    digitalWrite(rpiShutdown, LOW); 
    digitalWrite(rpiPower, LOW); 
  }
  delay(1000);
}
