# Smart Campus Planner

A C++17 command-line application for managing campus study rooms and preventing overlapping reservations.

## Features

- List available rooms and capacities
- Search rooms by date, time interval, and minimum capacity
- Create and cancel reservations
- Reject overlapping bookings and duplicate IDs
- Persist rooms and reservations to CSV
- Automated tests for scheduling edge cases

## Requirements

- A C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.16 or newer

## Build and run

```bash
cmake -S . -B build
cmake --build build
./build/smart-campus
```

On Windows, run `build\\Debug\\smart-campus.exe` or `build\\Release\\smart-campus.exe`, depending on your generator.

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

## Data format

The application uses `data/rooms.csv` and `data/reservations.csv`. CSV fields currently must not contain commas.

Dates use `YYYY-MM-DD`. Reservations use whole-hour intervals from 0 to 24. The end time is exclusive, so 14:00–16:00 and 16:00–18:00 can be booked consecutively.

## Design

The scheduler stores rooms and reservations in vectors. To detect a conflict, it checks whether `newStart < existingEnd && existingStart < newEnd` for reservations with the same room and date. This makes adjacent bookings legal while rejecting actual overlaps.

## Limitations

- Date validation checks basic `YYYY-MM-DD` structure and month/day ranges, not leap years or month-specific day counts.
- CSV values cannot contain commas.
- The app is a local prototype, not a multi-user service; simultaneous access and authentication are not implemented.

## Possible next steps

- Add room-capacity recommendations and recurring bookings
- Replace CSV with SQLite
- Add a graphical or web interface
- Add richer date/time validation
