#pragma once

#include <Arduino.h>

#include "gpiotype.hpp"

bool gpio_is_available(gpio_available &gpio_available, GpioMode mode)
{
    switch (mode)
    {
    case IO_INPUT:
        return gpio_available.input;
    case IO_INPUT_PULLUP:
        return gpio_available.input;
    case IO_OUTPUT:
        return gpio_available.output;
    case IO_ANALOG_INPUT:
        return gpio_available.analog_input;
    case IO_PWM:
        return gpio_available.pwm;
    case IO_SERVO:
        return gpio_available.pwm;
    case IO_TOUCH:
        return gpio_available.touch;
    case IO_UART_TX:
        return gpio_available.uart_tx;
    case IO_UART_RX:
        return gpio_available.uart_rx;
    default:
        return false;
    }
}


class GpioTable
{
public:
    GpioTable(GpioDefaultList gpio_list)
    {
        gpio_table_ = gpio_list;
    }

    std::vector<uint8_t> get_available_gpio(GpioMode mode)
    {
        std::vector<uint8_t> available_gpio;
        for (auto gpio : gpio_table_)
        {
            if (std::get<GPIO_PURPOSE>(gpio).input && mode == IO_INPUT)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).output && mode == IO_OUTPUT)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).analog_input && mode == IO_ANALOG_INPUT)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).touch && mode == IO_TOUCH)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).pwm && mode == IO_PWM)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).pwm && mode == IO_SERVO)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).uart_tx && mode == IO_TOUCH)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
            else if (std::get<GPIO_PURPOSE>(gpio).uart_rx && mode == IO_UART_RX)
            {
                available_gpio.push_back(std::get<PIN>(gpio));
            }
        }
        return available_gpio;
    }

    std::vector<uint8_t> ls_gpio(GpioMode grep_mode)
    {
        std::vector<uint8_t> gpio_list;
        for (auto gpio : gpio_table_)
        {
            if (grep_mode == IO_INPUT && std::get<GPIO_PURPOSE>(gpio).input)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_OUTPUT && std::get<GPIO_PURPOSE>(gpio).output)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_ANALOG_INPUT && std::get<GPIO_PURPOSE>(gpio).analog_input)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_TOUCH && std::get<GPIO_PURPOSE>(gpio).touch)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_PWM && std::get<GPIO_PURPOSE>(gpio).pwm)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_SERVO && std::get<GPIO_PURPOSE>(gpio).pwm)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_TOUCH && std::get<GPIO_PURPOSE>(gpio).uart_tx)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
            else if (grep_mode == IO_UART_RX && std::get<GPIO_PURPOSE>(gpio).uart_rx)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
        }
        return gpio_list;
    }

    std::vector<uint8_t> ls_free()
    {
        std::vector<uint8_t> gpio_list;
        for (auto gpio : gpio_table_)
        {
            Serial.print("GPIO is: ");
            Serial.println(std::get<GPIO_STATUS>(gpio).using_flag);
            if (!std::get<GPIO_STATUS>(gpio).using_flag)
            {
                gpio_list.push_back(std::get<PIN>(gpio));
            }
        }
        return gpio_list;
    }

    GpioDefaultList ls_all()
    {
        return gpio_table_;
    }

    bool is_free(uint8_t gpio)
    {
        for (auto gpio_info : gpio_table_)
        {
            if (std::get<PIN>(gpio_info) == gpio)
            {
                return !std::get<GPIO_STATUS>(gpio_info).using_flag;
            }
        }
        return false;
    }

    bool is_allocated(uint8_t gpio, GpioMode mode)
    {
        for (auto gpio_info : gpio_table_)
        {
            if (std::get<PIN>(gpio_info) == gpio &&
                std::get<GPIO_STATUS>(gpio_info).using_flag &&
                std::get<GPIO_STATUS>(gpio_info).mode == mode)
            {
                return true;
            }
        }
        return false;
    }

    bool allocate(uint8_t gpio, GpioMode mode)
    {
        Serial.print("allocate:");
        Serial.println(gpio);

        int index = 0;
        for (auto gpio_info : gpio_table_)
        {
            Serial.print("Search:");
            Serial.println(std::get<PIN>(gpio_info));
            if (std::get<PIN>(gpio_info) == gpio)
            {
                Serial.print("Found:");
                Serial.println(std::get<PIN>(gpio_info));

                if (std::get<GPIO_PURPOSE>(gpio_info).input)
                {
                    Serial.println("input");
                }
                else if (std::get<GPIO_PURPOSE>(gpio_info).output)
                {
                    Serial.println("output");
                }
                else if (std::get<GPIO_PURPOSE>(gpio_info).analog_input)
                {
                    Serial.println("analog_input");
                }
                else if (std::get<GPIO_PURPOSE>(gpio_info).pwm)
                {
                    Serial.println("pwm");
                }


                if (!std::get<GPIO_STATUS>(gpio_info).using_flag)
                {
                    std::get<GPIO_STATUS>(gpio_table_[index]).using_flag = true;
                    std::get<GPIO_STATUS>(gpio_table_[index]).mode = mode;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            index++;
        }
        return false;
    }

    void free(uint8_t gpio)
    {
        for (auto gpio_info : gpio_table_)
        {
            int index = 0;
            if (std::get<PIN>(gpio_info) == gpio)
            {
                if (!std::get<GPIO_STATUS>(gpio_info).using_flag)
                {
                    Serial.println("GPIO is not using");
                }
                Serial.print("deleting .... ");
                Serial.println(std::get<PIN>(gpio_info));
                std::get<GPIO_STATUS>(gpio_table_[std::get<PIN>(gpio_info)]) = gpio_status();
                return;
            }
            index++;
        }
    }

private:
    GpioDefaultList gpio_table_;
};
