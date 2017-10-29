#include <Wire.h>
#include <stdlib.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_VL6180X.h"

#define MINTEMP 22
#define MAXTEMP 34

Adafruit_VL6180X vl = Adafruit_VL6180X();
Adafruit_AMG88xx amg;

const int NO_MOVEMENT = 0;
const int UP_MOVEMENT = 1;
const int DOWN_MOVEMENT = 2;

int gestureTimeout = 0; 

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
uint8_t bufferImage[8][8] = {0}; 
uint8_t currentImage[8][8] = {0}; 
uint8_t bufferFinger[8][8] = {0}; 
uint8_t currentFinger[8][8] = {0}; 

const uint16_t camColors[] = {0x480F,
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
0xF080,0xF060,0xF040,0xF020,0xF800,};

// --------------------------------------------------
// | Gesture_Init                                   |
// | ---------------------------------------------- |
// | This initialization routine is executed in the |
// | Setup routine.                                 |
// --------------------------------------------------
void Gesture_Init() {
  // Set up the Thermal Camera. 
  bool status;
  status = amg.begin();
  if (!status) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1);
  }
  Serial.println("Thermal Camera has been Initialized. ");
  delay(100); 
  // Set up the Distance Sensor. 
  if (! vl.begin()) {
    Serial.println("Failed to find Distance Sensor, Terminating. ");
    while (1);
  }
  Serial.println("Distance Sensor has been Initialized. ");
}

// --------------------------------------------------
// | Gesture_Do                                     |
// | ---------------------------------------------- |
// | This is attahed to a delay routine and is used |
// | to detect gestures.                            |
// --------------------------------------------------
void Gesture_Do(){ 
  Gesture_Thermal_Do(); 
  // Gesture_Distance_Do(); 
}

// --------------------------------------------------
// | Gesture_Thermal_Do                             |
// | ---------------------------------------------- |
// | Detects hand movement based gestures using the |
// | thermal camera. This method detects and handle |
// | s gestures involving moving the hand up and do |
// | wn in front of the sensor.                     |
// --------------------------------------------------
void Gesture_Thermal_Do() {
  if (gestureTimeout > 0) gestureTimeout--; 
  
  // Display Thermal Camera Image on Display. 
  amg.readPixels(pixels);
  
  // Put pixels onto display and current Image. 
  for(int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++){
    uint8_t colorIndex = map(pixels[i], MINTEMP, MAXTEMP, 0, 255);
    currentImage[i / 8][i % 8] = colorIndex; 
    if (currentMessage > -1)
      tft.fillRect(displayPixelHeight * floor(i / 8), displayPixelWidth / 2 * (i % 8) + 64 , displayPixelHeight, displayPixelWidth / 2, camColors[colorIndex]);
    else
      tft.fillRect(displayPixelHeight * floor(i / 8), displayPixelWidth * (i % 8) , displayPixelHeight, displayPixelWidth, camColors[colorIndex]);
    
  }
  
  // Look at Current Image and Buffer Image. Find individual element differences.
  // If the difference is greater than a threshold, then mark that place's current
  // finger as 1, else clear. 
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (abs(currentImage[i][j] - bufferImage[i][j]) > 30) 
        currentFinger[i][j] = 1; 
      else
        currentFinger[i][j] = 0; 
  
  uint8_t currentFingerSector[3] = {0};
  uint8_t bufferFingerSector[3] = {0};

  // Compare the Current Finger and Buffer Finger using detectMovement.
  for (int j = 0; j < 3; j++) // Left third and Right Third. 
    for (int i = 0; i < 8; i++) {
      currentFingerSector[0] += currentFinger[i][j]; 
      bufferFingerSector[0] += bufferFinger[i][j]; 

      currentFingerSector[2] += currentFinger[i][j + 5]; 
      bufferFingerSector[2] += bufferFinger[i][j + 5]; 
    }
  for (int j = 3; j < 5; j++) // Mid third. 
    for (int i = 0; i < 8; i++) {
      currentFingerSector[1] += currentFinger[i][j]; 
      bufferFingerSector[1] += bufferFinger[i][j]; 
    }
  
  for (int j = 5; j < 8; j++) // Right third. 
    for (int i = 0; i < 8; i++) {
      currentFingerSector[2] += currentFinger[i][j]; 
      bufferFingerSector[2] += bufferFinger[i][j]; 
    }
  

  bool leftCurrent = currentFingerSector[0] > 16; 
  bool midCurrent = currentFingerSector[1] > 10; 
  bool rightCurrent = currentFingerSector[2] > 16; 
  bool leftBuffer = bufferFingerSector[0] > 16; 
  bool midBuffer = bufferFingerSector[1] > 10;
  bool rightBuffer = bufferFingerSector[2] > 16; 

  // Set Buffer Finger to Current Finger and Buffer Image to Current Image. 
  for (int i = 0; i < 8; i++) 
    for (int j = 0; j < 8; j++) {
      bufferFinger[i][j] = currentFinger[i][j]; 
      bufferImage[i][j] = currentImage[i][j]; 
    }


  // Check if the hand is in range. 
  /*
  uint8_t range = vl.readRange();
  uint8_t s2 = vl.readRangeStatus();

  // If hand is out of range, just return. 
  if (s2 != VL6180X_ERROR_NONE) return; 
  */
  
  // Check for Left to Right Movement. (Up to Down)
  if (((midCurrent && !rightCurrent && leftBuffer || rightCurrent && !midCurrent && midBuffer) && !leftCurrent) ||
      (leftBuffer && midBuffer && rightBuffer && (midCurrent || rightCurrent)) && !leftCurrent) {
    if (gestureTimeout > 0) return; 
    gestureTimeout = 5; 
    Serial.write("Gesture: Up to Down\n");
    if (totalMessages == 0) return; 
    currentMessage--; 
    if (currentMessage == -1) currentMessage = totalMessages - 1; 
    needsBTRedrawMessage = true; 
  }

  // Check for Right to Left Movement. (Down to Up) 
  if (((midCurrent && !leftCurrent && rightBuffer || leftCurrent && !midCurrent && midBuffer) && !rightCurrent) || 
      (leftBuffer && midBuffer && rightBuffer && (midCurrent || leftCurrent)) && !rightCurrent) {
    if (gestureTimeout > 0) return; 
    gestureTimeout = 5; 
    Serial.write("Gesture: Down to Up\n");
    if (totalMessages == 0) return; 
    currentMessage++; 
    if (currentMessage == totalMessages) currentMessage = 0; 
    needsBTRedrawMessage = true; 
  }

  
}

// --------------------------------------------------
// | Gesture_Distance_Do                            |
// | ---------------------------------------------- |
// | Detects hand movement gestures using the dista |
// | nce sensor. This method detects and handles ha |
// | nd gestures that involve putting a hand in fro |
// | nt of the sensor and then moving it perpendicu |
// | lar.                                           |
// --------------------------------------------------
void Gesture_Distance_Do() {
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    // Do stuff with the variable range. 
  }

  /*
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  */

}

