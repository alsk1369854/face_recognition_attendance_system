from flask import Response
import json
from datetime import datetime
from src.Server_manager.Util.JsonUtil import JsonUtil
from src.Server_manager.Database_connect.PymysqlUtil import DATETIME_FORMAT
from src.Attendance.Value_object.AttendanceRecord import AttendanceRecord



class FaceRecognitionController:
    face_recognizer = None
    attendance_record_DAO = None
    room_DAO = None
    person_DAO = None

    def index(self, request, session):
        webcam_device_id = session['webcam_device_id']
        return self.detection(request, webcam_device_id)

    def detection(self, request, webcam_device_id: str):
        binary_image_data = request.data
        room = self.room_DAO.get_room_by_webcam_device_id(webcam_device_id=webcam_device_id)
        detection_result = self.face_recognizer.detection_binary_image(binary_image_data)
        # 遍歷偵測到的已知人臉數據
        for known_face_value in detection_result['known_face_value_list']:
            # 插入 peron_name 到回傳資料中
            person_id = known_face_value['person_id']
            person = self.person_DAO.get_person_by_id(person_id=person_id)
            known_face_value['person_name'] = person.name
            # 新增時間記錄至DB
            now_datatime = datetime.now().strftime(DATETIME_FORMAT)
            attendance_record = AttendanceRecord(room=room,
                                                 person=person,
                                                 date_time_stamp=now_datatime)

            self.attendance_record_DAO.insert_new_attendance_record(attendance_record=attendance_record)
            # self.attendance_record_DAO.insert_new_attendance_record(room_id=attendance_record.room._id,
            #                                                         person_id=attendance_record.person._id,
            #                                                         date_time_stamp=attendance_record.date_time_stamp)

        response = json.dumps(detection_result, default=JsonUtil.obj_dict)
        return "", Response(response=response, status=200, mimetype="application/json")
