#include <SPI.h>
#include <RH_RF95.h>
#include <List.hpp>

#if defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)
  #define RFM95_CS    8
  #define RFM95_INT   3
  #define RFM95_RST   4
#endif

#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);
List<float> psiList;
const int PRESSURE_PIN = A1;

// Timers
unsigned long add_data_millis = 0;
const long ADD_DATA_INTERVAL = 2000;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  digitalWrite(RFM95_RST, LOW);
  digitalWrite(RFM95_RST, HIGH);

  while (!rf95.init()) {
    while (1); // hang forever if radio doesn't init
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1); // hang forever if freq not set
  }
  rf95.setTxPower(23, false);
}

void loop() {
  unsigned long current_millis = millis();

  // Every 2 seconds, add raw read voltage to list
  if (current_millis - add_data_millis >= ADD_DATA_INTERVAL) {
    add_data_millis = current_millis;

    float pressure_voltage = analogRead(PRESSURE_PIN);
    psiList.add(pressure_voltage);

    // Convert list to string
    String arrayString = floatToString(psiList, ',');
    // Conver string to array of chars
    char charBuffer[arrayString.length() + 1];
    arrayString.toCharArray(charBuffer, arrayString.length() + 1);

    rf95.send((uint8_t *)charBuffer, arrayString.length());
    // rf95.waitPacketSent(); might keep depending on how it sends the data in the water
  }
}

// Convert list to a string with comma seperation
String floatToString(List<float> &list, char seperator) {
  String result = "";
  for (int i = 0; i < list.getSize(); i++) {
    result = result + i + ": " + String(list.get(i));
    if (i < list.getSize() - 1) {
      result += seperator;
    }
  }
  return result;
}
