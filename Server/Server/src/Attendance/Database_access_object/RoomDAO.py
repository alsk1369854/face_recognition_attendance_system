from src.Server_manager.Database_connect.PymysqlUtil import PymysqlUtil, DATETIME_FORMAT
from src.Attendance.Value_object.Room import Room


class RoomDAO:
    def get_room_by_id(self, room_id):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * from room where _id = %s"

        cursor.execute(sql, (room_id))

        result = cursor.fetchone()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        _id = result[columns_name_map['_id']]
        name = result[columns_name_map['name']]
        webcam_device_id = result[columns_name_map['webcam_device_id']]

        # build value object
        room = Room(_id, name, webcam_device_id)

        return room

    def get_room_by_name(self, room_name):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * from room where name = %s"

        cursor.execute(sql, (room_name))

        result = cursor.fetchone()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        _id = result[columns_name_map['_id']]
        name = result[columns_name_map['name']]
        webcam_device_id = result[columns_name_map['webcam_device_id']]

        # build value object
        room = Room(_id, name, webcam_device_id)

        return room

    def get_room_by_webcam_device_id(self, webcam_device_id):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * from room where webcam_device_id like %s"

        cursor.execute(sql, (webcam_device_id))

        result = cursor.fetchone()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        _id = result[columns_name_map['_id']]
        name = result[columns_name_map['name']]
        webcam_device_id = result[columns_name_map['webcam_device_id']]

        # build value object
        room = Room(_id, name, webcam_device_id)

        return room
