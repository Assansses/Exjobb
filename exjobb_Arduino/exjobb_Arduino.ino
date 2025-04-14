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


void NTP_write(byte SL_AD=NTP_ADDRESS, byte BL_AD1=0x00, byte BL_AD0=0x00, int N=1, ...){

  int l_ParamVal = 0x00;

  va_list l_Arg;
  va_start(l_Arg, N);



  Serial.println("Writing data (AA) to adress 00 02");
  
  //WRITE DATA p. 102
  Wire.beginTransmission(SL_AD); 
  
  Wire.write(BL_AD1); // sends first (most significant) bytes of adress
  Wire.write(BL_AD0); // sends second (least significant) bytes of adress
  
  // Sends data to be stored
  for ( int i; i < N; ++i) {
    Wire.write(va_arg(l_Arg, byte)); 

  }
  Wire.endTransmission();    // stop transmitting

  va_end(l_Arg);

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

  //NTP_write(NTP_ADDRESS, 0x00, 0x02, 4, 0x01, 0x02, 0x03, 0x04);

  delay(10);



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
    Serial.print(c);      // Print the character
    Serial.print(", ");
  }
  Serial.println("Done");


}