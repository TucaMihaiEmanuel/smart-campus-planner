#pragma once
#include <string>

struct Reservation {
    int id{};
    std::string roomId;
    std::string date;      // YYYY-MM-DD
    int startHour{};       // 0..23
    int endHour{};         // 1..24, exclusive
    std::string owner;
};
