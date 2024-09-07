#include "CANRead.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

CAN_message_t msgRX;
CAN_message_t msgTX;

// int8_t msgBuf[3];

void setup() {
    // Serial comms
    Serial.begin(9600);
    while (!Serial) {}

    // Initialize CAN
    Can1.begin();
    Can1.setBaudRate(500000);

    // delay(500);
}

void loop() {
    // Add contents/data to message before sending
    // PopulateMessage(msgTX, ... );

    // Send a CAN message
    SendMessage(&msgTX);

    // Read a CAN message
    // ReadMessage(&msgRX);

    // delay(100);
}