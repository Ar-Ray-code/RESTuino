#pragma once

#include <Arduino.h>

#define _ESP32 0
#define _RPICO 1

#define BOARD_TYPE _RPICO

#if defined(ESP32)
#include <WiFi.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <WebServer.h>

#endif

#include <ArduinoJson.h>

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

class RestuinoFunc
{
public:
  void restuino_setup();
  void restuino_loop();

private:
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
};



void start();
void loop_func();