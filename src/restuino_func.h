#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

const char *host_name;
const uint8_t n;
uint16_t gpio_arr[];
const uint32_t minUs;
const uint32_t maxUs;
const uint8_t angle0;
const uint8_t angle;

namespace restuino
{
  enum status
  {
    nan = 0,
    digitalread,
    digitalwrite,
    analogread,
    ledcwrite,
    servo,
    touch,    // kore
    dacwrite, // kore
    save = 100,
    load,
    reboot,
    not_found,
  };
}

enum restuino::status request_to_num(String req);
void handle_not_found(void);
bool to0_flag();
void move_sg90(bool mode, uint8_t to_angle);
String read_eeprom();
bool put_to_control(uint8_t pin, String target);
bool post_to_setup(uint8_t pin, uint8_t setup_mode);
void load_status();
void put_to_control_root(uint8_t setup_mode);
String ip_to_String(uint32_t ip);
void handle_root(void);
void handle_gpio(int pin);
extern void restuino_setup();
extern void restuino_loop();