#include "Storage.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace {
std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) fields.push_back(field);
    return fields;
}
}

bool Storage::save(const Scheduler& scheduler,
                   const std::string& roomsPath,
                   const std::string& reservationsPath) {
    std::ofstream roomsFile(roomsPath);
    std::ofstream reservationsFile(reservationsPath);
    if (!roomsFile || !reservationsFile) return false;

    roomsFile << "id,name,capacity\n";
    for (const auto& room : scheduler.rooms())
        roomsFile << room.id << ',' << room.name << ',' << room.capacity << '\n';

    reservationsFile << "id,roomId,date,startHour,endHour,owner\n";
    for (const auto& r : scheduler.reservations())
        reservationsFile << r.id << ',' << r.roomId << ',' << r.date << ','
                         << r.startHour << ',' << r.endHour << ',' << r.owner << '\n';
    return static_cast<bool>(roomsFile) && static_cast<bool>(reservationsFile);
}

bool Storage::load(Scheduler& scheduler,
                   const std::string& roomsPath,
                   const std::string& reservationsPath) {
    std::ifstream roomsFile(roomsPath), reservationsFile(reservationsPath);
    if (!roomsFile || !reservationsFile) return false;

    Scheduler loaded;
    std::string line;
    std::getline(roomsFile, line); // header
    while (std::getline(roomsFile, line)) {
        if (line.empty()) continue;
        const auto f = split(line);
        if (f.size() != 3) return false;
        try {
            if (!loaded.addRoom({f[0], f[1], std::stoi(f[2])})) return false;
        } catch (...) { return false; }
    }

    std::getline(reservationsFile, line); // header
    while (std::getline(reservationsFile, line)) {
        if (line.empty()) continue;
        const auto f = split(line);
        if (f.size() != 6) return false;
        try {
            Reservation r{std::stoi(f[0]), f[1], f[2], std::stoi(f[3]),
                          std::stoi(f[4]), f[5]};
            if (!loaded.addReservation(r)) return false;
        } catch (...) { return false; }
    }

    scheduler = loaded;
    return true;
}
