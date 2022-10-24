from src.Server_manager.Database_connect.PymysqlUtil import PymysqlUtil, DATETIME_FORMAT
from src.Attendance.Value_object.AttendanceRecord import AttendanceRecord


class AttendanceRecordDAO:
    person_DAO = None
    room_DAO = None

    def insert_new_attendance_record(self, attendance_record: AttendanceRecord):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "insert into attendance_record (room, person, date_time_stamp) \n" \
              "values (%s, %s, %s)"

        return cursor.execute(sql,
                       (attendance_record.room._id, attendance_record.person._id, attendance_record.date_time_stamp))

    def get_room_records_between_two_datetime_list(self, room_id, start_datetime, end_datetime, order_by="_id"):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * " \
              "from attendance_record " \
              "where date_time_stamp in (	select min(date_time_stamp) as date_time_stamp " \
              "                             from testDB.attendance_record " \
              "                             where room = %s " \
              "                             and date_time_stamp between %s and %s " \
              "                             group by person) " \
              "order by %s ASC"

        cursor.execute(sql, (room_id, start_datetime, end_datetime, order_by))

        result_list = cursor.fetchall()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        attendance_record_list = []
        for result in result_list:
            _id = result[columns_name_map['_id']]
            room_id = result[columns_name_map['room']]  # room id
            room = self.room_DAO.get_room_by_id(room_id)  # build room
            person_id = result[columns_name_map['person']]  # person id
            person = self.person_DAO.get_person_by_id(person_id)  # build person
            date_time_stamp = result[columns_name_map['date_time_stamp']]
            date_time_stamp = date_time_stamp.strftime(DATETIME_FORMAT)

            # build value object
            attendance_record = AttendanceRecord(_id, room, person, date_time_stamp)
            attendance_record_list.append(attendance_record)

        return attendance_record_list

    def get_attendance_record_by_id(self, item_id):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * from attendance_record where _id = %s"

        cursor.execute(sql, (item_id))

        result = cursor.fetchone()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        _id = result[columns_name_map['_id']]
        room_id = result[columns_name_map['room']]  # room id
        room = self.room_DAO.get_room_by_id(room_id)  # build room
        person_id = result[columns_name_map['person']]  # person id
        person = self.person_DAO.get_person_by_id(person_id)  # build person
        date_time_stamp = result[columns_name_map['date_time_stamp']]  # type of : datetime
        date_time_stamp = date_time_stamp.strftime(DATETIME_FORMAT)  # format to str

        # build value object
        attendance_record = AttendanceRecord(_id, room, person, date_time_stamp)

        return attendance_record

