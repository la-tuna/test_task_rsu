#include "Lamp.h"

Lamp::Lamp() : state(OFF) {
    color[0] = color[1] = color[2] = 0;
}

void Lamp::turnOn() {
    state = ON;
    std::cout << "Lamp is ON" << std::endl;
}

void Lamp::turnOff() {
    state = OFF;
    std::cout << "Lamp is OFF" << std::endl;
}

void Lamp::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    std::cout << "Lamp color set to RGB("
              << static_cast<int>(red) << ", "
              << static_cast<int>(green) << ", "
              << static_cast<int>(blue) << ")"
              << std::endl;
}

// Обработка команды управления фонарем
void Lamp::processCommand(uint8_t type, uint16_t length, const uint8_t* value) {
    switch (type) {
    case 0x12:
        if (length == 0) turnOn();
        break;
    case 0x13:
        if (length == 0) turnOff();
        break;
    case 0x20:
        if (length == 3) setColor(value[0], value[1], value[2]);
        break;
    default:
        break;
    }
}