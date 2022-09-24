#pragma once

#include <Arduino.h>
#include "../server_utils/server_utils.hpp"

namespace restuino_pre
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

// static uint16_t gpio_arr[40] = {}; //すべて0で初期化

class PicoIO
{
public:
    PicoIO() {}
    ~PicoIO() {}

    String ioUpdate(ServerUtils &server_utils)
    {
        switch (server_utils.mode)
        {
        case MODE_GET:
            return ioGet(server_utils);
        case MODE_POST:
            return ioPost(server_utils);
        case MODE_PUT:
            return ioPut(server_utils);
        case MODE_DELETE:
            return ioDelete(server_utils);
        default:
            return server_utils.handle_not_found();
        }
    }

    String ioGet(ServerUtils &server_utils)
    {
        String res;
        uint16_t pin = server_utils.gpio_num;

        Serial.println(gpio_arr[pin]);
        switch (gpio_arr[pin])
        {
        case restuino_pre::servo:
            res = server_utils.gen_msg(200, "text/plain", String(gpio_arr[pin]) + "\r\n");
            break;

        case restuino_pre::digitalread:
            res = server_utils.gen_msg(200, "text/plain", String(digitalRead(pin)) + "\r\n");
            break;

        case restuino_pre::analogread:
            res = server_utils.gen_msg(200, "text/plain", String(analogRead(pin)) + "\r\n");
            break;

        default:
            res = server_utils.handle_not_found();
            break;
        }
        return res;
    }

    String ioPost(ServerUtils &server_utils)
    {
        String res;
        restuino_pre::status status = request_to_num(server_utils.data);
        gpio_arr[server_utils.gpio_num] = status;
        // if digitalWrite
        switch (gpio_arr[server_utils.gpio_num])
        {
        case restuino_pre::digitalwrite:
            pinMode(server_utils.gpio_num, OUTPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::analogread:
            pinMode(server_utils.gpio_num, INPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::ledcwrite:
            break;
        case restuino_pre::servo:
            pinMode(server_utils.gpio_num, OUTPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        default:
            res = server_utils.handle_not_found();
            break;
        }
        return res;
    }

    String ioPut(ServerUtils &server_utils)
    {
        String target = server_utils.data;
        String res;
        switch (gpio_arr[server_utils.gpio_num])
        {
        case restuino_pre::digitalwrite:
            if (target == "HIGH" or target == "1")
            {
                digitalWrite(server_utils.gpio_num, HIGH);
            }
            else if (target == "LOW" or target == "0")
            {
                digitalWrite(server_utils.gpio_num, LOW);
            }
            else
            {
                return server_utils.handle_not_found();
            }
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;

        case restuino_pre::ledcwrite:
            // pwm
            analogWrite(server_utils.gpio_num, server_utils.data.toInt());
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;

        default: // nan, not found ○
            res = server_utils.handle_not_found();
            break;
        }

        return res;
    }

    String ioDelete(ServerUtils &server_utils)
    {
        return server_utils.handle_not_found();
    }

    restuino_pre::status request_to_num(String req)
    {
        if (req == "nan")
            return restuino_pre::nan;
        else if (req == "digitalRead")
            return restuino_pre::digitalread;
        else if (req == "digitalWrite")
            return restuino_pre::digitalwrite;
        else if (req == "analogRead")
            return restuino_pre::analogread;
        else if (req == "ledcWrite")
            return restuino_pre::ledcwrite;
        else if (req == "Servo")
            return restuino_pre::servo;
        else if (req == "save")
            return restuino_pre::save;
        else if (req == "load")
            return restuino_pre::load;
        else if (req == "reboot")
            return restuino_pre::reboot;
        else
            return restuino_pre::not_found;
    }
};
