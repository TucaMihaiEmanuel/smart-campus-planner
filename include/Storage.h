#pragma once
#include "Scheduler.h"
#include <string>

namespace Storage {
    bool save(const Scheduler& scheduler,
              const std::string& roomsPath,
              const std::string& reservationsPath);
    bool load(Scheduler& scheduler,
              const std::string& roomsPath,
              const std::string& reservationsPath);
}
