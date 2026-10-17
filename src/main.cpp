#include "Scheduler.h"
#include "Storage.h"
#include <iostream>
#include <limits>

namespace {
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Please enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
std::string readText(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}
void listRooms(const Scheduler& s) {
    if (s.rooms().empty()) { std::cout << "No rooms yet.\n"; return; }
    for (const auto& r : s.rooms())
        std::cout << r.id << " | " << r.name << " | " << r.capacity << " seats\n";
}
void listReservations(const Scheduler& s) {
    if (s.reservations().empty()) { std::cout << "No reservations yet.\n"; return; }
    for (const auto& r : s.reservations())
        std::cout << '#' << r.id << " | " << r.roomId << " | " << r.date
                  << " | " << r.startHour << ":00-" << r.endHour
                  << ":00 | " << r.owner << '\n';
}
}

int main() {
    Scheduler scheduler;
    const std::string roomsPath = "data/rooms.csv";
    const std::string reservationsPath = "data/reservations.csv";

    if (!Storage::load(scheduler, roomsPath, reservationsPath)) {
        scheduler.addRoom({"R101", "Study Room 101", 12});
        scheduler.addRoom({"R204", "Quiet Room 204", 6});
        scheduler.addRoom({"LAB1", "Computer Lab", 24});
    }

    while (true) {
        std::cout << "\n=== SMART CAMPUS PLANNER ===\n"
                  << "1. List rooms\n2. Search available rooms\n"
                  << "3. Create reservation\n4. List reservations\n"
                  << "5. Cancel reservation\n6. Save data\n0. Exit\n";
        const int choice = readInt("Choice: ");

        if (choice == 0) {
            if (!Storage::save(scheduler, roomsPath, reservationsPath))
                std::cout << "Warning: could not save data. Ensure data/ exists.\n";
            std::cout << "Goodbye!\n";
            break;
        } else if (choice == 1) {
            listRooms(scheduler);
        } else if (choice == 2) {
            const auto date = readText("Date (YYYY-MM-DD): ");
            const int start = readInt("Start hour (0-23): ");
            const int end = readInt("End hour (1-24): ");
            const int capacity = readInt("Minimum capacity: ");
            bool found = false;
            for (const auto& room : scheduler.rooms()) {
                if (room.capacity >= capacity &&
                    scheduler.isAvailable(room.id, date, start, end)) {
                    std::cout << room.id << " | " << room.name << " | "
                              << room.capacity << " seats\n";
                    found = true;
                }
            }
            if (!found) std::cout << "No rooms match that request.\n";
        } else if (choice == 3) {
            const int id = readInt("Reservation ID (positive integer): ");
            const auto room = readText("Room ID: ");
            const auto date = readText("Date (YYYY-MM-DD): ");
            const int start = readInt("Start hour (0-23): ");
            const int end = readInt("End hour (1-24): ");
            const auto owner = readText("Your name/team: ");
            if (scheduler.addReservation({id, room, date, start, end, owner}))
                std::cout << "Reservation created.\n";
            else
                std::cout << "Could not create reservation. Check room, date, "
                             "hours, ID and conflicting bookings.\n";
        } else if (choice == 4) {
            listReservations(scheduler);
        } else if (choice == 5) {
            const int id = readInt("Reservation ID to cancel: ");
            std::cout << (scheduler.cancelReservation(id) ? "Cancelled.\n" : "ID not found.\n");
        } else if (choice == 6) {
            std::cout << (Storage::save(scheduler, roomsPath, reservationsPath)
                          ? "Data saved.\n" : "Save failed. Ensure data/ exists.\n");
        } else {
            std::cout << "Unknown option. Try again.\n";
        }
    }
    return 0;
}
