// Απαραίτητες Βιβλιοθήκες
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10; // Pin στο οποίο θα συνδεθεί το CS

String dataString = "Test String"; // Μήνυμα που θα αποθηκευτεί στην κάρτα SD

void setup(){ // Αρχικοποίηση
  Serial.begin(9600);           // Ενεργοποίηση Οθόνης Υπολογιστή
   if (!SD.begin(chipSelect)) { // Αν η ενεργοποίηση της κάρτας ΔΕΝ πετύχει
    Serial.println("Card failed, or not present"); // Τύπωσε μήνυμα αποτυχίας
    while (1);                                     // Μπες σε ατέρμονα βρόχο
  }
  Serial.println("card initialized."); // Αλλιώς εκτύπωσε μήνυμα επιτυχίας
}

void loop() {  // Επανάληψη Κώδικα
  File dataFile = SD.open("datalog.txt", FILE_WRITE); // Άνοιξε το αρχείο datalog.txt για εγγραφή
  if (dataFile) {    // Αν το άνοιγμα του αρχείου είναι επιτυχές
    dataFile.println(dataString);  // Αποθήκευσε στο αρχείο το String
    dataFile.close();              // Κλείσε το αρχείο
    Serial.println(dataString);    // Εμφάνισε στην Οθόνη αυτό που αποθήκευσες
  }
  else {   // Αν το άνοιγμα του αρχείου ΔΕΝ είναι επιτυχές
    Serial.println("error opening datalog.txt"); // Τύπωσε μήνυμα αποτυχίας
  }
  delay(5000); // Αναμονή για 5 sec πριν την επανάληψη
}