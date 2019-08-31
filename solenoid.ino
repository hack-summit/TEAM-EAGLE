

#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it.
#define SERIAL_BUFFER_SIZE 64


#define rx 10                                         //define what pin rx is going to be.
#define tx 11                                         //define what pin tx is going to be.

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work.

//ph sensor variables
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                 //have we received all the data from the PC
boolean sensor_stringcomplete = false;                //have we received all the data from the Atlas Scientific product
float ph;                                             //used to hold a floating point number that is the pH. 
String data="";

//flow sensor variables
float total_volume = 0;
volatile int flow_frequency;                // Measures flow sensor pulses
unsigned int l_hour;                        // Calculated litres/hour
float l_sec;                                // Calculated litres/sec
float flowrate;
float quantity;
unsigned int kg_sec;
unsigned char flowsensor = 2;               // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
int solenoidPin = 4;                        // This is the output pin on the Arduino we are using

void flow ()                                // Interrupt function
{
   flow_frequency++;
}

void setup() {                                        //set up the hardware
  
  pinMode(flowsensor, INPUT);
  pinMode(solenoidPin, OUTPUT);            // Sets the pin as an output
  digitalWrite(flowsensor, HIGH);          // Optional Internal Pull-Up

  currentTime = millis();
  cloopTime = currentTime;
    
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product

  attachInterrupt(0, flow, RISING);        // Setup Interrupt
  sei();                                   // Enable interrupts
 
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  char inchar = (char)Serial.read();                  //get the char we just received
  inputstring += inchar;                              //add it to the inputString
  if (inchar == '\r') {                               
    input_stringcomplete = true;                      //if the incoming character is a <CR>, set the flag
  }
}

void loop() {                                         //here we go...

  data="";
  if (input_stringcomplete) {                         //if a string from the PC has been received in its entirety                     
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    inputstring = "";                                 //clear the string
    input_stringcomplete = false;                     //reset the flag used to tell if we have received a completed string from the PC      
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character.
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;
    if (inchar == '\r') {
      sensor_stringcomplete = true;                   //if the incoming character is a <CR>, set the flag
    }
  }


  if (sensor_stringcomplete) {                        //if a string from the Atlas Scientific product has been received in its entirety
    //Serial.println(sensorstring);                     //send that string to the PC's serial monitor
    data+=sensorstring;
    
    delay (3000);
    sensorstring = "";                                //clear the string:
    sensor_stringcomplete = false;                    //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }


//flowsensorcode
    currentTime = millis();                             // Every second, calculate and print litres/hour
    if(currentTime >= (cloopTime + 1000))
    {
      cloopTime = currentTime;                     // Updates cloopTime
                                                                   // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5);  // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l_sec = (l_hour * 10 / 36);
      flowrate = l_sec;
      total_volume = total_volume + l_sec;
      quantity = total_volume;
      flow_frequency = 0;                   // Reset Counter

      data+=  "  flowrate: " + String(flowrate) + " mL/sec  " + "quantity: " + String(quantity) + " mL ";
     Serial.println(data);
     // Serial.print(l_sec);            // Print litres/sec
      //Serial.println("mL/sec");
      //Serial.print(total_volume);            // Print litres/sec
      //Serial.println("mL");      
   }
  
}
