from src.Attendance.Value_object.Person import Person
from src.Attendance.Value_object.Room import Room


class AttendanceRecord:
    def __init__(self, _id: int = None, room: Room = None, person: Person = None, date_time_stamp: str = None):
        self._id: int = _id
        self.room: Room = room
        self.person: Person = person
        self.date_time_stamp: str = date_time_stamp

    def __str__(self):
        return f"(_id:{self._id}, Room:{self.room}, Person:{self.person}, date_time_stamp:{self.date_time_stamp})"

    def get_id(self):
        return self._id

    def get_room(self):
        return self.room

    def get_person(self):
        return self.person
