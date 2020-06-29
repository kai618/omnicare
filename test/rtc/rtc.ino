#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(5,4,6); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  Serial.begin(57600);
  Serial.print(__DATE__);
  Serial.println(__TIME__);
}

void loop() {
  // put your main code here, to run repeatedly:

}
