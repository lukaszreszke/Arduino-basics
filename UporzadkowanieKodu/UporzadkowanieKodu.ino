/* USED PINS:
    2,3,6,8,9,10,11
    FREE PINS:
    7,4
*/
int encoder1 = 2;
int encoder2 = 3;
int encoder3 = 0;
int encoder4 = 1;

//M1 counters
int counterCW;
int counterCW_p = 0;
int counterCCW;
int counterCCW_p = 0;

//M2 counters
int counterCWM2;
int counterCW_pM2 = 0;
int counterCCWM2;
int counterCCW_pM2 = 0;

// Sensor's for JoyStick
int sensorX = A0;
int sensorY = A1;

int pumpVoltage = A2;
int pumpB = 13;

int refSpeedM1 = 6; // pin that is used for setpointing Motor1 reference speed
int refSpeedM2 = 5;

int getJoy = 0; // Reads analog value from JoyStick

//int motorSpeedMap; // -----------------------> THIS MIGHT BE USED FOR MAPPING -----------------> RIGHT NOW ITS USELESS

// Variables for counting time
unsigned long start;
unsigned long finish;
unsigned long dif;
unsigned long difM2;

float motorSpeed; // Variable that is used to count motor speed in rpm
float motorSpeedM2;

// Variables used for flaging directions (CW or CCW)
boolean goingUp = false;
boolean goingDown = false;
boolean goingUpM2 = false;
boolean goingDownM2 = false;

boolean pumpFlag = false;

// Flag used for counting motor speed, it's deterined by motor direction.
boolean difFlag = false;
boolean difFlagM2 = false;

int M1_A = 10; // Motor M1 at pin 10 and 11
int M1_B = 11;
int M2_A = 9; // Motor M2 at pin 9 and 8
int M2_B = 8;

int pumpOn = 7;
/*ONE ENCODER RESOLUTION IS 16 IMPULSES */
/*ONE MOTOR RESOLUTION IS 300 IMPULSES */

void setup()
{
  pinMode(pumpB, OUTPUT);
  digitalWrite(pumpB, LOW);
  
  pinMode(pumpVoltage,OUTPUT);
  analogWrite(pumpVoltage, 0);

  //Initialization of pin's
  pinMode(M1_A, OUTPUT);
  pinMode(M1_B, OUTPUT);
  pinMode(M2_A, OUTPUT);
  pinMode(M2_B, OUTPUT);
 
  pinMode(refSpeedM1, OUTPUT);
  //digitalWrite(M1_A, HIGH);
  //digitalWrite(M1_B, LOW);
  //digitalWrite(M2_A,HIGH);
  //digitalWrite(M2_B,LOW);
  counterCW = 0;
  counterCCW = 0;
  counterCWM2 = 0;
  counterCCWM2 = 0;
  //Serial prints for debugging and testing
  Serial.begin(9600);

  /* Setup encoder pins as inputs */
  pinMode(encoder1, INPUT);
  pinMode(encoder2, INPUT);
  pinMode(encoder3, INPUT);
  pinMode(encoder4, INPUT);

  pinMode(pumpOn, INPUT); // przycisk wlaczenia pompy
  
  digitalWrite(pumpOn, LOW);

  // encoder pin on interrupt 0
  attachInterrupt(0, decoder, FALLING);
  attachInterrupt(3, decoderM2, FALLING);
}

void loop()
{
  start = millis();

  getJoy = analogRead(sensorX);
  //odczytJoy = analogRead(sensorY);

  //motorSpeedMap = map(odczytJoy, 531, 1023, 0, 100);

  /* Logic for motor speed control by joystick.
     Joystick zero point is 531 (for sensor X).
  */
  if (getJoy > 540) {
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_B, LOW);
    digitalWrite(M1_A, HIGH);
    digitalWrite(M2_A, HIGH);

    analogWrite(refSpeedM1, getJoy / 4);
    analogWrite(refSpeedM2, getJoy / 4);
  }
  else if (getJoy < 520)
  {
    digitalWrite(M1_A, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);
    digitalWrite(M1_B, HIGH);

    analogWrite(refSpeedM1, (1000 - getJoy) / 4);
    analogWrite(refSpeedM2, (1000 - getJoy) / 4);
  }
  else //(odczytJoy>=520 && odczytJoy <= 540)
  {
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, LOW);
    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, LOW);

    analogWrite(refSpeedM1, 0);
    analogWrite(refSpeedM2, 0);
  }


  //using while statement to stay in the loop for continuous
  //interrupts
  while (goingUp == 1) // CW motion in the rotary encoder
  {
    goingUp = 0; // Reset the flag
    counterCW ++;
    difFlag = false;
  }
  while (goingDown == 1) // CCW motion in rotary encoder
  {
    goingDown = 0; // clear the flag
    counterCCW ++;
    difFlag = true;
  }
  //interrups for M2
  while (goingUpM2 == 1) // CW motion in the rotary encoder
  {
    goingUpM2 = 0; // Reset the flag
    counterCWM2 ++;
    difFlagM2 = false;
    //Serial.print("CW = ");
    // Serial.println(counterCWM2);
  }
  while (goingDownM2 == 1) // CCW motion in rotary encoder
  {
    goingDownM2 = 0; // clear the flag
    counterCCWM2 ++;
    difFlagM2 = true;
    //Serial.print("CCW = ");
    //Serial.println(counterCCWM2);
  }


  if (start - finish == 1000)
  {
    finish = start;

    if (difFlag == false)
    {
      dif = (counterCW - counterCW_p);
      counterCW_p = (counterCW);
      
      if (difFlagM2 == false)
      {
        difM2 = (counterCWM2 - counterCW_pM2);
        counterCW_pM2 = (counterCWM2);
      }
      else if (difFlagM2 == true)
      {
        difM2 = (counterCCWM2 - counterCCW_pM2);
        counterCCW_pM2 = (counterCCWM2);
      }
    }
    else if (difFlag == true)
    {
      dif = (counterCCW - counterCCW_p);
      counterCCW_p = (counterCCW);
      if (difFlagM2 == false)
      {
        difM2 = (counterCWM2 - counterCW_pM2);
        counterCW_pM2 = (counterCWM2);
      }
      else if (difFlagM2 == true)
      {
        difM2 = (counterCCWM2 - counterCCW_pM2);
        counterCCW_pM2 = (counterCCWM2);
      }
    }
    motorSpeed = (float)dif / 300; // Counting motor speed in RPM.
    motorSpeedM2 = (float)difM2 / 300;

      Serial.print("Predkosc silnika = ");
      Serial.print(motorSpeed * 60);
      Serial.println(" obr/min");

      Serial.print("Predkosc silnikaM2 = ");
      Serial.print(motorSpeedM2 * 60);
      Serial.println(" obr/min"); 
  }

 if (digitalRead(pumpOn) == HIGH)
  {
    pumpFlag = true;
  }
  else 
    pumpFlag = false;


  if (pumpFlag == true)
  {
    digitalWrite(pumpB, HIGH);
    analogWrite(pumpVoltage, 255);
  }
  else
  {
    digitalWrite(pumpB, LOW);
    analogWrite(pumpVoltage, 0);
  } 
}

void decoder()
{
  if (digitalRead(encoder1) == digitalRead(encoder2))
  {
    goingUp = 1; //if encoder channels are the same, direction is CW
  }
  else
  {
    goingDown = 1; //if they are not the same, direction is CCW
  }
}
void decoderM2()
{
  if (digitalRead(encoder3) == digitalRead(encoder4))
  {
    goingUpM2 = 1; //if encoder channels are the same, direction is CW
  }
  else
  {
    goingDownM2 = 1; //if they are not the same, direction is CCW
  }
}



