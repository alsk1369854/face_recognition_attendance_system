from src.Attendance.Value_object.Room import Room
from src.Attendance.Value_object.Person import Person


class AttendanceRecord:
    def __init__(self, _id: int = None, room: Room = None, person: Person = None, date_time_stamp: str = None):
        self._id: int = _id
        self.room: Room = room
        self.person: Person = person
        self.date_time_stamp: str = date_time_stamp

    def __str__(self):
        return f"(_id:{self._id}, Room:{self.room}, Person:{self.person}, date_time_stamp:{self.date_time_stamp})"
