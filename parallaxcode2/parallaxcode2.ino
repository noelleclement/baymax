#include <Servo.h>

//motoren
Servo servoLeft;            //linker motor
Servo servoRight;           //rechter motor

//zijultra
#define ECHOPINrechts 8      //pin to receive echo pulse
#define TRIGPINrechts 9      //pin to send trigger pulse

//onderultra
#define ECHOPINonder 4       //echopin onderultra
#define TRIGPINonder 5       //trigpin onderultra

//voorultra
#define ECHOPINvoor 7       //echopin voorultra
#define TRIGPINvoor 6       //trigpin voorultra

long distance_voor, distance_rechts, distance_onder, voor, rechts, onder;


void setup() {
  

  //motoren
  servoLeft.attach(11);       //linker motor pin 9
  servoRight.attach(10);      //rechter motor pin 5

  //ultrasonen
  Serial.begin(9600);
  pinMode(ECHOPINrechts, INPUT);
  pinMode(TRIGPINrechts, OUTPUT);

  pinMode(ECHOPINonder, INPUT);
  pinMode(TRIGPINonder, OUTPUT);

  pinMode(ECHOPINvoor, INPUT);
  pinMode(TRIGPINvoor, OUTPUT);
  
}

void loop() {

  ///////ULTRASONEN///////
      //voor 
      //meten ultrasoon voor
        digitalWrite(TRIGPINvoor, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGPINvoor, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGPINvoor, LOW);
        
      //distance calculation ultrasoon voor
        distance_voor = pulseIn(ECHOPINvoor, HIGH);
        long voor = distance_voor/29/2;
        if(voor < 0){ //indien gemeten afstand negatief is.
          voor = 0;
         }
        Serial.print("Uvoor: ");
        Serial.print(voor);
        Serial.println(" cm");
      
      //rechts 
      //meten ultrasoon rechts
        digitalWrite(TRIGPINrechts, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGPINrechts, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGPINrechts, LOW);
        
        //distance calculation ultrasoon rechts
        distance_rechts = pulseIn(ECHOPINrechts, HIGH);
        long rechts = distance_rechts/29/2;
        if(rechts < 0){ //indien gemeten afstand negatief is.
          rechts = 0;
        }
        Serial.print("Urechts: ");
        Serial.print(rechts);
        Serial.println(" cm");
      
      //onder 
      //meten ultrasoon onder
        digitalWrite(TRIGPINonder, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGPINonder, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGPINonder, LOW);
        
        //distance calculation ultrasoon onder
        distance_onder = pulseIn(ECHOPINonder, HIGH);
        long onder = distance_onder/29/2;
        if(onder < 0){ //indien gemeten afstand negatief is.
          onder = 0;
        }
        
        Serial.print("Uonder: ");
        Serial.print(onder);
        Serial.println(" cm");

    
      
        
    

   /////CODE/////
  if(onder<=7){
      if(voor>7){
          if(rechts>4 && rechts <8){     //als gewone weg, geen muur voor en muur aan de rechter kant is niet te ver weg: vooruit rijden
            rijvooruit(50);
           //Serial.println("vooruit rijden");
          }
          else if(rechts>=8 && rechts <13){  //als gewone weg, geen muur voor en muur aan de rechter kant is een beetje ver weg: beetje corrigeren naar rechts
            draaibeetjerechts();
            //Serial.println("beetje rechts draaien");
          }
          else if(rechts<=4){                //als gewone weg, geen muur voor en muur an de rechter kant is een beetje ver weg: beetje corrigeren naar links
            draaibeetjelinks();
            //Serial.println("beetje links draaien");
          }

          /*else if(rechts>=13){
            draaikwartrechts();
            rijvooruit(50);
            //Serial.println("bochtje naar rechts");
          }
          */
      }
      
      else if(voor<=7){                     
          if(rechts>9){                   //als gewone weg, wel muur voor en rechts geen muur: naar rechts draaien
            draaikwartrechts();
            rijvooruit(50);
            //Serial.println("bochtje naar rechts");
          }
          else if(rechts<=9){            //als gewone weg, wel muur voor en rechts wel muur: naar links draaien
            draaikwartlinks();
            rijvooruit(100);
            //Serial.println("bochtje naar links");
          }
      }   
  }
  
  else if(onder>7){                    
    rijachteruit(400);
   
      if(rechts>13){                //als afgrond: rij achteruit,  en als rechts geen muur een kwart naar rechts draaien en verder normaal detecteren
        draaikwartrechts();
        loop();
        /*
        voor = voorultrasoon();
        
        //rij vooruit tot muur
        
        while(voorultrasoon() > 7){
          rijvooruit(50);
        }
        draaikwartrechts();
        */
      }
      
      else if(rechts<=13){        //als afgrond: rij achteruit,  en als rechts wel muur een kwart naar links draaien en verder normaal detecteren
        draaikwartlinks();
        loop();
        
        //rij vooruit tot muur
        
        /*while(voorultrasoon() > 7){
          rijvooruit(50);
        }
        draaikwartlinks();
        */
      }
  }
  



/*
   if(onder>13){
      rijachteruit(250);
      if(rechts>10){
        draaikwartrechts();
        //rij vooruit tot muur
        //draaikwartrechts();
      }
      if(rechts<=10){
        draaikwartlinks();
        //rij vooruit tot muur
        //draaikwartlinks();
      }
      
   }
*/  
  
}

void rijvooruit (int tijd){
  //tijd is in microsec dus 1 sec is 1000 microsec
  servoLeft.writeMicroseconds(1550);     //linker motor draait counterclockwise(vooruit) 
  servoRight.writeMicroseconds(1470);    //rechter motor draaien clockwise (vooruit)
  delay(tijd);                          //rechter motor draait voor 'tijd' sec (90cm op 1000 met batterijen en twee motoren)

}

void rijachteruit (int tijd){
  //tijd is in microsec dus 1 sec is 1000 microsec
  
  servoLeft.writeMicroseconds(1470);    //linkermotor draait clockwise (achteruit)
  servoRight.writeMicroseconds(1550);    //rechter motor draaien counterclockwise (achteruit)
  delay(tijd);                          //rechter motor draait andersom voor 'tijd' sec

}

void rijniet (int tijd){
  //tijd is in microsec dus 1 sec is 1000 microsec
  
  servoLeft.writeMicroseconds(1510);     //linker motor draait niet
  servoRight.writeMicroseconds(1510);    //rechter motor draait niet
  delay(tijd);                          //rechter motor draait niet voor 'tijd' sec

}

void draaibeetjelinks(){
  servoLeft.writeMicroseconds(1550);    //normale snelheid linkermotor
  servoRight.writeMicroseconds(1450);   //snellere snelheid rechtermotor
  delay(25);                            
}

void draaibeetjerechts(){
  servoLeft.writeMicroseconds(1590);    //normale snelheid linkermotor
  servoRight.writeMicroseconds(1470);   //snellere snelheid rechtermotor
  delay(25);                            
}

void draaiheel (){
  //draait naar links
  servoLeft.writeMicroseconds(1000);     //linker motor draait clockwise(achteruit) 
  servoRight.writeMicroseconds(1000);    //rechter motor draaien clockwise (vooruit)
  delay(1000);                          //rechter motor draait voor 1 sec 

}

void draaikwartlinks(){
  
  servoLeft.writeMicroseconds(1000);     //linker motor draait clockwise(achteruit) 
  servoRight.writeMicroseconds(1000);    //rechter motor draaien clockwise (vooruit)
  delay(275);                          //rechter motor draait voor 0,275 sec 

}

void draaikwartrechts(){
  
  servoLeft.writeMicroseconds(2000);     //linker motor draait counterclockwise(vooruit) 
  servoRight.writeMicroseconds(2000);    //rechter motor draaien counterclockwise (achteruit)
  delay(275);                          //rechter motor draait voor 0,275 sec 

}




/*
 * 
 * werkt:
  if(ultrasoonvoor()>10.0){
    rijvooruit(1000);
  }
  else{
    rijniet(1000);
  }
 */
