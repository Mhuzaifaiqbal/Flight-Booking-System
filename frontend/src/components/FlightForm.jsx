
import React, { useState } from "react";
import { bookSeat, cancelSeat } from "../api";
import "./FlightForm.css"; 

const FlightForm = () => {
  const [row, setRow] = useState("");
  const [col, setCol] = useState("");
  const [name, setName] = useState("");
  const [phone, setPhone] = useState("");
  const [action, setAction] = useState("book");
  const [message, setMessage] = useState("");

  const handleSubmit = async (e) => {
    e.preventDefault();
    setMessage("");

    const payload = {
      row: parseInt(row),
      col: parseInt(col),
      name,
      phone,
    };

    try {
      const response =
        action === "book" ? await bookSeat(payload) : await cancelSeat(payload);
      setMessage(response.message);
    } catch (error) {
      setMessage(error?.response?.data?.detail || "An error occurred");
    }
  };

  return (
    <div className="flight-form-container">
      <h2 className="form-heading">
        {action === "book" ? " Book a Seat" : " Cancel a Booking"}
      </h2>
      <form onSubmit={handleSubmit} className="form">
        <div className="form-group">
          <label>Row</label>
          <input
            type="number"
            value={row}
            onChange={(e) => setRow(e.target.value)}
            required
          />
        </div>

        <div className="form-group">
          <label>Column</label>
          <input
            type="number"
            value={col}
            onChange={(e) => setCol(e.target.value)}
            required
          />
        </div>

        <div className="form-group">
          <label>Name</label>
          <input
            type="text"
            value={name}
            onChange={(e) => setName(e.target.value)}
            required
          />
        </div>

        <div className="form-group">
          <label>Phone</label>
          <input
            type="text"
            value={phone}
            onChange={(e) => setPhone(e.target.value)}
            required
          />
        </div>

        <div className="form-group">
          <label>Action</label>
          <select
            value={action}
            onChange={(e) => setAction(e.target.value)}
          >
            <option value="book">Book Seat</option>
            <option value="cancel">Cancel Seat</option>
          </select>
        </div>

        <button type="submit" className="submit-button">
          {action === "book" ? "Book Seat" : "Cancel Seat"}
        </button>

        {message && <div className="form-message">{message}</div>}
      </form>
    </div>
  );
};

export default FlightForm;
