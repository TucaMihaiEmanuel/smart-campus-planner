#include "Scheduler.h"
#include <algorithm>
#include <cctype>

bool Scheduler::validDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    const int month = std::stoi(date.substr(5, 2));
    const int day = std::stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

const Room* Scheduler::findRoom(const std::string& id) const {
    const auto it = std::find_if(rooms_.begin(), rooms_.end(),
        [&](const Room& room) { return room.id == id; });
    return it == rooms_.end() ? nullptr : &(*it);
}

bool Scheduler::addRoom(const Room& room) {
    if (room.id.empty() || room.name.empty() || room.capacity <= 0) return false;
    if (findRoom(room.id)) return false;
    rooms_.push_back(room);
    return true;
}

bool Scheduler::isAvailable(const std::string& roomId,
                            const std::string& date,
                            int startHour, int endHour) const {
    if (!findRoom(roomId) || !validDate(date) ||
        startHour < 0 || endHour > 24 || startHour >= endHour) {
        return false;
    }

    for (const auto& existing : reservations_) {
        if (existing.roomId != roomId || existing.date != date) continue;
        // Half-open intervals: [start, end). Adjacent bookings are allowed.
        if (startHour < existing.endHour && existing.startHour < endHour) {
            return false;
        }
    }
    return true;
}

bool Scheduler::addReservation(const Reservation& r) {
    if (r.id <= 0 || r.owner.empty() || !validDate(r.date) ||
        r.startHour < 0 || r.endHour > 24 || r.startHour >= r.endHour) return false;
    if (!findRoom(r.roomId)) return false;
    if (std::any_of(reservations_.begin(), reservations_.end(),
        [&](const Reservation& existing) { return existing.id == r.id; })) return false;
    if (!isAvailable(r.roomId, r.date, r.startHour, r.endHour)) return false;
    reservations_.push_back(r);
    return true;
}

bool Scheduler::cancelReservation(int id) {
    const auto it = std::find_if(reservations_.begin(), reservations_.end(),
        [&](const Reservation& r) { return r.id == id; });
    if (it == reservations_.end()) return false;
    reservations_.erase(it);
    return true;
}
