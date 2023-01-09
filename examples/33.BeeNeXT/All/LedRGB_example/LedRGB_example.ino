
#define PWM_FREQ        2000
#define PWM_CHANNEL     2
#define PWM_RESOLUTION  8

const int led = 4;
void setup()
{
  //Initialize GPIO, turn off tricolor light

  pinMode(4, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // set PWM_CHANNEL
  //ledcAttachPin(led, PWM_CHANNEL);  // Connect the PWM_CHANNEL to the corresponding pin
}

void loop()
{
  digitalWrite(4,  LOW);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(500);
  digitalWrite(4,  HIGH);
  digitalWrite(16, LOW);
  digitalWrite(17, HIGH);
  delay(500);
  digitalWrite(4,  HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, LOW);
  delay(500);
  digitalWrite(4,  HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(500);
  
  ledcAttachPin(led, PWM_CHANNEL);
  // gradually brighten
   for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle = dutyCycle - 5)
  {
    ledcWrite(PWM_CHANNEL, dutyCycle);  // output PWM
    delay(100);
  }
  // gradually darken
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 5)
  {
    ledcWrite(PWM_CHANNEL, dutyCycle);  // output PWM
    delay(100);
  }
  delay(500);
}


