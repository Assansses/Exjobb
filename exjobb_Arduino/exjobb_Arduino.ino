/*********************************************************************************************
* Assar Johansson, 2025-04-14
*
* This is a demo to read and wright to the NTP5332 via I2C with the wire library (https://docs.arduino.cc/language-reference/en/functions/communication/wire/)
* Page and figure numbers refer to the NTP5332 datasheet (https://www.nxp.com/docs/en/data-sheet/NTP53x2.pdf)
*
*
***********************************************************************************************/
#include <Wire.h>

// Device adressing described in 8.3.1.1.5 on page 100
#define NTP_ADDRESS 0x54 // The Wire library adds W/R-bit in its functions
#define ED_PIN 5



void ED_ISR(){
  Serial.println("ED_ISR has been fired");
  while(!digitalRead(ED_PIN)){} // While RF-filed is presented
  Serial.println("Exiting ED_ISR");
}



void setup() {
  Serial.begin(9600); // Start serial interface 

  Wire.begin(); // join I2C bus (address optional for master)

  pinMode(ED_PIN, INPUT); // Low when RF-filed is presented 
  

}


void loop() {
  
 //"Software interrupt"
  if(!digitalRead(ED_PIN)){ // If filed is presented
    ED_ISR();
  } 



}
/*
  Serial.println("Writing data (AA) to adress 00 02");
  
  //WRITE DATA p. 102
  Wire.beginTransmission(NTP_ADDRESS); 
  
  Wire.write(0x00); // sends first (most significant) bytes of adress
  Wire.write(0x02); // sends second (least significant) bytes of adress
  
  // Sends data to be stored
  // 4 Data blocks should be sent for writing to EEPROM
  Wire.write(0xAB); //DATA 0
  Wire.write(0xBA); //DATA 1
  Wire.write(0xFA); //DATA 2
  Wire.write(0xBA); //DATA 3 

  Wire.endTransmission();    // stop transmitting

  delay(500);


  Serial.println("Reading data from adress 00 02");
  //READ DATA p. 102
  //Prepare adress
  Wire.beginTransmission(NTP_ADDRESS); // Write start adress 

  Wire.write(0x00); // sends first (most significant) bytes of adress
  Wire.write(0x02); // sends second (least significant) bytes of adress

  Wire.endTransmission(); // Send device adress, memmory adress and a stop bit

  Wire.requestFrom(NTP_ADDRESS, 16);    // Wait for 16 bytes from the device (NTP5332)

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read();   // Read bytes as char
    Serial.println(c);      // Print the character
  }
  Serial.println("Done");

*/
}