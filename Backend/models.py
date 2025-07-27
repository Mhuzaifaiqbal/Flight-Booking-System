from enum import Enum
from typing import List
from datetime import datetime
import csv

ROWS = 5
COLUMNS = 5

class SeatCategory(str, Enum):
    FIRST = "First"
    BUSINESS = "Business"
    ECONOMY = "Economy"

class Seat:
    def __init__(self, row, col, category: SeatCategory):
        self.row = row
        self.col = col
        self.category = category
        self.is_booked = False
        self.passenger_name = ""
        self.phone_number = ""
        self.time = ""

    def to_dict(self):
     return {
        "row": self.row,
        "col": self.col,
        "category": self.category,
        "isBooked": self.is_booked,
        "passengerName": self.passenger_name,
        "phoneNumber": self.phone_number,
        "time": self.time
    }

class Flight:
    def __init__(self, flight_number: str, date: str):
        self.flight_number = flight_number
        self.date = date
        self.booked_numbers = set()
        self.seats: List[List[Seat]] = []
        for i in range(ROWS):
            category = SeatCategory.FIRST if i == 0 else SeatCategory.BUSINESS if i == 1 else SeatCategory.ECONOMY
            row = [Seat(i + 1, j + 1, category) for j in range(COLUMNS)]
            self.seats.append(row)

    def current_timestamp(self):
        return datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    def save_to_csv(self):
        try:
            with open("backend/bookings.csv", "w", newline='') as file:
                writer = csv.writer(file)
                writer.writerow(["Row", "Col", "Name", "Phone", "Class", "Booked", "Timestamp"])
                for row in self.seats:
                    for seat in row:
                        if seat.is_booked:
                            writer.writerow([
                                seat.row, seat.col, seat.passenger_name,
                                seat.phone_number, seat.category, "Yes", seat.time
                            ])
        except Exception as e:
            print("CSV saving failed:", e)

    def get_seating_chart(self):
        return [[seat.to_dict() for seat in row] for row in self.seats]

    def book_seat(self, row, col, name, phone):
        try:
            row = int(row)
            col = int(col)
        except ValueError:
            return False, "Row and column must be integers"

        if not (1 <= row <= ROWS and 1 <= col <= COLUMNS):
            return False, "Invalid seat position"

        if phone in self.booked_numbers:
            return False, "Phone number already used for booking"

        seat = self.seats[row - 1][col - 1]
        if seat.is_booked:
            return False, "Seat already booked"

        seat.is_booked = True
        seat.passenger_name = name
        seat.phone_number = phone
        seat.time = self.current_timestamp()
        self.booked_numbers.add(phone)
        self.save_to_csv()
        return True, f"Seat booked in {seat.category} class"

    def cancel_seat(self, row, col, name, phone):
        if not (1 <= row <= ROWS and 1 <= col <= COLUMNS):
            return False, "Invalid seat position"

        seat = self.seats[row - 1][col - 1]
        if not seat.is_booked or seat.passenger_name != name or seat.phone_number != phone:
            return False, "No matching booking found"

        seat.is_booked = False
        seat.passenger_name = ""
        seat.phone_number = ""
        seat.time = ""
        self.booked_numbers.discard(phone)
        self.save_to_csv()
        return True, "Booking cancelled"

    def available_seats_by_category(self):
        counts = {cat: 0 for cat in SeatCategory}
        for row in self.seats:
            for seat in row:
                if not seat.is_booked:
                    counts[seat.category] += 1
        return counts

    def view_bookings(self):
     bookings = []
     for row in self.seats:
        for seat in row:
            if seat.is_booked:
                booking = seat.to_dict()
                booking["flight_no"] = self.flight_number
                booking["seat_no"] = f"{seat.row}-{seat.col}"
                bookings.append(booking)
     return bookings
