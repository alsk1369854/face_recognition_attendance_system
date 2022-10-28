from flask import Response
import json
from src.Server_manager.Util.JsonUtil import JsonUtil


class AttendanceController:
    room_DAO = None
    attendance_record_DAO = None

    def index(self):
        value = {
            'template': 'index.html',
            'data': {}
        }
        return "render_template", value

    def search(self, request, room_name: str, start_datetime: str, end_datetime: str):
        order_by = request.args.get('order_by', default='person')
        room = self.room_DAO.get_room_by_name(room_name)
        attendance_record_list = \
            self.attendance_record_DAO.get_room_records_between_two_datetime_list(room_id=room._id,
                                                                                  start_datetime=start_datetime,
                                                                                  end_datetime=end_datetime,
                                                                                  order_by=order_by)
        response = json.dumps(attendance_record_list, default=JsonUtil.obj_dict)
        return "", Response(response=response, status=200, mimetype="application/json")

    def get_all_room(self):
        room = self.room_DAO.get_all()
        response = json.dumps(room, default=JsonUtil.obj_dict)
        return "", Response(response=response, status=200, mimetype="application/json")