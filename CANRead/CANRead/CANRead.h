// Safe guards
#ifndef CANREAD_H
#define CANREAD_H

// Libraries
#include <stdint.h>
#include <Arduino.h>
#include <FlexCAN_T4.h>

// Prototypes
void SendMessage(CAN_message_t * message);

void ReadMessage(CAN_message_t * message);

void PopulateMessage(CAN_message_t * message, uint8_t DLC, int8_t * messageBuf, uint16_t ID);

void OutputMessageContents(CAN_message_t * message);

// End safe guards
#endif /* CANREAD_H */