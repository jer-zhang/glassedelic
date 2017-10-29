// **********************************************************
// * Bluetooth                                              *
// * This handles Bluetooth interfacing.                    *
// **********************************************************
// NOTE: The hardware connections are as below. 
#include <SoftwareSerial.h>
SoftwareSerial EEBlue(0, 1); // RX | TX

// --------------------------------------------------
// | Bluetooth_Init                                 |
// | ---------------------------------------------- |
// | This initialization routine is executed in the |
// | Setup routine.                                 |
// --------------------------------------------------
void Bluetooth_Init() {
  EEBlue.begin(9600); 
}

// --------------------------------------------------
// | strcpy                                         |
// | ---------------------------------------------- |
// | A special strcpy implementation that ignroes t |
// | character '|' since it is used as a terminator |
// | by the bluetooth transmitter.                  |
// --------------------------------------------------
char *strcpy(char *dest, const char *src) {
  unsigned i;
  for (i=0; src[i] != '\0'; i++) {
    if (src[i] == '|') break; 
    dest[i] = src[i];
  }
  dest[i] = '\0'; 
  return dest;
}

// --------------------------------------------------
// | Bluetooth_Do                                   |
// | ---------------------------------------------- |
// | This is attahed to a delay routine and is used |
// | to detect incoming messages.                   |
// --------------------------------------------------
void Bluetooth_Do() {
  // Feed any data from bluetooth to Terminal.
  char bufMsg[40] = {0}; int ind = 0;
  if (EEBlue.available()) {
    while (bufMsg[ind - 1] != '|')
      if (EEBlue.available()) bufMsg[ind++] = EEBlue.read();
  }
  if (bufMsg[0] != '\0') {
    Serial.write("Received BT Message: "); 
    Serial.write(bufMsg); 
    Serial.write("\n"); 
    // Copy into BTMessages. 
    totalMessages++; 
    currentMessage = totalMessages - 1;
    needsBTRedrawMessage = true; 
    strcpy(BTMessages[currentMessage], bufMsg); 
  }
  
  // To send message to phone: EEBlue.write("Text");
}

