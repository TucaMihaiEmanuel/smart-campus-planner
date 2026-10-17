#pragma once
#include "Room.h"
#include "Reservation.h"
#include <string>
#include <vector>

class Scheduler {
public:
    bool addRoom(const Room& room);
    bool addReservation(const Reservation& reservation);
    bool cancelReservation(int id);

    bool isAvailable(const std::string& roomId,
                     const std::string& date,
                     int startHour, int endHour) const;

    const std::vector<Room>& rooms() const { return rooms_; }
    const std::vector<Reservation>& reservations() const { return reservations_; }
    const Room* findRoom(const std::string& id) const;

private:
    std::vector<Room> rooms_;
    std::vector<Reservation> reservations_;
    static bool validDate(const std::string& date);
};
