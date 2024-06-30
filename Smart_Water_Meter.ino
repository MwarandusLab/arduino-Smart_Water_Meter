#include <SoftwareSerial.h>

int Lock = 7;
int Pump = 6;
int TX = 5;
int RX = 4;
int Flow =3;

char incomingByte;
String inputString ;

int smsSent=0;

SoftwareSerial mySerial(5, 4); //SIM800L Tx & Rx is connected to Arduino #5 & #4

void setup() {
  Serial.begin(9600);
  
  mySerial.begin(9600);

  pinMode(Lock, OUTPUT);
  pinMode(Pump, OUTPUT);
  
  digitalWrite(Lock, HIGH);
  digitalWrite(Pump, HIGH);
  
  while(!mySerial.available()){
    mySerial.println("AT");
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(500);
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  delay(500);
  //mySerial.println("AT+CMGL=\"REC UNREAD\""); //Read Unread Messages


}

void loop() {
  updateSerial();

  if(mySerial.available()){  
    delay(100);
    while(mySerial.available()){
      incomingByte = mySerial.read();
      inputString += incomingByte;
    }
    delay(1000);

    Serial.println(inputString);
    inputString.toUpperCase(); //uppercase the received Message
    if(inputString.indexOf("ON") > -1){
      digitalWrite(Pump, LOW);
      // digitalWrite(Lock, LOW);
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      mySerial.println("AT+CMGS=\"+254748613509\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      mySerial.print("VALVE OPEN"); //text content
      updateSerial();
      mySerial.write(26);
      delay(1000);
    }else if(inputString.indexOf("OFF") > -1){
      digitalWrite(Pump, HIGH);
      delay(500);
      digitalWrite(Lock, HIGH);
      delay(1000);
      mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
      updateSerial();
      mySerial.println("AT+CMGS=\"+254748613509\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      mySerial.print("VALVE CLOSED"); //text content
      updateSerial();
      mySerial.write(26);
      delay(1000);
    }else if(inputString.indexOf("STATUS") > -1){
      if(digitalRead(Pump) == HIGH && digitalRead(Lock) == HIGH){
        mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
        updateSerial();
        mySerial.println("AT+CMGS=\"+254748613509\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
        updateSerial(); 
        mySerial.print("VALVE CLOSED"); //text content
        updateSerial();
        mySerial.write(26);
        delay(1000);
      }else if(digitalRead(Pump) == LOW && digitalRead(Lock) == LOW){
        mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
        updateSerial();
        mySerial.println("AT+CMGS=\"+254748613509\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
        updateSerial();
        mySerial.print("VALVE OPEN"); //text content
        updateSerial();
        mySerial.write(26);
        delay(1000);
      }
    }

    delay(1000);
    //delete Messages & Save Memory
        
    if(inputString.indexOf("OK") == -1){
      mySerial.println("AT+CMGDA=\"DEL ALL\"");

      delay(1000);
      }
      inputString = "";
  }

}

void updateSerial(){
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
