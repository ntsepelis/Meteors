#include <SPI.h> 
#include <Ethernet.h>
#include <Twitter.h>
#include <Wire.h>



byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 10, 148, 31, 177 };
EthernetServer server(80);
Twitter twitter("1213123363477999617-vWctbExK2TcaszrElSp3NU4VqhK4WC");

//String myData = "";
char msg[140] = "";
char x;
String message = "";
String rest = "";
String temperature = "";
String humidity = "";
String pressure = "";
int index;
int counter = 0;

void setup()
{
  Wire.begin(7);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());  
}

void loop(){

  counter += 1;

  Serial.println(message);
  
  index = message.indexOf(',');
  temperature = message.substring(0,index);
  rest = message.substring(index+1);

  index = rest.indexOf(',');
  humidity = rest.substring(0,index);
  pressure = rest.substring(index+1);

if(counter == 300){
  String myData = "";
  myData = "Θερμοκρασία: " + temperature + " C\n";
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

  EthernetClient client = server.available();
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
  delay(1000);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

void receiveEvent(int bytes){
  x = (Wire.read());    // receive byte as an integer
  message += (x);
}
