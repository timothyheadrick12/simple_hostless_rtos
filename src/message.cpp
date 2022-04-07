#include "message.h"

Message::~Message() {
    delete message;
}

byte* Message::send() const{
    //digitalWrite(MIO, HIGH);
}

byte* Message::receive() {

}