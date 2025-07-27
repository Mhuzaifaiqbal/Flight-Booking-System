import axios from "axios";

const API_BASE = "http://localhost:8000";  // FastAPI backend base URL

// Get the current seating chart
export const getSeatingChart = async () => {
  const response = await axios.get(`${API_BASE}/seating`);
  return response.data;
};

// Get the available seats categorized
export const getAvailableSeats = async () => {
  const response = await axios.get(`${API_BASE}/available`);
  return response.data;
};

// Get all current bookings
export const getBookings = async () => {
  const response = await axios.get(`${API_BASE}/bookings`);
  return response.data;
};

// Book a seat
export const bookSeat = async (booking) => {
  const response = await axios.post(`${API_BASE}/book`, booking);
  return response.data;
};

// Cancel a booking
export const cancelSeat = async (booking) => {
  const response = await axios.post(`${API_BASE}/cancel`, booking);
  return response.data;
};
