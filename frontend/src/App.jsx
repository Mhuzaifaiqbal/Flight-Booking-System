
import "./App.css";
import FlightForm from "./components/FlightForm";
import CurrentBookings from "./components/currentbookings";

function App() {
  return (
    <div className="app">
      <h1 className="main-heading"> Airline Seat Booking System</h1>
      <div>
        <FlightForm />
        <CurrentBookings />
      </div>
    </div>
  );
}

export default App;
