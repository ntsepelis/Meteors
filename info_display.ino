
/************************ Βιβλιοθήκες ********************************************/

// Βιβλιοθήκες Οθόνης

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//-------------------------------------------------
#include "DHT.h" // Απαραίτητη Βιβλιοθήκη

//-------------------------------------------------
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


/************************* Δήλωση Σταθερών *******************************************/

// Διαστάσεις Οθόνης

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Σύνδεση της Οθόνης SSD1306 σε I2C (SDA, SCL pins)

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

// --------------------------------------

#define DHTPIN 6  // Το Ψηφιακό Pin 6 θα χρησιμοποιηθεί για διάβασμα δεδομένων
#define DHTTYPE DHT11   // DHT 11

// ---------------------------------------

#define BMP_SCK 5  // Έξοδος SCK του αισθητήρα στο Pin 13 του Arduino
#define BMP_MISO 4 // Έξοδος SDO του αισθητήρα στο Pin 12 του Arduino
#define BMP_MOSI 3 // Έξοδος SDI του αισθητήρα στο Pin 11 του Arduino
#define BMP_CS 2   // Έξοδος CS του αισθητήρα στο Pin 10 του Arduino


/************************* Δήλωση ΒΑΣΙΚΩΝ Μεταβλητών *******************************************/

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Μεταβλητή Οθόνης

DHT dht(DHTPIN, DHTTYPE); // Βασικό Αντικείμενο για αποθήκευση Μετρήσεων

float hum;  // Μεταβλητή Υγρασίας
float temp; // Μεταβλητή Θερμοκρασίας

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Βασικό Αντικείμενο Μετρήσεων
float temperature;
float pressure;
float alttude;
float humm;


String msg = "";

/*************************** Κυρίως Πρόγραμμα *****************************************/ 

void setup() { //Αρχικές Ρυθμίσεις
    
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  dht.begin();
  bmp.begin();
}


void loop() { // Κυρίως Πρόγραμμα
  
  displayText();
  
  delay(2000);
  
}

/*************************** Βοηθητική Συνάρτηση *****************************************/ 

void displayText(void){
  
  display.clearDisplay();     // Καθάρισε την Οθόνη
  display.setTextSize(2);     // Normal 1:1 κλίμακα pixel
  display.setTextColor(WHITE);// Χρώμα Κειμένου: ΛΕΥΚΟ
  display.setCursor(0,0);     // Κέρσορας στο (0,0)
  
  
  int chk = dht.read(DHTPIN); // Διάβασμα Δεδομένων από το Ψηφιακό Pin 2
  hum = dht.readHumidity();                        // Αποθήκευση τιμής Υγρασίας (DHT.humidity) στη μεταβλητή hum
//  temp= dht.readTemperature();                 // Αποθήκευση τιμής Θερμοκρασίας (DHT.temperature) στη μεταβλητή temp
//  msg += String(temp) + "C\n"; // Καταχώρηση των μετρήσεων σε String

  pressure = bmp.readPressure();  // Αποθήκευση τιμής Ατμοσφαιρικής Πίεσης
  temperature = bmp.readTemperature(); // Αποθήκευση τιμής Θερμοκρασίας
  alttude = bmp.readAltitude (1050.35); // Αποθήκευση τιμής Ύψους από την Επιφάνεια της Θάλασσας
  msg = String(pressure/105.035) + "mbar\n";
  msg += String(temperature) + " C\n";
  msg += String(hum) + " %\n";
  msg += String(alttude) + " m";
  
  display.println(msg);   // Εμφάνιση των μετρήσεων στην Οθόνη του Υπολογιστή
  display.display();
}
