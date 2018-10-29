#include "external/PigpioMS/PigpioMS.hpp"
#include "external/clipp/include/clipp.h"
#include <iostream>
#include <string>

enum class action { help, send, scan, reset_id };

std::optional<std::string> port;

namespace send {
unsigned char id;
unsigned char cmd;
short value;
auto run() {
  auto ms =
      port ? RPMS::MotorSerial{4, 10, port->c_str()} : RPMS::MotorSerial{};
  ms.init();
  ms.send(id, cmd, value);
}
} // namespace send

namespace scan {
auto run() { throw "TODO: not implemented yet"; }
} // namespace scan

namespace reset_id {
unsigned char new_id;
std::optional<unsigned char> current_id;
constexpr unsigned char cmd_reset_id = 254;
auto run() {
  auto ms =
      port ? RPMS::MotorSerial{4, 10, port->c_str()} : RPMS::MotorSerial{};
  ms.init();
  ms.send(current_id.value_or(255), cmd_reset_id, new_id);
}
} // namespace reset_id

auto main(int argc, char *argv[]) -> int {
  using clipp::value, clipp::option, clipp::command, clipp::parse,
      clipp::usage_lines;

  auto action = action::help;

  auto send = (command("send").set(action, action::send),
               value("id").set(send::id) & value("cmd").set(send::cmd) &
                   value("value").set(send::value));

  auto scan = (command("scan").set(action, action::scan));

  auto reset_id =
      (command("reset") & command("id").set(action, action::reset_id),
       value("new id").set(reset_id::new_id),
       option("--from") & value("current id").set(reset_id::current_id));

  auto inoclient = (send | scan | reset_id, option("--port") & value("path"));

  if (!parse(argc, argv, inoclient)) {
    std::cout << "Error: invalid arguments\n";
    action = action::help;
  }

  switch (action) {
  case action::help:
    std::cout << "Usage:\n" << usage_lines(inoclient, "inoclient") << std::endl;
    break;
  case action::send:
    send::run();
    break;
  case action::scan:
    scan::run();
    break;
  case action::reset_id:
    reset_id::run();
    break;
  }
}
