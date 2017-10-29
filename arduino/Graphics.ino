// **********************************************************
// * Graphics                                               *
// * This handles graphics routines specific to the glasses *
// * setup.                                                 * 
// **********************************************************

#include "monof5510pt7b.h"
#include "pictures.h"
boolean isInverted = false; 

// --------------------------------------------------
// | Graphics_DrawText                              |
// | ---------------------------------------------- |
// | Draws some text on a screen. Each line holds   |
// | 13 letters in the Monofur font.                |
// | NOTE: texts are wrapped by character, not by   |
// |       words.                                   |
// --------------------------------------------------
void Graphics_DrawText(const char *text, uint16_t color) {
  tft.setFont(&monof5510pt7b);             // Set the font. 
  tft.setCursor(0, 26);                    // setCursor in Adafruit_GFX takes pixels as units. 
  tft.setTextColor(color);                 // Color should be RGB565. 
  tft.setTextWrap(true);                   // The wrapped text cuts off words and does not respect cursor indents. 
  tft.print(text);                         // Print the actual text. 
}

// --------------------------------------------------
// | Graphics_DrawTextHalf                          |
// | ---------------------------------------------- |
// | Draws some text on a screen. Each line holds   |
// | 6 letters in the Monofur font.                 |
// | Only the Right Half of the screen as seen by   |
// | the human eye is occupied, leaving the left    |
// | half empty for a graphic.                      |
// | NOTE: texts are wrapped by character, not by   |
// |       words.                                   |
// | NOTE: newlines '\n' are counted towards the    |
// |       character limit per line. This means     |
// |       this routine should not be used to draw  |
// |       text that contains multiline formatting. |
// --------------------------------------------------
void Graphics_DrawTextHalf(const char *text, uint16_t color) {
  int i; int lineNumber = 0; const char *text2 = text;  // Some variables. 
  tft.setFont(&monof5510pt7b);                          // Set the font. 
  tft.setTextColor(color);                              // Color should be RGB565. 
  tft.setTextWrap(true);                                // The wrapped text cuts off words and does not respect cursor indents.
  for (i = 0; i < getStringLength(text2); i++) {        // Cycle thru every character in the string. 
    if (i % 8 == 0) {                                   // See if a new line is needed. 
      lineNumber++;                                     // Add a new line. 
      tft.setCursor(80, 8 + 18 * lineNumber);           // Move the cursor to a new line. 
    }                                                   // Move along. 
    tft.print(text[i]);                                 // Print the character. 
  }                                                     // Move along. 
}

// --------------------------------------------------
// | Graphics_DrawTextWord                          |
// | ---------------------------------------------- |
// | Draws some text on a screen. Each line holds   |
// | 13 letters in the Monofur font.                |
// | NOTE: texts are wrapped by the word, and are   |
// |       wrapped with a new line never cutting in |
// |       the middle of a word.                    |
// --------------------------------------------------
void Graphics_DrawTextWord(const char *text, uint16_t color) {
  int i; int j; int WordLength = 0;              // Some variables. 
  int linePosition = 0;                          // Initialize the line cursor position. 
  int lineNumber = 0;                            // Initialize the line cursor number. 
  tft.setFont(&monof5510pt7b);                   // Set the font. 
  tft.setTextColor(color);                       // Set the color. 
  tft.setTextWrap(false);                        // Do not wrap extra text. 
  tft.setCursor(0, 26);                          // Set the initial cursor position. 
  for (i = 0; i < getStringLength(text); i++) {  // Cycle thru every character. 
    WordLength = 0;                              // Begin finding the length of the next word. 
    j = i;                                       // Find the length of the next word.
    while (text[j] != ' ') {                     // Locate the next space to find the word length. 
      WordLength++;                              // When it's not space, add to word length. 
      j++;                                       // Add to the search index. 
    }                                            // These aren't the droids you're looking for. 
    if (WordLength + linePosition >= 17) {       // See if the next word fits. 
      linePosition = 0;                          // Switch to a new line if it doesn't fit. 
      lineNumber++;                              // Switch to a new line if it doesn't fit. 
      tft.setCursor(0, 26 + 18 * lineNumber);    // Switch to a new line if it doesn't fit. 
    }                                            // Move along. 
    for (j = i; j < i + WordLength + 1; j++) {   // Now print out the word. 
      tft.print(text[j]);                        // Print out each character. 
      linePosition++;                            // Incrementing the cursor. 
    }                                            // Move along. 
    i += WordLength;                             // Increment the word pointer. 
  }                                              // Move along. 
}

// --------------------------------------------------
// | Graphics_DrawTextWordHalf                      |
// | ---------------------------------------------- |
// | Draws some text on a screen. Each line holds   |
// | 13 letters in the Monofur font.                |
// | NOTE: texts are wrapped by the word, and are   |
// |       wrapped with a new line never cutting in |
// |       the middle of a word.                    |
// --------------------------------------------------
void Graphics_DrawTextWordHalf(const char *text, uint16_t color) {
  int i; int j; int WordLength = 0;              // Some variables. 
  int linePosition = 0;                          // Initialize the line cursor position. 
  int lineNumber = 0;                            // Initialize the line cursor number. 
  tft.setFont(&monof5510pt7b);                   // Set the font. 
  tft.setTextColor(color);                       // Set the color. 
  tft.setTextWrap(false);                        // Do not wrap extra text. 
  tft.setCursor(80, 14);                         // Set the initial cursor position. 
  for (i = 0; i < getStringLength(text); i++) {  // Cycle thru every character. 
    WordLength = 0;                              // Begin finding the length of the next word. 
    j = i;                                       // Find the length of the next word.
    while (text[j] != ' ') {                     // Locate the next space to find the word length. 
      WordLength++;                              // When it's not space, add to word length. 
      j++;                                       // Add to the search index. 
    }                                            // These aren't the droids you're looking for. 
    if (WordLength + linePosition >= 9) {        // See if the next word fits. 
      linePosition = 0;                          // Switch to a new line if it doesn't fit. 
      lineNumber++;                              // Switch to a new line if it doesn't fit. 
      if (lineNumber > 2) return; 
      tft.setCursor(80, 14 + 18 * lineNumber);   // Switch to a new line if it doesn't fit. 
    }                                            // Move along. 
    for (j = i; j < i + WordLength + 1; j++) {   // Now print out the word. 
      if (j > getStringLength(text)) return; 
      tft.print(text[j]);                        // Print out each character. 
      linePosition++;                            // Incrementing the cursor. 
    }                                            // Move along. 
    i += WordLength;                             // Increment the word pointer. 
  }                                              // Move along. 
}

// --------------------------------------------------
// | Graphics_DrawBitmap                            |
// | ---------------------------------------------- |
// | Draws an image onto the screen using the RGB   |
// | 565 color format.                              | 
// --------------------------------------------------
void Graphics_DrawBitmap(int x, int y, int w, int h, const uint16_t *bitmap) {
  for (int i = 0; i < w; i++) {                             // Cycle thru the y-rows. 
    for (int j = 0; j < h; j++) {                           // Cycle thru the x-rows. 
      tft.drawPixel(x + j, y + h - i, bitmap[i * w + j]);   // Draw the color. 
    }                                                       // Move along. 
  }                                                         // Move along. 
}

// --------------------------------------------------
// | Graphics_ClearScreeen                          |
// | ---------------------------------------------- |
// | Clears the screen.                             | 
// --------------------------------------------------
void Graphics_ClearScreen() {
  tft.fillScreen(0x0000);   // Fill the screen with Black. 
}

// --------------------------------------------------
// | Graphics_DrawBitmapColor                       |
// | ---------------------------------------------- |
// | Draws an image onto the screen.                |
// | NOTE: This skips all colors marked 0xFFFF.     |
// --------------------------------------------------
void Graphics_DrawBitmapColor(int x, int y, int w, int h, const uint16_t *bitmap, uint16_t color) {
  for (int i = 0; i < w; i++) {                             // Cycle thru the y-rows. 
    for (int j = 0; j < h; j++) {                           // Cycle thru the x-rows. 
      if (bitmap[i * w + j] != 0xFFFF)                      // Testfor. 
        tft.drawPixel(x + j, y + h - i, color);             // Draw the color. 
    }                                                       // Move along. 
  }                                                         // Move along. 
}

// --------------------------------------------------
// | Graphics_Invert                                |
// | ---------------------------------------------- |
// | Inverts all colors on the display.             |
// --------------------------------------------------
void Graphics_Invert() {
  isInverted = ~isInverted;       // Toggle. 
  tft.invertDisplay(isInverted);  // Invert. 
}

// --------------------------------------------------
// | Graphics_DrawIconAndText                       |
// | ---------------------------------------------- |
// | Clears the screen, draws text, and draws an    |
// | icon to the screen.                            |
// | NOTE: Values of IconIndex are:                 |
// |       0. Glasses                               |
// |       1. Mail                                  |
// |       2. Message                               |
// |       3. Navigation Left                       |
// |       4. Navigation Right                      |
// |       5. Navigation U Turn                     |
// |       6. Other Notification                    |
// |       7. Sound                                 |
// --------------------------------------------------
void Graphics_DrawIconAndText(uint8_t IconIndex, const char *text, uint16_t color) {
  tft.fillRect(0, 0, 160, 64, 0);                                            // Clear the screen. 
  Graphics_DrawTextWordHalf(text, color);                             // Draw the text. 
  if (IconIndex == 0)                                                 // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, Glasses, color);          // Test and draw the icon.
  else if (IconIndex == 1)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, Mail, color);             // Test and draw the icon.
  else if (IconIndex == 2)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, Message, color);          // Test and draw the icon.
  else if (IconIndex == 3)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, NavigationLeft, color);   // Test and draw the icon.
  else if (IconIndex == 4)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, NavigationRight, color);  // Test and draw the icon.
  else if (IconIndex == 5)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, NavigationUTurn, color);  // Test and draw the icon.
  else if (IconIndex == 6)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 0, 64, 64, OtherNotif, color);       // Test and draw the icon.
  else if (IconIndex == 7)                                            // Test and draw the icon.
    Graphics_DrawBitmapColor(8, 2, 64, 64, Sound, color);            // Test and draw the icon.
}

