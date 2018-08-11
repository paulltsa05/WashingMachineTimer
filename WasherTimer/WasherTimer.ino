/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 0;

// Relay Board (RED Wire - VCC(+5V) and Black Wire(GND))
int RelayPower = 4;  // Relay Board (White Wire)
int AgitationRelay1 = 5; // Relay Board (Grey Wire)
int AgitationRelay2 = 2; // Relay Board (Blue Wire)

int BuzzerRelay = 3; // Relay Board (Green Wire)


// Variables will change :
int AgitationState = HIGH;             // ledState used to set the LED
int RelayPowerState = HIGH;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 7000;           // interval at which to blink (milliseconds)
const long WashTime = 20*60000;//30*60000;           // interval at which to blink (milliseconds)

unsigned long WashTimer = 0;

int OldbuttonState = HIGH;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(RelayPower, OUTPUT);
  pinMode(AgitationRelay1, OUTPUT);
  pinMode(AgitationRelay2, OUTPUT);  
  pinMode(BuzzerRelay, OUTPUT);

  digitalWrite(AgitationRelay1, HIGH);// OFF
  digitalWrite(AgitationRelay2, HIGH);// OFF
  digitalWrite(RelayPower,HIGH);// OFF
  digitalWrite(BuzzerRelay,HIGH);// OFF
}

// the loop routine runs over and over again forever:
void loop() {
  delay(50);        // delay in between reads for stability
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  if(OldbuttonState != buttonState)
  {
    // print out the state of the button:
    Serial.print("buttonState:");
    Serial.println(buttonState);
    if(buttonState == 0) // Switch Press Detected
    {
      Serial.println("Press Detected");
      WashTimer = 0; //Reset Timer
      if(RelayPowerState == HIGH)
      {
        RelayPowerState = LOW ;// ON Power Relay
        Serial.println("Switch ON Power Relay");
        
        AgitationState = HIGH; // OFF
      }
      else
      {
        RelayPowerState = HIGH ;// OFF Power Relay 
        AgitationState = HIGH; // OFF
        Serial.println("Switch OFF Power Relay"); 
      }

//      digitalWrite(AgitationRelay2, AgitationState);
  
      digitalWrite(BuzzerRelay,LOW);// ON
      digitalWrite(AgitationRelay1, HIGH);
      digitalWrite(AgitationRelay2, HIGH);      
      delay(250);
      digitalWrite(BuzzerRelay,HIGH);// OFF

      digitalWrite(RelayPower,RelayPowerState);
      if(AgitationState==HIGH)
      {
        digitalWrite(AgitationRelay1, LOW);
        digitalWrite(AgitationRelay2, HIGH);
      }
      else
      {
        digitalWrite(AgitationRelay1, HIGH);
        digitalWrite(AgitationRelay2, LOW);
      }
      previousMillis = 0; //Reset interval comparator of previous global var
    }
    
    
  }
  OldbuttonState = buttonState;
  
  //Switch OFF on WashTimer reach WashTime
  if(WashTimer >= WashTime)
  {
     //switch OFF
     WashTimer = 0; //Reset Timer
     RelayPowerState = HIGH ;// OFF Power Relay
     AgitationState = HIGH ;// OFF Power Relay 
     digitalWrite(RelayPower,RelayPowerState);
     digitalWrite(AgitationRelay1, HIGH);
     digitalWrite(AgitationRelay2, HIGH);
     Serial.println("WashTimeOut Detected !!!");

     //Switch on Buzzer for 8 sec
     digitalWrite(BuzzerRelay,LOW);// ON
     int count= 80; //8 sec 
     while(count--)
     {
        delay(100);
        if(digitalRead(pushButton) == 0)//if button press detected skip buzzer
          break;
     }
     digitalWrite(BuzzerRelay,HIGH);// OFF  
     delay(2000);         
     
  }

  
    if(RelayPowerState == LOW )//check it is power ON
    {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        WashTimer +=interval;
        previousMillis = currentMillis;

        if (AgitationState == LOW) {
          AgitationState = HIGH;
        } else {
          AgitationState = LOW;
        }    
          Serial.print("changing relay state : ");
          Serial.println(AgitationState);
          Serial.print("WashTimer :");
          Serial.println(WashTimer);
          digitalWrite(RelayPower,HIGH);// off for 1.5 sec for changing direction
          digitalWrite(AgitationRelay1, HIGH);
          digitalWrite(AgitationRelay2, HIGH);
          
          delay(1000);
          if(AgitationState==HIGH)
          {
            digitalWrite(AgitationRelay1, LOW);
            digitalWrite(AgitationRelay2, HIGH);
          }
          else
          {
            digitalWrite(AgitationRelay1, HIGH);
            digitalWrite(AgitationRelay2, LOW);
          }
          delay(1000);
//         int countOFF= 20; //2 sec 
//         while(countOFF--)
//         {
//            delay(100);
//            if(digitalRead(pushButton) == 0)//if button press detected skip buzzer
//            {
//               WashTimer = 0; //Reset Timer
//               RelayPowerState = HIGH ;// OFF Power Relay
//               AgitationState = HIGH ;// OFF Power Relay 
//               digitalWrite(RelayPower,RelayPowerState);
//               digitalWrite(AgitationRelay, AgitationState);
//               Serial.println("WashTimeOut Detected !!!");
//               delay(2000);
//              break;
//            }
//         }          
//          if(countOFF == 0)//normal working if not interrupt by button press to switch OFF
//          {  
            digitalWrite(RelayPower,LOW);
//          }
            
    }
  }


}



