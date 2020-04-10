#include <SPI.h> 
#include <WiFiNINA.h>  // Βιβλιοθήκη WiFi
#include "Twitter.h"  // Βιβλιοθήκη επικοινωνίας με το Twitter
#include "DHT.h"                             // Βιβλιοθήκες χειρισμού του αισθητήρα DHT11
#include <Adafruit_Sensor.h>      // Βιβλιοθήκες χειρισμού αισθητήρων της εταιρίας Adafruit
#include <Adafruit_BMP280.h>  // Βιβλιοθήκες χειρισμού του αισθητήρα BMP280 της εταιρίας Adafruit

#define DHTPIN 6              // Ο DHT11 είναι συνδεδεμένος στο Pin 6 του Arduino
#define DHTTYPE DHT11 // O Τύπος DHT είναι o DHT11
#define BMP_SCK 5          // Έξοδος SCK του αισθητήρα στο Pin 5 του Arduino
#define BMP_MISO 4       // Έξοδος SDO του αισθητήρα στο Pin 4 του Arduino
#define BMP_MOSI 3       // Έξοδος SDI του αισθητήρα στο Pin 3 του Arduino
#define BMP_CS 2            // Έξοδος CS του αισθητήρα στο Pin 2 του Arduino

char ssid[] = “ONOMA_TOΠΙΚΟΥ_ΔΙΚΤΥΟΥ”;
char pass[] = “ΣΥΝΘΗΜΑΤΙΚΟ_ΤΟΠΙΚΟΥ_ΔΙΚΤΥΟΥ”; 
int keyIndex = 0;                 // Μεταβλητή απαραίτητη για WEP κρυπτογράφηση δικτύου
int status = WL_IDLE_STATUS; // Κατάσταση δικτύου
Twitter twitter("ΜΥΣΤΙΚΟ_ΚΛΕΙΔΙ_ΑΠΟ: arduino-tweet.appspot.com");

DHT dht(DHTPIN, DHTTYPE); // Αντικείμενο Μετρήσεων του DHT11
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Αντικείμενο Μετρήσεων του BMP280

char msg[140] = "";  // Πίνακας χαρατκήρων που θα φιλοξενήσει στις μετρήσεις

void setup(){
  Serial.begin(9600);
  dht.begin();         // Εκκίνηση λειτουργίας αισθητήρα DHT11
  bmp.begin();       // Εκκίνηση λειτουργίας αισθητήρα BMP280
  // Προσπάθεια σύνδεσης στο τοπικό δίκτυο
  while (status != WL_CONNECTED) { // Όσο δεν είναι συνδεδεμένο
    Serial.print("Attempting to connect to SSID: "); // Εμφάνισε μήνυμα 
    Serial.println(ssid);                                                  // προσπάθειας σύνδεσης
    status = WiFi.begin(ssid, pass); // Σύνδεση στο Τοπικό δίκτυο ssid, με κωδικό pass
    delay(1000); // Αναμονή για 1sec
  }
  printWifiStatus(); // Εμφάνισε την κατάσταση σύνδεσης
}

void loop(){
  String myData = "";  // Καθαρισμός συμβολοσειράς που περιέχει τα δεδομένα
  myData += "Θερμοκρασία: " + String(bmp.readTemperature()) + " C\n"; // Μέτρηση Θερμοκρασίας
  myData += "Υγρασία: " + String(dht.readHumidity()) + " % \n";                 // Μέτρηση Υγρασίας
  myData += "Ατμοσφαιρική Πίεση: " + String(bmp.readPressure()/100) + " mbar"; // Μέτρηση Πίεσης
  myData.toCharArray(msg,140); // Τελικό μήνυμα που περιέχει όλες τις μετρήσεις
  if (twitter.post(msg)) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
  delay(60000); // Αναμονή για 60sec = 1min
}