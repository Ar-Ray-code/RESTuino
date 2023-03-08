#include "RESTuino/restuino_func.h"
#include "RESTuino/gpio_utils/gpiotype.hpp"

void setup()
{
  GpioDefaultList table;

                            // input, output, analog_input, touch, pwm, uart_tx, uart_rx
  bool TORCH_DEFALUT = false;
  gpio_available available0 = {true, true, false, TORCH_DEFALUT, true, true, false};
  gpio_available available1 = {true, true, false, TORCH_DEFALUT, true, false, true};
  gpio_available available2 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available3 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available4 = {true, true, false, TORCH_DEFALUT, true, true, false};
  gpio_available available5 = {true, true, false, TORCH_DEFALUT, true, false, true};
  gpio_available available6 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available7 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available8 = {true, true, false, TORCH_DEFALUT, true, true, false};
  gpio_available available9 = {true, true, false, TORCH_DEFALUT, true, false, true};
  gpio_available available10 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available11 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available12 = {true, true, false, TORCH_DEFALUT, true, true, false};
  gpio_available available13 = {true, true, false, TORCH_DEFALUT, true, false, true};
  gpio_available available14 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available15 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available16 = {true, true, false, TORCH_DEFALUT, true, true, false};
  gpio_available available17 = {true, true, false, TORCH_DEFALUT, true, false, true};
  gpio_available available18 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available19 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available20 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available21 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available22 = {true, true, false, TORCH_DEFALUT, true, false, false};
  gpio_available available26 = {true, true, true, TORCH_DEFALUT, true, false, false};
  gpio_available available27 = {true, true, true, TORCH_DEFALUT, true, false, false};
  gpio_available available28 = {true, true, true, TORCH_DEFALUT, true, false, false};

  table.push_back(std::make_tuple(0, available0, gpio_status()));
  table.push_back(std::make_tuple(1, available1, gpio_status()));
  table.push_back(std::make_tuple(2, available2, gpio_status()));
  table.push_back(std::make_tuple(3, available3, gpio_status()));
  table.push_back(std::make_tuple(4, available4, gpio_status()));
  table.push_back(std::make_tuple(5, available5, gpio_status()));
  table.push_back(std::make_tuple(6, available6, gpio_status()));
  table.push_back(std::make_tuple(7, available7, gpio_status()));
  table.push_back(std::make_tuple(8, available8, gpio_status()));
  table.push_back(std::make_tuple(9, available9, gpio_status()));
  table.push_back(std::make_tuple(10, available10, gpio_status()));
  table.push_back(std::make_tuple(11, available11, gpio_status()));
  table.push_back(std::make_tuple(12, available12, gpio_status()));
  table.push_back(std::make_tuple(13, available13, gpio_status()));
  table.push_back(std::make_tuple(14, available14, gpio_status()));
  table.push_back(std::make_tuple(15, available15, gpio_status()));
  table.push_back(std::make_tuple(16, available16, gpio_status()));
  table.push_back(std::make_tuple(17, available17, gpio_status()));
  table.push_back(std::make_tuple(18, available18, gpio_status()));
  table.push_back(std::make_tuple(19, available19, gpio_status()));
  table.push_back(std::make_tuple(20, available20, gpio_status()));
  table.push_back(std::make_tuple(21, available21, gpio_status()));
  table.push_back(std::make_tuple(22, available22, gpio_status()));
  table.push_back(std::make_tuple(26, available26, gpio_status()));
  table.push_back(std::make_tuple(27, available27, gpio_status()));
  table.push_back(std::make_tuple(28, available28, gpio_status()));

  start(table, (byte*)"\x00\x08\xDC\x11\x22\x33", IPAddress(192, 168, 0, 177));
}
void loop()
{
}
