#include <SPI.h>
#include <RH_RF95.h>

#if defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)  // Feather M0 w/Radio
  #define RFM95_CS    8
  #define RFM95_INT   3
  #define RFM95_RST   4
#endif

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    while (1);
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1);
  }
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop() {
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  float radiopacket[10] = {3.33, 3.33, 3.33, 5.43, 1.97, 8.21, 0.21, 9.54, 7.43, 5.01};
  String arrayString = floatToString(radiopacket, 10, ',');

  char charBuffer[arrayString.length() + 1]; // +1 for null terminator
  arrayString.toCharArray(charBuffer, arrayString.length() + 1);
  char hello[6] = "hello";

  // Send the data over LoRa
  rf95.send((uint8_t *)charBuffer, arrayString.length());
  delay(50);
  //rf95.send((uint8_t *)hello, 6);

  delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; 
  uint8_t len = sizeof(buf);
 
  if (rf95.waitAvailableTimeout(1000)) {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len)) {
    } else {
    }
  } else {
  }

}

String floatToString(float *array, int length, char delimiter) {
  String result = "";
  // Loop through each element in the array
  for (int i = 0; i < length; i++) {
    // Convert the float value to a string and append it to the result
    result += String(array[i]);
    // Add delimiter unless it's the last element
    if (i < length - 1) {
      result += delimiter;
    }
  }
  return result;
}
