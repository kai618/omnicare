#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(6,7,8); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  Serial.begin(57600);
  Serial.print(__DATE__);
  Serial.println(__TIME__);
}

void loop() {
  // put your main code here, to run repeatedly:

}
