#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = “ONOMA_TOΠΙΚΟΥ_ΔΙΚΤΥΟΥ”;
char pass[] = “ΣΥΝΘΗΜΑΤΙΚΟ_ΤΟΠΙΚΟΥ_ΔΙΚΤΥΟΥ”; 
int keyIndex = 0;                 // Μεταβλητή απαραίτητη για WEP κρυπτογράφηση δικτύου
int status = WL_IDLE_STATUS; // Κατάσταση δικτύου
WiFiServer server(80);                                      // Ορισμός Server στο port 80

void setup() {    // Αρχικοποίηση
  Serial.begin(9600); // Εκκίνηση της σύνδεσης στην οθόνη
  // Έλεγχος λειτουργίας του WiFi του Arduino
  if (WiFi.status() == WL_NO_MODULE) { // Αν δεν βρεθεί WiFi Module 
    Serial.println("Communication with WiFi module failed!"); // Εμφάνισε μήνυμα αποτυχίας
    while (true); // Μην προχωράς στην εκτέλεση του προγράμματος
  }
  String fv = WiFi.firmwareVersion();  // Αποθήκευσε την έκδοση του firmware της διάταξης WiFi
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) { // Αν δεν είναι η πιο πρόσφατη
    Serial.println("Please upgrade the firmware"); // Εμφάνισε μήνυμα ενημέρωσης/αναβάθμισης
  }
  // Προσπάθεια σύνδεσης στο τοπικό δίκτυο
  while (status != WL_CONNECTED) { // Όσο δεν είναι συνδεδεμένο
    Serial.print("Attempting to connect to SSID: "); // Εμφάνισε μήνυμα 
    Serial.println(ssid);                                                  // προσπάθειας σύνδεσης
    status = WiFi.begin(ssid, pass); // Σύνδεση στο Τοπικό δίκτυο ssid, με κωδικό pass
    delay(1000); // Αναμονή για 1sec
  }
  server.begin(); // Εκκίνηση του Server
  Serial.print("server is at IP: "); // Μήνυμα στην οθόνη ότι ο Server λειτουργεί στη διεύθυνση ΙΡ: 
  Serial.println(Ethernet.localIP()); // του server
  Serial.println("So, connect there!");  // Προτροπή για σύνδεση σε αυτήν τη διεύθυνση
  printWifiStatus(); // Εμφάνισε την κατάσταση σύνδεσης
}

void loop() {  // Επαναλαμβανόμενο πρόγραμμα – Εντολές προς τον Server
  WiFiClient client = server.available(); // Περίμενε αίτησεις για σύνδεση
  if (client) {                                                       // Αν υπάρξει αίτηση για σύνδεση από πελάτη (client) = browser
    Serial.println("new client");                      // Τύπωσε στην οθόνη το μήνυμα “new client”
    boolean currentLineIsBlank = true;         // κάθε αίτηση τελειώνει με μια κενή γραμμή, προς το παρόν αληθές
    while (client.connected()) {                      // όσο ο πελάτης είναι συνδεδεμένος
      if (client.available()) {                              // αν είναι διαθέσιμος
        char c = client.read();                            // ξεκίνα να διαβάζεις την αίτηση του πελάτη γράμμα προς γράμμα
        Serial.write(c);                                         // Γράψε την αίτηση στην οθόνη
        if (c == '\n' && currentLineIsBlank) {   // Αν έφτασες σε κενή γραμμή
          client.println("HTTP/1.1 200 OK");     // Τύπος HTTP 
          client.println("Content-Type: text/html"); // Το κείμενο που θα ακολουθήσει είναι μορφής
                                                                                    // HTML, δηλαδή είναι ιστοσελίδα
          client.println("Connection: close");      // Η σύνδεση θα κλείσει μετά την προβολή της σελίδας
          client.println("Refresh: 5");                    // Αυτόματη ανανέωση σελίδας κάθε 5 δευτερόλεπτα
          client.println();                                         // Κενή γραμμή
// Εμφάνιση της Ιστοσελίδας που αναφέρθηκε πιο πάνω
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");          
          client.println("<h1>IoT Meteorologic Hub @ 1st Lyceum of Serres</h1>");
          client.print("<h2> This is a TEST Page");
          client.println("</h2><br />");
          client.println("</body>");
          client.println("</html>");// Τέλος Ιστοσελίδας
          break;                                    // ‘Εξοδος από το βρόχο while
        }
        if (c == '\n') { // Αν έφτασες σε αλλαγή γραμμής 
          currentLineIsBlank = true; // Τότε θέσε τη μεταβλητή  currentLineIsBlank αληθή, ώστε να εμφανιστεί η σελίδα
        } else if (c != '\r') { // Αλλιώς αν δεν έχει πατηθεί το enter
          currentLineIsBlank = false; // βρίσκεσε στην ίδια γραμμή, οπότε συνέχισε το διάβασμα της αίτησης
        }
      }
    }
    delay(1); // Δώσε χρόνο στον browser να λάβει τα δεδομένα
    client.stop();  // Κλείσε τη σύνδεση
    Serial.println("client disconnected"); // Τύπωσε στην οθόνη ότι η σύνδεση τερματίστηκε
  }
}