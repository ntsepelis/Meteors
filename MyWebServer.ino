

#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h" 
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHTPIN 6
#define DHTTYPE DHT11
#define BMP_SCK 5  // Έξοδος SCK του αισθητήρα στο Pin 13 του Arduino
#define BMP_MISO 4 // Έξοδος SDO του αισθητήρα στο Pin 12 του Arduino
#define BMP_MOSI 3 // Έξοδος SDI του αισθητήρα στο Pin 11 του Arduino
#define BMP_CS 2   // Έξοδος CS του αισθητήρα στο Pin 10 του Arduino

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); // Βασικό Αντικείμενο Μετρήσεων

void setup() {

  dht.begin();
  bmp.begin();
  Serial.begin(9600);
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<head>");
          client.println("Arduino Meteorologic Server");
          client.println("</head>");

          client.println("<body>");

          client.print("Temprature: ");
          client.print(bmp.readTemperature());
          client.print(" C");
          client.println("<br />");

          client.print("Humidity: ");
          client.print(dht.readHumidity());
          client.print(" %");
          client.println("<br />");

          client.print("Pressure: ");
          client.print(int(bmp.readPressure()/100));
          client.print(" mbar");
          client.println("<br />");

          client.println("</body>");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  delay(5000);
}
