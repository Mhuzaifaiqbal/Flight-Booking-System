from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from models import Flight

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"], 
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

flight = Flight("PK-707", "2025-08-01")

class BookingRequest(BaseModel):
    row: int
    col: int
    name: str
    phone: str


@app.get("/seating")
def get_seating():
    return flight.get_seating_chart()

@app.get("/available")
def get_available_by_category():
    return flight.available_seats_by_category()

@app.get("/bookings")
def get_bookings():
    return flight.view_bookings()

@app.post("/book")
def book_seat(req: BookingRequest):
    try:
        print("Incoming booking request:", req)
        success, msg = flight.book_seat(req.row, req.col, req.name, req.phone)
        if not success:
            raise HTTPException(status_code=400, detail=msg)
        return {"message": msg}
    except Exception as e:
        print("Error during booking:", str(e))
        raise HTTPException(status_code=500, detail="An error occurred while booking the seat")

@app.post("/cancel")
def cancel_seat(req: BookingRequest):
    try:
        print("Incoming cancel request:", req)
        success, msg = flight.cancel_seat(req.row, req.col, req.name, req.phone)
        if not success:
            raise HTTPException(status_code=400, detail=msg)
        return {"message": msg}
    except Exception as e:
        print("Error during cancellation:", str(e))
        raise HTTPException(status_code=500, detail="An error occurred while canceling the seat")
