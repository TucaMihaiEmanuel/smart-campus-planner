#include "Scheduler.h"
#include <cassert>
#include <iostream>

int main() {
    Scheduler s;
    assert(s.addRoom({"R1", "Study Room", 8}));
    assert(!s.addRoom({"R1", "Duplicate ID", 10}));
    assert(!s.addRoom({"R2", "Invalid capacity", 0}));

    assert(s.addReservation({1, "R1", "2026-10-20", 14, 16, "Team A"}));
    assert(!s.addReservation({2, "R1", "2026-10-20", 15, 17, "Team B"}));
    assert(s.addReservation({3, "R1", "2026-10-20", 16, 18, "Team C"}));
    assert(!s.addReservation({4, "MISSING", "2026-10-20", 10, 11, "Team D"}));
    assert(!s.addReservation({5, "R1", "2026-10-20", 10, 10, "Team E"}));
    assert(!s.addReservation({1, "R1", "2026-10-21", 10, 11, "Duplicate ID"}));
    assert(s.cancelReservation(1));
    assert(!s.cancelReservation(999));
    assert(s.isAvailable("R1", "2026-10-20", 14, 16));

    std::cout << "All scheduler tests passed.\n";
}
