#ifndef LAMP_H
#define LAMP_H

#include <string>
#include <iostream>

enum LampState {
    OFF,
    ON
};

class Lamp {
public:
    Lamp();
    void turnOn();
    void turnOff();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void processCommand(uint8_t type, uint16_t length, const uint8_t* value);

private:
    LampState state;
    uint8_t color[3];
};

#endif