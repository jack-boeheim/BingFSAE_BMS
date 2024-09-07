#include "CANRead.h"

// Send a CAN message
void SendMessage(CAN_message_t * msg) {
    Can1.write(*msg);
}

// Read a CAN message
void ReadMessage(CAN_message_t * msg) {
    Can1.read(*msg);
    OutputMessageContents(msg);
}

void PopulateMessage(CAN_message_t * message, uint8_t DLC, int8_t * messageBuf, uint16_t ID) {
    // Head of Message
    message->flags.extended  =  0;
    message->flags.remote    =  0;
    message->flags.overrun   =  0;
    message->flags.reserved  =  0;
    message->id              =  ID;
    message->len             =  DLC;

   // Populate data buffer
   for (int i = 0; i < DLC; ++i) {
      message->buf[i] = messageBuf[i];
   }
}

// Output CAN message contents to serial monitor
void OutputMessageContents(CAN_message_t * msg) {
    Serial.print("  ID: 0x"); Serial.print(msg->id, HEX );
    Serial.print("  LEN: "); Serial.print(msg->len);
    Serial.print(" DATA: ");

    // Output all contents of message
    for (int i = 0; i < msg->len; ++i) {
        Serial.print(msg->buf[i], HEX);
        Serial.print(" ");
    }

    Serial.print("  TS: "); Serial.println(msg->timestamp);
    Serial.println();
}