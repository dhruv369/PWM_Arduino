// C code
// 2023-07-30 
// Initial version : PWN generation using Arduino, follow the two different check version 1.0.0.0

#define CPU_FREQ (16000000)
#define CLOCK_CYCLE_TIME (0.0625 ) //usecs


const int pwmOutputPin = 9;
const int potentiometerPin = A0;
const int mainOkPin = A1;
const int feedbackOkPin = A2;
const int minPotValue = 0;
const int maxPotValue = 5;
const int minDutyCycle = 64;
const int maxDutyCycle = 128;
  
unsigned long prevTime = millis();

void CalculateCpuLoad(unsigned long exeTime)
{
  double cpuLoad;
  /*CPU Load (%) = (Execution Time(us) / Clock Cycle Time(nsec)) * 100
    Clock Cycle Time = 1 second / Clock Frequency
                 = 1 second / 16,000,000 Hz
                 = 0.0000000625 seconds
                 = 62.5 nanoseconds
    */
 cpuLoad = ( exeTime  / CLOCK_CYCLE_TIME) * 100;
  
  Serial.print("cpuLoad (%): ");
  Serial.println(cpuLoad);
  
}

void setup()
{
    // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(mainOkPin, INPUT_PULLUP);  pinMode(mainOkPin, INPUT_PULLUP);

  pinMode(feedbackOkPin, INPUT_PULLUP);
  pinMode(potentiometerPin, INPUT_PULLUP);
  pinMode(pwmOutputPin, OUTPUT);
}
unsigned char MainCheckRoutine()
{
  //Perform maincheck, break the execution if something broken.
   int readMainOkValue = analogRead(mainOkPin);
	unsigned char mainOkValue = map(readMainOkValue,0,1023,0,1); 
  
  Serial.print("mainOkValue (On/Off): ");
  Serial.println(mainOkValue);
  
  
  //check if everthing is ok
  if(mainOkValue)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  
}

unsigned char FeedbackCheckRoutine()
{
  //Feedback check, break the execution if something broken.
    int readFeedbackOkValue = analogRead(feedbackOkPin);
    char feedbackOkValue = map(readFeedbackOkValue,0,1023,0,1); 
  
   Serial.print("feedbackOkValue (On/Off): ");
  Serial.println(feedbackOkValue);
 
    //check if everthing is ok 
  	if(feedbackOkValue)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  
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
  
  unsigned long startTime, endTime, executionTime;
  unsigned char mainCheck_flag = 0;
  unsigned char feedbackCheck_flag = 0;
  // Record the start time
  startTime = micros();
  
  unsigned long currentTime = millis();
  /*Task1 : Main check*/
  mainCheck_flag = MainCheckRoutine();
  
  /*Task2: Feedback signal check*/
  feedbackCheck_flag = FeedbackCheckRoutine();
  
  
  /*Task3: Generate the PWM signal every 10ms*/
  if(mainCheck_flag & feedbackCheck_flag)
  {
    GeneratePWM();
    //if (currentTime - prevTime > 10) {
    //  GeneratePWM();
    //  prevTime = currentTime;
    //}
  }  
  // Record the end time
  endTime = micros();
// Calculate the execution time
  executionTime = endTime - startTime;

  // Output the execution time to the serial monitor
  Serial.print("Execution Time: ");
  Serial.print(executionTime);
  Serial.println(" microseconds");
  
  CalculateCpuLoad(executionTime);
}
