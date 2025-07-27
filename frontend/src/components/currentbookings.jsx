
import React, { useEffect, useState } from "react";
import { getBookings, cancelSeat } from "../api";
import "./currentbookings.css";

const CurrentBookings = () => {
  const [bookings, setBookings] = useState([]);

  const fetchBookings = async () => {
    try {
      const data = await getBookings();
      setBookings(data);
    } catch (error) {
      console.error("Error fetching bookings:", error);
    }
  };

  useEffect(() => {
    fetchBookings();
  }, []);

  const handleCancel = async (booking) => {
    try {
      await cancelSeat({
        row: booking.row,
        col: booking.col,
        name: booking.passengerName,
        phone: booking.phoneNumber,
      });
      fetchBookings();
    } catch (error) {
      console.error("Cancel failed:", error);
      alert("Failed to cancel booking.");
    }
  };

  return (
    <div className="bookings-container">
      <h2 className="bookings-heading">Current Bookings</h2>
      {bookings.length === 0 ? (
        <p className="no-bookings-text">No bookings found.</p>
      ) : (
        <table className="bookings-table">
          <thead>
            <tr>
              <th>Flight No</th>
              <th>Passenger Name</th>
              <th>Seat No</th>
              <th>Phone</th>
              <th>Action</th>
            </tr>
          </thead>
          <tbody>
            {bookings.map((booking, index) => (
              <tr key={index}>
                <td>{booking.flight_no}</td>
                <td>{booking.passengerName}</td>
                <td>{booking.seat_no}</td>
                <td>{booking.phoneNumber}</td>
                <td>
                  <button
                    className="cancel-button"
                    onClick={() => handleCancel(booking)}
                  >
                    Cancel
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      )}
    </div>
  );
};

export default CurrentBookings;
