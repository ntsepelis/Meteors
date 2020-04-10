#include <SPI.h> 
#include <WiFiNINA.h>
#include <Wire.h>
#include "Twitter.h"
#include <SD.h>

const int chipSelect = 10;

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;    
int keyIndex = 0;                

int status = WL_IDLE_STATUS;
WiFiServer server(80);
Twitter twitter("MY_SECRET_KEY");

char msg[20] = "";
char x = 0;
String message = "";
String rest = "";
String temperature = "";
String humidity = "";
String pressure = "";
int index = 0;
int counter = 0;

void setup(){

  Serial.begin(9600);

  if (!SD.begin(chipSelect)) { // Αν η ενεργοποίηση της κάρτας ΔΕΝ πετύχει 
      Serial.println("Card failed, or not present"); // Τύπωσε μήνυμα αποτυχίας 
      while (1); // Μπες σε ατέρμονα βρόχο
  }else{
    Serial.println("card initialized."); // Αλλιώς εκτύπωσε μήνυμα επιτυχίας
  }

  Wire.begin(9);                // Επικοινωνία στη διεύθυνση 9 (dummy τιμή)
  Wire.onReceive(receiveEvent); // Όταν δέχεσαι είσοδο εκτέλεσε τη συνάρτηση διαχείρισης receiveEvent

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(1000);

  server.begin();
  Serial.print("server is at ");
  Serial.println(WiFi.localIP());    
}

  printWifiStatus();

}


void loop(){

  counter += 1;

  index = message.indexOf(',');                           // Βρες που είναι το κόμμα στο μήνυμα που έλαβες
  temperature = message.substring(0,index); // από την αρχή μέχρι το κόμμα είναι η θερμοκρασία
  rest = message.substring(index+1);                // μετά το κόμμα μέχρι το τέλος είναι το υπόλοιπο μήνυμα

  index = rest.indexOf(',');                      // που είναι το καινούριο κόμμα
  humidity = rest.substring(0,index);  // από την αρχή μέχρι το κόμμα είναι η υγρασία 
  pressure = rest.substring(index+1); // μετά το κόμμα μέχρι το τέλος είναι η πίεση
  if(counter == 30){  // αν ο μετρητής είναι 30 (=30 * 0,5 = 15 sec)
   // Αποθήκευσε τις μετρήσεις στην Κάρτα SD  
    String dataString = temperature + "," + humidity + "," + pressure + "\n"; // String μετρήσεων
    File dataFile = SD.open("datalog.txt", FILE_WRITE); 
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println("Data Saved to Disk!");
    }else{
      Serial.println("error opening datalog.txt");
    }
  }
  
if(counter == 120){// αν ο μετρητής είναι 120 (=120 * 0,5 = 60 sec)
// Στείλε το μήνυμα Μετρήσεων στο Twitter
  Serial.println("--------------------- Trying to tweet -------------------");
  String myData = "";
  myData = "Θερμοκρασία: " + temperature + " C\n";
  Serial.println(myData);
  myData += "Υγρασία: " + humidity + " % \n";
  myData += "Ατμοσφαιρική Πίεση: " + pressure + " mbar";
  myData.toCharArray(msg,140);

  if (twitter.post(msg)) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println("------------------------------------ OK.");
    } else {
      Serial.print("--------------------------------- TWEET failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("------------------------------------- connection failed.");
  }
  
  counter = 0;
}
// Υποστήριξη Ιστοσελίδας Μετρήσεων
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<body>");

          client.println("<h1>IoT Meteorologic Hub @ 1st Lyceum of Serres</h1>");
          client.print("<h3>Temprature: ");
          client.print(temperature);
          client.print(" C");
          client.println("<br /><br />");

          client.print("Humidity: ");
          client.print(humidity);
          client.print(" %");
          client.println("<br /><br />");

          client.print("Pressure: ");
          client.print(pressure);
          client.print(" mbar</h3>");
          client.println("<br />");
          
          client.println("</body>");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }

  message = "";
  delay(500);
}

// Συνάρτηση Χειρισμού εισόδου από την i2c
void receiveEvent(int bytes){
  x = Wire.read();    // Αποθήκευση της εισόδου με τη μορφή int
  message += x;      // Σύνθεση μηνύματος Εισόδου
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}