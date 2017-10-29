// **********************************************************
// * Display                                                *
// * This handles LCD display interfacing.                  *
// **********************************************************
// NOTE:
// The following line should be placed in Adafruit_ST7735::drawPixel: 
// x = _width - 1 - x; 
// This reflects the screen. 

// --------------------------------------------------
// | Display_Init                                   |
// | ---------------------------------------------- |
// | This initialization routine is executed in the |
// | Setup routine.                                 |
// --------------------------------------------------
void Display_Init() {
  tft.initR(INITR_REDTAB);        // Initialize to the RedTab screen spec. 
  tft.fillScreen(ST7735_BLACK);   // Clear the screen. 
  displayPixelWidth = tft.width() / 8;
  displayPixelHeight = tft.height() / 8;
  tft.setRotation(1);             // Rotation is horizontal. 
  Serial.write("Display has been Initialized. \n"); 
}

