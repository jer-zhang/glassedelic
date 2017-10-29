// **********************************************************
// * MiscFunctions                                          *
// * This contains misc. functions used by other parts of   *
// * the program.                                           * 
// **********************************************************

// --------------------------------------------------
// | getStringLength                                |
// | ---------------------------------------------- |
// | Returns the length of a string.                |
// --------------------------------------------------
int getStringLength(const char *str) {
  uint32_t Length = 0;                        // Clear a variable. 
  while (*str != '\0') { str++; Length++; }   // Count the string. 
  return Length;                              // Return. 
}

