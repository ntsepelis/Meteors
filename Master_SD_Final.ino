
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "DHT.h" 
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHTPIN 6  // Το Ψηφιακό Pin 6 θα χρησιμοποιηθεί για διάβασμα δεδομένων
#define DHTTYPE DHT11   // DHT 11
#define BMP_SCK A0  // Έξοδος SCK του αισθητήρα στο Pin 5 του Arduino
#define BMP_MISO A1 // Έξοδος SDO του αισθητήρα στο Pin 4 του Arduino
#define BMP_MOSI A2 // Έξοδος SDI του αισθητήρα στο Pin 3 του Arduino
#define BMP_CS A3   // Έξοδος CS του αισθητήρα στο Pin 2 του Arduino

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 3;
const int chipSelect = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DHT dht(DHTPIN, DHTTYPE); // Βασικό Αντικείμενο για αποθήκευση Μετρήσεων

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Βασικό Αντικείμενο Μετρήσεων

int mode = 1;
String msg = "";
String dataString = "";
float x = 0;
char buff[20]="";

void setup(){
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  Wire.begin(); // join i2c bus (address optional for master)
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  dht.begin();
  bmp.begin();
  Serial.begin(9600);

}

void loop(){

  lcd.clear();
  msg = "";
  dataString = "";
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // print the number of seconds since reset:
  if(mode == 1){
    lcd.setCursor(0, 0);
    lcd.print("Temperature");
    msg = String(bmp.readTemperature()) + " C";
    lcd.setCursor(0, 1);
    lcd.print(msg);
    mode = 2;
  }else if(mode == 2){
    lcd.setCursor(0, 0);
    lcd.print("Hummidity");
    msg = String(dht.readHumidity()) + " %";
    lcd.setCursor(0, 1);
    lcd.print(msg);
    mode = 3;
  }else if(mode == 3){
    lcd.setCursor(0, 0);
    lcd.print("Pressure");
    msg = String(bmp.readPressure()/100) + " mbar";
    lcd.setCursor(0, 1);
    lcd.print(msg);
    mode = 1;
  }

  dataString = String(bmp.readTemperature());
  dataString += ",";
  dataString += String(dht.readHumidity());
  dataString += ",";
  dataString += String(bmp.readPressure()/100);
  dataString += "\n";

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("error opening datalog.txt");
  }

  dataString.toCharArray(buff,20);
  for(int i=0;i<20;i++){
    Wire.beginTransmission(7);
    Wire.write(buff[i]);
    Wire.endTransmission();
    //Serial.println(buff);
  }

 delay(1000);
}
