from src.Server_manager.Database_connect.PymysqlUtil import PymysqlUtil, DATETIME_FORMAT
from src.Attendance.Value_object.Person import Person


class PersonDAO:
    def get_person_by_id(self, person_id):
        db = PymysqlUtil.get_connect()
        cursor = db['cursor']

        sql = "select * from person where _id = %s"

        cursor.execute(sql, (person_id))

        result = cursor.fetchone()
        columns_name_map = PymysqlUtil.columns_name_of_index_map(cursor)
        _id = result[columns_name_map['_id']]
        name = result[columns_name_map['name']]

        # build value object
        person = Person(_id, name)

        return person