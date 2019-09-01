/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */
 
 /*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

#include <EEPROM.h>

#include "PinChangeInterrupt.h"




//const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to


unsigned int load = 0,gas=0;        // value read from the pot
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

 
/*const int analogInPin2 = A1;  // Analog input pin that the potentiometer is attached to

const int analogInPin3 = A2;
const int analogInPin4 = A3;
const int analogInPin5 = A4;
const int analogInPin6 = A5;
*/


const int valve1 = 6;
const int valve2 = 7;

#define flow1 8
#define flow2 9





char a[73];
unsigned int m=0;
unsigned char mm,tt=0,load_cell=0;



unsigned int pulse1,pulse2,ph,lev1=0,ph1,ph2;

boolean stringComplete = false;  // whether the string is complete
boolean started=false;


unsigned int sec=0;

void received();


void send_msg();


void send_msg()
{
  
  unsigned int kk=0;
 
   Serial.print('*');

  if(ph1<= 9){Serial.print("00");Serial.print(ph1);}
  else if(ph1 <= 99){Serial.print("0");Serial.print(ph1);}
  else if(ph1 <= 999){Serial.print(ph1);} 

  if(ph2<= 9){Serial.print("00");Serial.print(ph2);}
  else if(ph2 <= 99){Serial.print("0");Serial.print(ph2);}
  else if(ph2 <= 999){Serial.print(ph2);}

  
  if(pulse1<= 9){Serial.print("00");Serial.print(pulse1);}
  else if(pulse1 <= 99){Serial.print("0");Serial.print(pulse1);}
  else if(pulse1 <= 999){Serial.print(pulse1);}

   if(pulse2<= 9){Serial.print("00");Serial.print(pulse2);}
  else if(pulse2 <= 99){Serial.print("0");Serial.print(pulse2);}
  else if(pulse2 <= 999){Serial.print(pulse2);} 
}

void receive()
{
  unsigned int i=0;
 if(m > 2)
 {

    if(a[1] == '1')
    {
       if(a[2] == 'N')
       {
         digitalWrite(valve1,LOW);
       }
       else if(a[2] == 'F')
       {
          digitalWrite(valve1,HIGH);
       }
    }
    else if(a[1] == '2')
    {
       if(a[2] == 'N')
       {
         digitalWrite(valve2,LOW);
       }
       else if(a[2] == 'F')
       {
          digitalWrite(valve2,HIGH);
       }
    }
    m =0;
  
 }

}

void setup() {
  // initialize serial:
  
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("   ARDUINO BASED");
 
  pinMode(1,INPUT_PULLUP);
  pinMode(0,INPUT_PULLUP);

   pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);

  pinMode(valve1,OUTPUT);
  pinMode(valve2,OUTPUT);


  pinMode(flow1,INPUT_PULLUP);
  pinMode(flow2,INPUT_PULLUP);

 
  
  
  Serial.begin(9600);
  
  delay(1000);
  sec=170;
 
  
    attachPCINT(digitalPinToPCINT(flow1),l1, CHANGE);
    attachPCINT(digitalPinToPCINT(flow2),l2, CHANGE);

    digitalWrite(valve1,HIGH);
    digitalWrite(valve2,HIGH);
    

    lcd.clear();
   started=true;
}


void loop() {


        ph1 =  analogRead(A0);
         lcd.setCursor(0,0);
         lcd.print("p1:");
         lcd.print( ph1);

          ph2 =  analogRead(A1);
         lcd.setCursor(0,1);
         lcd.print("p2:");
         lcd.print( ph2);

        receive();

       /* if(m > 8)
        {
            for(int i =0;i<=9;i++)
            {
              lcd.setCursor(0+i,1);lcd.print(a[i]);
              
            }
            m = 0;
        }*/

        if(ph1 < 60 || ph1 > 80){digitalWrite(valve1,HIGH);}
        else{digitalWrite(valve1,LOW);}

        if(ph2 < 60 || ph2 > 80){digitalWrite(valve2,HIGH);}
        else{digitalWrite(valve2,LOW);}
          

 
        lcd.setCursor(11,0);lcd.print(pulse1);

        lcd.setCursor(11,1);lcd.print(pulse2);
      
       sec++;
       delay(300);
       if(sec > 1)
       {
        sec = 0;
          send_msg();
        pulse1 = 0;
        pulse2 = 0;
       }
    
      
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar;
    
    
    //a[m] = (char)Serial.read(); 
    inChar = (char)Serial.read(); 
    
    
 
      a[m] = inChar;
      if(a[0] == '*'){ if(m<=6){m++;}}
   
     
  }
}

void l1(void) {
     pulse1++; 
}

void l2(void) {
  pulse2++;
}







