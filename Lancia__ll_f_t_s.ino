#include <everytime.h>

//Lábkiosztás
const int heaterPin = 5; //Digital láb kimenet a fűtés kapcsoló reléhez
const int battVoltagePin = 6; //Analog bemenet láb feszültségosztóval az akksi szint figyeléshez.
                              //(3-mal kell osztani a feszültséget legalább)
const int switcherPin = 7; //Digital bemenet, ha ez 1-re vált, bekapcsolja a fűtést
const int statusLED = 8; //LED ami akkor világít, ha bekapcsoltuk a fűtést

//Egyéb globális változók
bool turnHeaterOn = false; //Bool változó ami ha igaz, bekapcsolja a fűtést
float batteryMinV = 11.9; //Az akksi minimális feszültsége ami alatt nem enged fűteni
int switcherState = 0; //Kapcsoló állapotát tároló változó
long turnOnMillis = 0;
long heatingTime = 60000 * 30; //A fűtés bekapcsolva tartásának ideje millisec-ben

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); //elindítjuk a soros kapcsolatot
pinMode(heaterPin, OUTPUT); //beállítjuk a lábak funkcióit
pinMode(battVoltagePin, INPUT);
pinMode(switcherPin, INPUT);
}

void loop() {
// Beolvassuk az akksi feszültséget és kiírjuk a soros porton
  float batteryVoltage = analogRead(battVoltagePin) * 3; // kiolvassuk az akksi feszültséget 
                                                         //és szorozzuk hárommal a feszültségosztás miatt
  Serial.print("Battery: "); // Soros ablakba kiírja a szöveget
  Serial.print(batteryVoltage); // Soros ablakba kiírja a feszültséget
  Serial.println(" V"); // Soros ablakba kiírja a szöveget, majd új sort kezd

//Beolvassuk a kapcsoló jelét
every(50){
  switcherState = digitalRead(switcherPin);
}
  
//Bekapcsolja a fűtést, ha a kapcsoló állapota HIGH-ra változott
  if (switcherState == HIGH) {
    // Ha jelet érez a switcher lábon, be/ki kapcsolja a fűtést
    if (turnHeaterOn == false){
      turnHeaterOn = true;
      turnOnMillis = millis();
      Serial.print("Kapcsolójel állapota: "); // Soros ablakba kiírja a szöveget
      Serial.println(switcherState); // Soros ablakba kiírja a kapcsolójel állapotát, majd új sort kezd
    }else{
      turnHeaterOn = false;
    }
  }

//Ha eltelt a fűtés kívánt ideje, kikapcsolja a fűtést
  if (millis() > turnOnMillis + heatingTime){
    turnHeaterOn = false;
  }
  
// Ha az akksi feszültsége túl alacsony, kikapcsolja a fűtést
//Ennek a loop végén kell lennie, a fűtés kapcsolása előtt, mert ha valami a loopon belül bekapcsolná a fűtést,
//felülírja a változót ha alacsony az akksi fesz, mielőtt megpróbálná bekapcsolni
  if (batteryVoltage < batteryMinV){
    turnHeaterOn = false;
  }

//Másodpercenként kétszer ellenőrzi, hogy be van-e kapcsolva a fűtés és kapcsolgatja a statusLED-et
//és a fűtés reléjét
  every(500){
     if (turnHeaterOn == true){
      digitalWrite(statusLED, HIGH);
      digitalWrite(heaterPin, HIGH);
      }  else{
      digitalWrite(statusLED, LOW);
      digitalWrite(heaterPin, LOW);
      }
  }
}
