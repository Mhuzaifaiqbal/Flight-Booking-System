#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <set>
using namespace std;

const int ROWS = 5;
const int COLUMNS = 5;

enum SeatCategory { FIRST, BUSINESS, ECONOMY };

class Seat {
public:
    int row, col;
    SeatCategory category;
    bool isBooked;
    string passengerName;
    string phoneNumber;
    string time;

    Seat(int row, int col, SeatCategory category)
        : row(row), col(col), category(category), isBooked(false),
          passengerName(""), phoneNumber(""), time("") {}

    string getCategoryString() const {
        switch (category) {
            case FIRST: return "First";
            case BUSINESS: return "Business";
            case ECONOMY: return "Economy";
        }
        return "";
    }

    void display() const {
        cout << (isBooked ? "[X]" : "[O]");
    }
};

class Flight {
private:
    vector<vector<Seat>> seats;
    string flightNumber;
    string date;
    set<string> booked_numbers;

    string currentTimestamp() const {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buf);
    }

    void saveToCSV() const {
        ofstream file("bookings.csv");
        file << "Row,Col,Name,Phone,Class,Booked,Timestamp\n";
        for (const auto &row : seats) {
            for (const auto &seat : row) {
                if (seat.isBooked) {
                    file << seat.row << "," << seat.col << "," << seat.passengerName << ","
                         << seat.phoneNumber << "," << seat.getCategoryString() << ",Yes," << seat.time << "\n";
                }
            }
        }
        file.close();
    }

public:
    Flight(string number, string flightDate) : flightNumber(number), date(flightDate) {
        for (int i = 0; i < ROWS; ++i) {
            vector<Seat> row;
            SeatCategory cat = (i == 0) ? FIRST : (i == 1) ? BUSINESS : ECONOMY;
            for (int j = 0; j < COLUMNS; ++j)
                row.emplace_back(i + 1, j + 1, cat);
            seats.push_back(row);
        }
    }

    void displaySeatingChart() {
        cout << "\nSeating Chart (B = Booked, A = Available)\n";
        cout << "   ";
        for (int col = 1; col <= 5; ++col)
            cout << "  " << col << " ";
        cout << "\n";

        for (int row = 0; row < 5; ++row) {
            cout << row + 1 << " ";
            for (int col = 0; col < 5; ++col) {
                const Seat &seat = seats[row][col];
                cout << (seat.isBooked ? "\033[31m[B]\033[0m " : "\033[32m[A]\033[0m ");
            }

            if (row == 0)
                cout << "<- First Class";
            else if (row == 1 || row == 2)
                cout << "<- Business Class";
            else
                cout << "<- Economy Class";

            cout << endl;
        }
    }

    bool bookSeat(int row, int col, string name, string phone) {
        if (row < 1 || row > 5 || col < 1 || col > 5) {
            cout << "Invalid Seat Number\n";
            return false;
        }

        if (booked_numbers.count(phone)) {
            cout << "Phone number already used for booking.\n";
            return false;
        }

        Seat &seat = seats[row - 1][col - 1];
        if (seat.isBooked) {
            cout << "Seat already Booked\n";
            return false;
        }

        seat.isBooked = true;
        seat.passengerName = name;
        seat.phoneNumber = phone;
        seat.time = currentTimestamp();
        booked_numbers.insert(phone);

        saveToCSV();

        cout << "\nSeat booked successfully in " << seat.getCategoryString() << " class.\n";
        return true;
    }

    bool cancelSeat(int row, int col, string name, string phone) {
        if (row < 1 || row > 5 || col < 1 || col > 5) {
            cout << "Invalid Seat Number\n";
            return false;
        }

        Seat &seat = seats[row - 1][col - 1];
        if (!seat.isBooked || seat.passengerName != name || seat.phoneNumber != phone) {
            cout << "No matching booking found.\n";
            return false;
        }

        seat.passengerName = "";
        seat.phoneNumber = "";
        seat.time = "";
        seat.isBooked = false;
        booked_numbers.erase(phone);

        saveToCSV();

        cout << "Booking cancelled.\n";
        return true;
    }

    void displayAvailableSeatsByCategory() const {
        int first = 0, business = 0, economy = 0;
        for (const auto &row : seats) {
            for (const auto &seat : row) {
                if (!seat.isBooked) {
                    switch (seat.category) {
                        case FIRST: first++; break;
                        case BUSINESS: business++; break;
                        case ECONOMY: economy++; break;
                    }
                }
            }
        }

        cout << "\nAvailable Seats by Category:\n";
        cout << "First Class: " << first << endl;
        cout << "Business Class: " << business << endl;
        cout << "Economy Class: " << economy << endl;
    }

    void viewBookings() const {
        cout << "\n--- Bookings ---\n";
        for (const auto &row : seats) {
            for (const auto &seat : row) {
                if (seat.isBooked) {
                    cout << "Seat: R" << seat.row << "C" << seat.col
                         << ", Name: " << seat.passengerName
                         << ", Phone: " << seat.phoneNumber
                         << ", Category: " << seat.getCategoryString()
                         << ", Time: " << seat.time << endl;
                }
            }
        }
    }
};




int main() {
    Flight flight("PK-707", "2025-08-01");
    int choice;
    do {
        cout << "\nAirline Seat Booking System\n";
        cout << "1. Display Seating Chart\n";
        cout << "2. Display Available Seats by Category\n";
        cout << "3. View Bookings\n";
        cout << "4. Book a Seat\n";
        cout << "5. Cancel a Seat\n";
        cout << "6. Exit the Program\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                flight.displaySeatingChart(); break;
            case 2:
                flight.displayAvailableSeatsByCategory(); break;
            case 3:
                flight.viewBookings(); break;
            case 4: {
                int row, col; string name, phone;
                cout << "Enter row (1-5): "; cin >> row;
                cout << "Enter column (1-5): "; cin >> col;
                cout << "Enter name: "; cin.ignore(); getline(cin, name);
                cout << "Enter phone: "; getline(cin, phone);
                if (!flight.bookSeat(row, col, name, phone))
                    cout << "Booking failed.\n";
                break;
            }
            case 5: {
                int row, col; string name, phone;
                cout << "Enter row (1-5): "; cin >> row;
                cout << "Enter column (1-5): "; cin >> col;
                cout << "Enter name: "; cin.ignore(); getline(cin, name);
                cout << "Enter phone: "; getline(cin, phone);
                if (!flight.cancelSeat(row, col, name, phone))
                    cout << "Cancellation failed.\n";
                break;
            }
            case 6:
                cout << "Exiting the program. Thank you!\n"; break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}

