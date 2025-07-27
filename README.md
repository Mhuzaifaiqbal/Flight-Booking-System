**Project Highlights:
Frontend (React):**
I built an interactive interface using React, styled it with custom CSS, and designed two key components:
FlightForm.jsx: for adding new flights.
CurrentBookings.jsx: for displaying and managing real-time seat bookings.
These were integrated into App.jsx to build a responsive and user-friendly booking system. The interface includes features like:
Booking a seat
Canceling a booking
Error handling for already-booked or invalid seat operations
A direct Cancel button beside each booking for one-click removal

**Backend (FastAPI):**
Initially written in C++, the backend logic was later converted into Python. I then built a FastAPI application to serve endpoints for:
Creating flights
Booking seats
Canceling bookings
Fetching real-time flight data
The API ensures proper validation of requests and maintains data persistence through CSV for simplicity
