//Απαραίτητες Βιβλιοθήκες
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP=8,XM=A2,YP=A3,YM=9; // Pin της οθόνης για διάφορες λειτουργίες
const int TS_LEFT=878,TS_RT=119,TS_TOP=107,TS_BOT=896; // Διαστάσεις οθόνης 

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Ορισμός οθόνης Αφής (TouchScreen)

Adafruit_GFX_Button t_btn, h_btn, p_btn, all_btn; // τα κουμπιά επιλογών

int pixel_x, pixel_y;     // Μεταβλητές σημείου πίεσης στην Οθόνη

// Ορισμός Χρωμάτων
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void){  // Αρχικοποίηση
  
  uint16_t ID = tft.readID();   // Τύπος Οθόνης
  if (ID == 0xD3D3) ID = 0x9486; // Αλλαγή τύπου αν είναι ο  0xD3D3
  
  tft.begin(ID);               // Έναρξη λειτουργίας Οθόνης
  tft.setRotation(0);      // Οθόνη σε κατακόρυφη θέση
  tft.fillScreen(BLACK); // Μαύρο φόντο οθόνης
  tft.setFont(NULL);      // Δεν έχει τεθεί είδος γραμματοσειράς
  // Κόκκινα Κουμπιά του Μενού
  t_btn.initButton(&tft,  120, 60, 200, 50, WHITE, RED, WHITE, "TEMP(C)", 2);       // Θερμοκρασία
  h_btn.initButton(&tft, 120, 130, 200, 50, WHITE, RED, WHITE, "HUMM(%)", 2); // Υγρασία
  p_btn.initButton(&tft, 120, 200, 200, 50, WHITE, RED, WHITE, "PRESS(mb)", 2); // Πίεση
  all_btn.initButton(&tft, 120, 270, 200, 50, WHITE, RED, WHITE, "ALL", 2);            // Όλα
    
  t_btn.drawButton(false);    // Μην σχεδιάζεις
  h_btn.drawButton(false);   //  ακόμα
  p_btn.drawButton(false);   // κανένα
  all_btn.drawButton(false); // κουμπί
}


void loop(void){ // Κώδικας επανάληψης

  bool down = Touch_getXY();  // Σημείο στην οθόνη που πατήθηκε
  t_btn.press(down && t_btn.contains(pixel_x, pixel_y));       // έλεγξε
  h_btn.press(down && h_btn.contains(pixel_x, pixel_y));     // αν πατήθηκε
  p_btn.press(down && p_btn.contains(pixel_x, pixel_y));     // κάποιο από
  all_btn.press(down && all_btn.contains(pixel_x, pixel_y)); // τα κουμπιά
  
  // αν αφέθηκε το σημείο που πατήθηκε
  if (t_btn.justReleased() || h_btn.justReleased() || p_btn.justReleased() || all_btn.justReleased()){
        t_btn.drawButton(); // Εμφάνισε
        h_btn.drawButton(); // όλα
        p_btn.drawButton(); // τα 
        all_btn.drawButton(); // κουμπιά
    }
    if (t_btn.justPressed()) { // αν πατήθηκε Θερμοκρασία
        tft.fillScreen(BLACK);   // κάνε το φόντο της οθόνης μαύρο
        showmsgXY(20, 100, 3, NULL, "Temperature"); // γράψε: Temperature
        showmsgXY(80, 150, 3, NULL, "27 C"); // άλλαξε γραμμή και γράψε: 27 C
        delay(5000); // αναμονή για 5sec
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
    }
    if (h_btn.justPressed()) { // αν πατήθηκε Υγρασία
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
        showmsgXY(20, 100, 3, NULL, "Hummidity"); // γράψε:  Hummidity
        showmsgXY(80, 150, 3, NULL, "87 %"); // άλλαξε γραμμή και γράψε: 87 %
        delay(5000); // αναμονή για 5sec
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
    }
    if (p_btn.justPressed()) { // αν πατήθηκε Πίεση
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
        showmsgXY(20, 100, 3, NULL, "Pressure"); // γράψε:   Pressure
        showmsgXY(80, 150, 3, NULL, "1023 mbar");  // άλλαξε γραμμή και γράψε:  1023 mbar
        delay(5000); // αναμονή για 5sec
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
    }
    if (all_btn.justPressed()) { // αν πατήθηκε Όλα
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
        showmsgXY(20, 100, 3, NULL, "A L L"); // γράψε: ALL
        delay(5000);  // αναμονή για 5sec
        tft.fillScreen(BLACK); // κάνε το φόντο της οθόνης μαύρο
    }

    
}

bool Touch_getXY(void){ // Συνάρτηση χειρισμού αφής (παρέχεται από τον κατασκευαστή)
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

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg){ // Συνάρτηση προβολής μηνύματος
                                                                                                                             // (παρέχεται από τον κατασκευαστή)
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(RED);
    tft.setTextSize(sz);
    tft.print(msg);
}