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


void NTP_EEPROM_write(byte SL_AD, byte BL_AD1, byte BL_AD0, byte DATA0, byte DATA1, byte DATA2, byte DATA3){
  Serial.print("Writing "); 
  Serial.print(DATA0, HEX); Serial.print(", "); Serial.print(DATA1, HEX); Serial.print(", "); Serial.print(DATA2, HEX); Serial.print(", "); Serial.print(DATA3, HEX);
  Serial.print(" to address ");
  Serial.print(BL_AD1); Serial.println(BL_AD0);

  
  //WRITE DATA p. 102
  Wire.beginTransmission(SL_AD); 
  
  Wire.write(BL_AD1); // sends first (most significant) bytes of adress
  Wire.write(BL_AD0); // sends second (least significant) bytes of adress
  
  // Sends data to be stored
  // 4 Data blocks should be sent for writing to EEPROM
  Wire.write(DATA0); //DATA 0
  Wire.write(DATA1); //DATA 1
  Wire.write(DATA2); //DATA 2
  Wire.write(DATA3); //DATA 3 

  Wire.endTransmission();    // stop transmitting

}
void NTP_EEPROM_read(uint8_t SL_AD, uint8_t BL_AD1, uint8_t BL_AD0){

  Serial.print("Reading data from adress ");
  Serial.print(BL_AD1); Serial.println(BL_AD0);

  //READ DATA p. 102
  //Prepare adress
  Wire.beginTransmission(SL_AD); // Write start adress 

  Wire.write(BL_AD1); // sends first (most significant) bytes of adress
  Wire.write(BL_AD0); // sends second (least significant) bytes of adress
  
  Wire.endTransmission(); // Send device adress, memmory adress and a stop bit

  Wire.requestFrom(SL_AD, (uint8_t)4);    // Wait for 4 bytes from the device (NTP5332)

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read();   // Read bytes as char
    Serial.print(c, HEX);      // Print the character
    Serial.print(", ");
  }
  Serial.println("");
}



void setup() {
  Serial.begin(9600); // Start serial interface 

  Wire.begin(); // join I2C bus (address optional for master)

  pinMode(ED_PIN, INPUT); // Low when RF-filed is presented 
  

}


void loop() {

delay(5000);
NTP_EEPROM_read(NTP_ADDRESS, 0x00, 0x00);
NTP_EEPROM_write(NTP_ADDRESS, 0x00, 0x00, 0x00,0x00,0x00,0x00);
NTP_EEPROM_read(NTP_ADDRESS, 0x00, 0x00);
delay(5000);
NTP_EEPROM_write(NTP_ADDRESS, 0x00, 0x00, 0x01,0x02,0x03,0x04);


  /*
 //"Software interrupt"
  if(!digitalRead(ED_PIN)){ // If filed is presented
    ED_ISR();
  } 
  */



}