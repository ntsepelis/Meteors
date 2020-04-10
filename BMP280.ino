// Απαραίτητες Βιβλιοθήκες
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 5  // Έξοδος SCK του αισθητήρα στο Pin 5 του Arduino
#define BMP_MISO 4 // Έξοδος SDO του αισθητήρα στο Pin 4 του Arduino
#define BMP_MOSI 3 // Έξοδος SDI του αισθητήρα στο Pin 3 του Arduino
#define BMP_CS 2   // Έξοδος CS του αισθητήρα στο Pin 2 του Arduino

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Βασικό Αντικείμενο
                                                           // Μετρήσεων
float temperature; // Μεταβλητή για αποθήκευση της θερμοκρασίας
float pressure;    // Μεταβλητή για αποθήκευση της ατμοσφαιρικής πίεσης
String msg = "";   // Μεταβλητή String για προβολή των μετρήσεων στην οθόνη

void setup() {        //Αρχικοποίηση
  Serial.begin(9600); // Ενεργοποίηση Οθόνης
  bme.begin();        // Έναρξη Λειτουργίας Αντικειμένου
}
  
void loop() {  // Επαναλαμβανόμενος Κώδικας
    pressure = bmp.readPressure();          // Αποθήκευση τιμής Ατμοσφαιρικής Πίεσης
    temperature = bmp.readTemperature();    // Αποθήκευση τιμής Θερμοκρασίας
    msg += String(pressure/105035) + "atm, "; // Αποθήκευση μετρήσεων
    msg += String(temperature) + "*C, ";      // στη μεταβλητή msg
    msg += String(altimeter) + " m, ";        // για προβολή στην οθόνη
    Serial.println(msg);                      // Εκτύπωση των μετρήσεων
  delay(2000);                                // Αναμονή  2sec πριν την επανάληψη
}