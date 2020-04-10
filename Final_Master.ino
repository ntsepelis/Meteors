#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <Wire.h>

#define BMP_SCK 29  // Έξοδος SCK του αισθητήρα στο Pin 13 του Arduino
#define BMP_MISO 27 // Έξοδος SDO του αισθητήρα στο Pin 12 του Arduino
#define BMP_MOSI 25 // Έξοδος SDI του αισθητήρα στο Pin 11 του Arduino
#define BMP_CS 23   // Έξοδος CS του αισθητήρα στο Pin 10 του Arduino

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define DHTPIN 31  // Το Ψηφιακό Pin 6 θα χρησιμοποιηθεί για διάβασμα δεδομένων
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); // Βασικό Αντικείμενο για αποθήκευση Μετρήσεων

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Βασικό Αντικείμενο Μετρήσεων
float temperature;
float pressure;
float hum;

String msg = "";
char buff[20];
int counter = 0;

const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=878,TS_RT=119,TS_TOP=107,TS_BOT=896;

MCUFRIEND_kbv tft;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button t_btn, h_btn, p_btn, all_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void){

  counter += 1;

  Serial.begin(9600);
  Wire.begin();
  bmp.begin();
  dht.begin();
  
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setFont(NULL);
    
  t_btn.initButton(&tft,  120, 60, 200, 50, WHITE, RED, WHITE, "TEMP(C)", 2);
  h_btn.initButton(&tft, 120, 130, 200, 50, WHITE, RED, WHITE, "HUMM(%)", 2);
  p_btn.initButton(&tft, 120, 200, 200, 50, WHITE, RED, WHITE, "PRESS(mb)", 2);
  all_btn.initButton(&tft, 120, 270, 200, 50, WHITE, RED, WHITE, "ALL", 2);
    
  t_btn.drawButton(false);
  h_btn.drawButton(false);
  p_btn.drawButton(false);
  all_btn.drawButton(false);

}


void loop(void){

  Serial.println(counter);

  pressure = bmp.readPressure();  // Αποθήκευση τιμής Ατμοσφαιρικής Πίεσης
  temperature = bmp.readTemperature(); // Αποθήκευση τιμής Θερμοκρασίας
  hum = dht.readHumidity();            // Αποθήκευση τιμής Υγρασίας (DHT.humidity) στη μεταβλητή hum

  bool down = Touch_getXY();
  t_btn.press(down && t_btn.contains(pixel_x, pixel_y));
  h_btn.press(down && h_btn.contains(pixel_x, pixel_y));
  p_btn.press(down && p_btn.contains(pixel_x, pixel_y));
  all_btn.press(down && all_btn.contains(pixel_x, pixel_y));
    
    if (t_btn.justReleased() || h_btn.justReleased() || p_btn.justReleased() || all_btn.justReleased()){
        t_btn.drawButton();
        h_btn.drawButton();
        p_btn.drawButton();
        all_btn.drawButton();
    }
    if (t_btn.justPressed()) {
      msg = String(temperature) + " C";
      tft.fillScreen(BLACK);
      msg.toCharArray(buff, 50);
      showmsgXY(10, 100, 3, NULL, buff);
      delay(5000);
      tft.fillScreen(BLACK);
    }
    if (h_btn.justPressed()) {
      msg = String(hum) + " %";
      tft.fillScreen(BLACK);
      msg.toCharArray(buff, 50);
      showmsgXY(10, 100, 3, NULL, buff);
      delay(5000);
      tft.fillScreen(BLACK);
    }
    if (p_btn.justPressed()) {
      msg = String(pressure/100) + " mbar";
      tft.fillScreen(BLACK);
      msg.toCharArray(buff, 50);
      showmsgXY(10, 100, 3, NULL, buff);
      delay(5000);
      tft.fillScreen(BLACK);
    }
    if (all_btn.justPressed()) {
      msg = String(temperature) + " C\n\n" + String(hum) + " %\n\n" + String(pressure/100) + " mbar";
      tft.fillScreen(BLACK);
      msg.toCharArray(buff, 50);
      showmsgXY(0, 100, 3, NULL, buff);
      delay(5000);
      tft.fillScreen(BLACK);
    }
    // Δημιουργία String τιμών 
    msg = String(temperature);
    msg += ",";
    msg += String(hum);
    msg += ",";
    msg += String(pressure/100);
    msg += "\n";
    msg.toCharArray(buff,20); // Μετατροπή String σε πίνακα Χαρακτήρων για μετάδοση
    for(int i=0;i<20;i++){  // Μετάδοση 20 χαρακτήρων
      Wire.beginTransmission(9); // Εκκίνηση μετάδοσης στη διεύθυνση 9 (dummy τιμή)
      Wire.write(buff[i]);                // Μετάδοση ενός Χαρακτήρα
      Wire.endTransmission();      // Τέλος Μετάδοσης
    }

    delay(500);    // Αναμονή 0,5 sec
}

bool Touch_getXY(void){
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);  
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg){
    //int16_t x1, y1;
    //uint16_t wid, ht;
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(RED);
    tft.setTextSize(sz);
    tft.print(msg);
}