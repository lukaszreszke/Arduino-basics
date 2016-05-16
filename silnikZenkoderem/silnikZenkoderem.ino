int pin1 = 2;
int pin2 = 3;
int counterCW;
int counterCW_p = 0;
int counterCCW;
int counterCCW_p = 0;
unsigned long start;
unsigned long finish;
unsigned long dif;
float predkoscSilnika;
boolean goingUp = false;
boolean goingDown = false;
boolean difFlag = false;

int silnik = 6;
int M1_A = 10;
int M1_B = 11;
int M2_A = 9;
int M2_B = 8;
/*ONE ENCODER RESOLUTION IS 16 IMPULSES */

void setup()
{
  pinMode(silnik,OUTPUT);
  pinMode(M1_A,OUTPUT);
  pinMode(M1_B,OUTPUT);
  pinMode(M2_A,OUTPUT);
  pinMode(M2_B,OUTPUT);
  digitalWrite(M1_A, HIGH);
  digitalWrite(M1_B, LOW);
  digitalWrite(M2_A,HIGH);
  digitalWrite(M2_B,LOW);
  counterCW = 0;
  counterCCW = 0;
  //Serial prints for debugging and testing
  Serial.begin(9600);

/* Setup encoder pins as inputs */
    pinMode(pin1, INPUT); // Pin 2
    pinMode(pin2, INPUT); // Pin 4 

// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, decoder, FALLING);
}

void loop()
{
 

   start = millis();

//using while statement to stay in the loop for continuous
//interrupts
  while(goingUp==1) // CW motion in the rotary encoder
    {
      goingUp=0; // Reset the flag
      counterCW ++;
      difFlag = false;
      //Serial.println(counterCW);
     }
   while(goingDown==1) // CCW motion in rotary encoder
{
  goingDown=0; // clear the flag
  counterCCW ++;
  difFlag = true;
  //Serial.println(counterCCW);
}
        if(start - finish == 1000)
      {
        finish = start;
        if(difFlag == false)
          {
            dif = (counterCW - counterCW_p);
            counterCW_p = (counterCW);
          }
        else if(difFlag == true)
          {
             dif = (counterCCW - counterCCW_p);
            counterCCW_p = (counterCCW);
          }
        predkoscSilnika = (float)dif/300;  
        Serial.print("Predkosc silnika = ");
        Serial.print(predkoscSilnika*60);
        Serial.println(" obr/min");
        
      }

}

void decoder()
{
  if (digitalRead(pin1) == digitalRead(pin2))
    {
      goingUp = 1; //if encoder channels are the same, direction is CW
    }
  else
    {
      goingDown = 1; //if they are not the same, direction is CCW
    }
}



