#pragma once

#include <Arduino.h>
#include <vector>
#include <tuple>

#define GPIO_MAX 40

#define PIN 0
#define GPIO_PURPOSE 1
#define GPIO_STATUS 2

enum GpioMode
{
    IO_NONE = 0,
    IO_INPUT,
    IO_INPUT_PULLUP,
    IO_OUTPUT,
    IO_ANALOG_INPUT,
    IO_PWM,
    IO_SERVO,
    IO_TOUCH,
    IO_UART_TX,
    IO_UART_RX
};

struct gpio_available
{
    bool input = false;
    bool output = false;
    bool analog_input = false;
    bool touch = false;
    bool pwm = false;
    bool uart_tx = false;
    bool uart_rx = false;
};

struct gpio_status
{
    GpioMode mode = GpioMode::IO_NONE;
    bool using_flag = false;
    String status_str = "";
};


typedef std::vector<std::tuple<uint8_t, gpio_available, gpio_status>> GpioDefaultList;