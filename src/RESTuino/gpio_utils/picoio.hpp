#pragma once

#include <Arduino.h>
#include "../server_utils/server_utils.hpp"

#include "gpio_allocator.hpp"
#include "gpiotype.hpp"

#include <Servo.h>
#include <vector>
#include <algorithm>

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
        touch,
        dacwrite,
        save = 100,
        load,
        reboot,
        not_found,
        ls_free
    };
}


class PicoIO
{
public:
    PicoIO(GpioDefaultList gpio_list)
    {
        gpio_table_pico_ = std::make_unique<GpioTable>(gpio_list);
    }
    ~PicoIO() {}

    GpioMode purpose_convert(restuino_pre::status _status)
    {
        switch (_status)
        {
        case restuino_pre::digitalread:
            return GpioMode::IO_INPUT;
        case restuino_pre::digitalwrite:
            return GpioMode::IO_OUTPUT;
        case restuino_pre::analogread:
            return GpioMode::IO_ANALOG_INPUT;
        case restuino_pre::ledcwrite:
            return GpioMode::IO_PWM;
        case restuino_pre::servo:
            return GpioMode::IO_SERVO;
        case restuino_pre::touch:
            return GpioMode::IO_TOUCH;
        default:
            return GpioMode::IO_NONE;
        }
    }

    GpioMode purpose_convert(uint16_t _status)
    {
        switch (_status)
        {
        case 1:
            return GpioMode::IO_INPUT;
        case 2:
            return GpioMode::IO_OUTPUT;
        case 3:
            return GpioMode::IO_ANALOG_INPUT;
        case 4:
            return GpioMode::IO_PWM;
        case 5:
            return GpioMode::IO_SERVO;
        case 6:
            return GpioMode::IO_TOUCH;
        default:
            return GpioMode::IO_NONE;
        }
    }

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

    template <typename T>
    int detect_index(const std::vector<T> &vec, const T &value)
    {
        auto it = std::find(vec.begin(), vec.end(), value);
        if (it != vec.end())
        {
            return std::distance(vec.begin(), it);
        }
        else
        {
            return -1;
        }
    }

    String ioGet(ServerUtils &server_utils)
    {
        String res;
        int angle;
        uint16_t pin = server_utils.gpio_num;

        if (!gpio_table_pico_->is_allocated(server_utils.gpio_num, purpose_convert(gpio_arr[pin])))
        {
            return server_utils.handle_not_found();
        }

        Serial.println("---- GET ----");
        Serial.println("pin: " + String(pin));
        Serial.println("status: " + String(gpio_arr[pin]));

        Serial.println(gpio_arr[pin]);
        switch (gpio_arr[pin])
        {
        case restuino_pre::servo:
            angle = servos[detect_index<uint8_t>(servo_pins, pin)].read();
            res = server_utils.gen_msg(200, "text/plain", String(angle));
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

        if (status == restuino_pre::ls_free)
        {
            auto gpios = gpio_table_pico_->ls_all();
            res += "GPIO\tMODE\tSTATUS\r\n";
            for (auto gpio : gpios)
            {
                if (std::get<0>(gpio) < 10)
                {
                    res += " ";
                }
                res += String(std::get<0>(gpio));
                res += "\t";
                if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_NONE)
                    res += "FREE";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_INPUT)
                    res += "INPUT";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_OUTPUT)
                    res += "OUTPUT";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_ANALOG_INPUT)
                    res += "ANALOG";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_PWM)
                    res += "PWM";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_SERVO)
                    res += "SERVO";
                else if (std::get<GPIO_STATUS>(gpio).mode == GpioMode::IO_TOUCH)
                    res += "TOUCH";
                else
                    res += "?";
                res += "\r\n";
            }
            res += "----------------\r\n";
            res = server_utils.gen_msg(200, "text/plain", res);
            return res;
        }

        // GPIO availability check
        if (!gpio_table_pico_->allocate(server_utils.gpio_num, purpose_convert(status)))
        {
            return server_utils.handle_not_found();
        }

        switch (gpio_arr[server_utils.gpio_num])
        {
        case restuino_pre::digitalwrite:
            pinMode(server_utils.gpio_num, OUTPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::digitalread:
            pinMode(server_utils.gpio_num, INPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::analogread:
            pinMode(server_utils.gpio_num, INPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::ledcwrite:
            pinMode(server_utils.gpio_num, OUTPUT);
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::servo:
            if (servo_pins.size() > 0)
            {
                if (detect_index<uint8_t>(servo_pins, server_utils.gpio_num) != -1)
                {
                    res = server_utils.handle_not_found();
                    return res;
                }
            }
            servos.push_back(Servo());
            servo_pins.push_back(server_utils.gpio_num);
            servos[servos.size() - 1].attach(server_utils.gpio_num);

            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        default:
            res = server_utils.handle_not_found();
            break;
        }
        return res;
    }

private:
    String ioPut(ServerUtils &server_utils)
    {
        String target = server_utils.data;
        String res;

        Serial.println("---- PUT ----");
        Serial.println("pin: " + String(server_utils.gpio_num));
        Serial.println("status: " + String(target));

        if (!gpio_table_pico_->is_allocated(server_utils.gpio_num, purpose_convert(gpio_arr[server_utils.gpio_num])))
        {
            return server_utils.handle_not_found();
        }


        switch (gpio_arr[server_utils.gpio_num])
        {
        case restuino_pre::digitalwrite:
            if (target == "HIGH")// or target == "1")
            {
                digitalWrite(server_utils.gpio_num, HIGH);
            }
            else if (target == "LOW") // or target == "0")
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
            // 0 ~ 255
            if (server_utils.data.toInt() < 0 || server_utils.data.toInt() > 255)
            {
                Serial.println("error: set 0 ~ 255");
                return server_utils.handle_not_found();
            }
            analogWrite(server_utils.gpio_num, server_utils.data.toInt());
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        case restuino_pre::servo:
            // 0 ~ 180
            if (server_utils.data.toInt() < 0 || server_utils.data.toInt() > 180)
            {
                Serial.println("error: set 0 ~ 180");
                return server_utils.handle_not_found();
            }
            servos[detect_index<uint8_t>(servo_pins, server_utils.gpio_num)].write(server_utils.data.toInt());
            res = server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;

        default:
            res = server_utils.handle_not_found();
            break;
        }

        return res;
    }

    String ioDelete(ServerUtils &server_utils)
    {
        gpio_table_pico_->free(server_utils.gpio_num);

        switch (gpio_arr[server_utils.gpio_num])
        {
        case restuino_pre::servo:
            if (servo_pins.size() > 0)
            {
                if (detect_index<uint8_t>(servo_pins, server_utils.gpio_num) != -1)
                {
                    servos[detect_index<uint8_t>(servo_pins, server_utils.gpio_num)].detach();
                    servos.erase(servos.begin() + detect_index<uint8_t>(servo_pins, server_utils.gpio_num));
                    servo_pins.erase(servo_pins.begin() + detect_index<uint8_t>(servo_pins, server_utils.gpio_num));
                    return server_utils.gen_msg(200, "text/plain", "Erase OK\r\n");
                }
            }
        default:
            return server_utils.gen_msg(200, "text/plain", "OK\r\n");
            break;
        }
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
        else if (req == "ledcWrite" || req == "pwm")
            return restuino_pre::ledcwrite;
        else if (req == "Servo")
            return restuino_pre::servo;
        else if (req == "save")
            return restuino_pre::save;
        else if (req == "load")
            return restuino_pre::load;
        else if (req == "reboot")
            return restuino_pre::reboot;
        else if (req == "ls")
            return restuino_pre::ls_free;
        else
            return restuino_pre::not_found;
    }
private:
    // 10 servos
    std::vector<Servo> servos;
    std::vector<uint8_t> servo_pins;
    std::unique_ptr<GpioTable> gpio_table_pico_;
    // Servo servo;
};
