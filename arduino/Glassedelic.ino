// **********************************************************
// * Glassedelic                                            *
// * This is the main program file for the connected glass- *
// * es system.                                             * 
// **********************************************************

// ST7735 Hardware Interfacing. 
#define sclk 13
#define mosi 11
#define cs   10  
#define dc   9 
#define rest  8
#define sdcs 4

#include <Adafruit_GFX.h>  
#include <Adafruit_ST7735.h>
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rest);
uint16_t displayPixelWidth, displayPixelHeight;

char BTMessages[50][40] = {0}; 
int currentMessage = -1;
int totalMessages = 0; 
bool needsBTRedrawMessage = false;  

// --------------------------------------------------
// | Setup                                          |
// | ---------------------------------------------- |
// | This initialization routine is executed in the |
// | Main routine upon startup.                     |
// --------------------------------------------------
void setup() {
  // All status messages are implemented in the corresponding Init() procedures. 
  Serial.begin(9600); 
  Serial.write("Initializing Gesture Glasses. \n");
  Display_Init();   
  Gesture_Init(); 
  Bluetooth_Init();
}

// --------------------------------------------------
// | Loop                                           |
// | ---------------------------------------------- |
// | This loop routine is executed indefinitely.    |
// --------------------------------------------------
void loop() {
  // Do Gestures. 
  Gesture_Thermal_Do(); 
  Bluetooth_Do(); 
  if (needsBTRedrawMessage) {
    needsBTRedrawMessage = false; 
    Graphics_DrawIconAndText(6, BTMessages[currentMessage], 0xFFFF); 
  }
  delay(50); 
}
