#include <Bounce.h>

#define LEDB 16
#define LEDR 17

#define BTNA 13
#define BTNB 14

Bounce buttonA = Bounce(BTNA, 10);
Bounce buttonB = Bounce(BTNB, 10);  // 10 = 10 ms debounce time

const int ledPin = 6;

byte pinC020[]={12,10, 7, 4, 45,44,43,42,41,40,39,38,20,21,22, 0xFF,
                23,24,25,26,27,  8, 2, 9, 3, 1, 0, 5, 6,11,19, 0xFF};

#define eA18 pinC020[ 0]
#define eA17 pinC020[29]
#define eA16 pinC020[ 1]
#define eA15 pinC020[ 2]
#define eA14 pinC020[28]
#define eA13 pinC020[27]
#define eA12 pinC020[ 3]
#define eA11 pinC020[24]
#define eA10 pinC020[22]
#define eA09 pinC020[25]
#define eA08 pinC020[26]
#define eA07 pinC020[ 4]
#define eA06 pinC020[ 5]
#define eA05 pinC020[ 6]
#define eA04 pinC020[ 7]
#define eA03 pinC020[ 8]
#define eA02 pinC020[ 9]
#define eA01 pinC020[10]
#define eA00 pinC020[11]

#define eD7 pinC020[20]
#define eD6 pinC020[19]
#define eD5 pinC020[18]
#define eD4 pinC020[17]
#define eD3 pinC020[16]
#define eD2 pinC020[15]
#define eD1 pinC020[14]
#define eD0 pinC020[13]

#define eWE pinC020[30]
#define eOE pinC020[23]
#define eCE pinC020[21]

byte pinA[19]={eA00,eA01,eA02,eA03,eA04,eA05,eA06,eA07,eA08,eA09,eA10,eA11,eA12,eA13,eA14,eA15,eA16,eA17,eA18};
byte pinD[8] ={eD0,eD1,eD2,eD3,eD4,eD5,eD6,eD7};

void setup()
{
  Serial.begin(115200);
  
  pinMode(13,INPUT_PULLUP); // BTN A
  pinMode(14,INPUT_PULLUP); // BTN B

  pinMode(16,OUTPUT); // Blue Led
  pinMode(17,OUTPUT); // Red Led
  
  pinMode(eWE,OUTPUT);
  pinMode(eOE,OUTPUT);
  pinMode(eCE,OUTPUT);
  
  for (int i=0;i<19;i++)
  {
    pinMode(pinA[i],OUTPUT);
  }

  for (int i=0;i<8;i++)
  {
    pinMode(pinD[i],INPUT);
  }
  
}

unsigned long addr = 0;
boolean newaddr = true;

void loop()
{
  digitalWrite(LEDB,HIGH);
  
  Serial.println("Ready...");
  
  do
  {
    buttonA.update();
    buttonB.update(); 
  } while (digitalRead(BTNA) == HIGH);

  digitalWrite(LEDB,LOW);
  
  byte c;
  long r = 0;
  
  Serial.println();
  Serial.println("      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
  Serial.println("      -----------------------------------------------");
  
  for (r=0;r < (0x2000-1);r++)
  {
    if (r < 0x10) Serial.print("0");
    if (r < 0x100) Serial.print("0");
    if (r < 0x1000) Serial.print("0");
    
    Serial.print(r,HEX); Serial.print(") ");

    char sixteen[16];
    
    for (c=0;c < 0x10;c++)
    {
      addr = c + r * 0x10;
      
      digitalWrite(eCE, HIGH);
      digitalWrite(eWE, HIGH);
      digitalWrite(eOE, HIGH);
      
      for(int i = 0; i < 18;i++)
      {
        byte adrspin = pinA[i];
        byte adrsvlu = 1 & ( addr >> i);
        digitalWrite(adrspin, adrsvlu);
      }
      
      digitalWrite(eCE, LOW);
      digitalWrite(eOE, LOW);
      
      delayMicroseconds(1); // AT29C020 150 ns
      
      byte value = PINB;

      if (value >= 32)
      { 
        sixteen[c] = char(value);
      }
      else
      {
        sixteen[c] = '.'; // unpritable Chars
      }
      
      if (value < 0x10) Serial.print("0");
      
      Serial.print(value,HEX); Serial.print(" ");
      
      digitalWrite(eOE, HIGH);
      
    }

    Serial.print(" |  ");

    for (c=0;c < 0x10;c++)
    {
      Serial.print(sixteen[c]); 
      //Serial.print(" ");
    }    
    
    Serial.println();
  }
  
  Serial.println();
  
  digitalWrite(eCE, HIGH);
  
  do
  {
    buttonA.update();
    buttonB.update(); 
  } while (digitalRead(BTNA) == LOW);  
}
