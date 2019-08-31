
#define SERIAL_BUFFER_SIZE 64
float total_volume = 0;
volatile int flow_frequency;                // Measures flow sensor pulses
unsigned int l_hour;                        // Calculated litres/hour
float l_sec;                        // Calculated litres/sec
unsigned int kg_sec;
unsigned char flowsensor = 2;               // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
int solenoidPin = 4;                        // This is the output pin on the Arduino we are using
void flow ()                                // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   pinMode(flowsensor, INPUT);
   pinMode(solenoidPin, OUTPUT);            // Sets the pin as an output
   digitalWrite(flowsensor, HIGH);          // Optional Internal Pull-Up
   
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING);        // Setup Interrupt
   sei();                                   // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}
void loop ()
{
   currentTime = millis();                  // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime;              // Updates cloopTime
                                            // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l_sec = (l_hour * 10 / 36);
      total_volume = total_volume + l_sec;
      flow_frequency = 0;                   // Reset Counter
      digitalWrite(solenoidPin, HIGH);    //Switch Solenoid ON
      delay(1000);                      //Wait 1 Second
      digitalWrite(solenoidPin, LOW);     //Switch Solenoid OFF
      
      delay(1000);
      Serial.print("b");
      Serial.print(l_hour);            // Print litres/hour
      Serial.println("L/hour");
      Serial.print("b");
      Serial.print(l_sec);            // Print litres/sec
      Serial.println("mL/sec");      
   }
   
   Serial.print("b");
   Serial.print(total_volume);            // Print litres/sec
   Serial.println("mL");
   delay(0);
}
