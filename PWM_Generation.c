// C code
// 2023-07-30 
// Initial version : PWN generation using Arduino, follow the two different check version 1.0.0.0

const int pwmOutputPin = 9;
const int potentiometerPin = A0;
const int minPotValue = 0;
const int maxPotValue = 5;
const int minDutyCycle = 64;
const int maxDutyCycle = 128;
  
unsigned long prevTime = millis();

void setup()
{
    // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potentiometerPin, INPUT_PULLUP);
  pinMode(pwmOutputPin, OUTPUT);
}
void MainCheck()
{
  //Perform maincheck, break the execution if something broken.
  
}

void FeedbackCheck()
{
  //Feedback check, break the execution if something broken.
}

void GeneratePWM()
{
  /*Subtask1: Read DC input from 0 to 5*/
  int potValue = analogRead(potentiometerPin);
  int capPotValue = map(potValue,0,1024,minPotValue,maxPotValue);  
  int dutyCycle = map(capPotValue,0,5,minDutyCycle,maxDutyCycle);  
  
  Serial.print("potValue (0-5): ");
  Serial.println(capPotValue);
  Serial.print("dutyCycle (64-128): ");
  Serial.println(dutyCycle);
  
  
  //Generate the PWM
  analogWrite (pwmOutputPin, dutyCycle);  //generates pwm of 50% duty cycle
    
  
}

void loop()
{
  unsigned long currentTime = millis();
  
  /*Task1 : Main check*/
  MainCheck();
  
  /*Task2: Feedback signal check*/
  FeedbackCheck();
  
  
  /*Task3: Generate the PWM signal every 10ms*/
  if (currentTime - prevTime > 10) {
    GeneratePWM();
    prevTime = currentTime;
  }
  
  
}
