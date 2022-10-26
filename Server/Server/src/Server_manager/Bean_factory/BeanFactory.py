from application_context import beans

# Beans class
# Face recognition class =================================================================
from src.Face_recognition.FaceRecognizer import FaceRecognizer
# Server => Controller class =============================================================
from src.Attendance.Controllers.AttendanceController import AttendanceController
from src.Attendance.Controllers.FaceRecognitionController import FaceRecognitionController
# Server => Database access object =======================================================
from src.Attendance.Database_access_object.AttendanceRecordDAO import AttendanceRecordDAO
from src.Attendance.Database_access_object.PersonDAO import PersonDAO
from src.Attendance.Database_access_object.RoomDAO import RoomDAO


class BeanFactory:
    def __init__(self):
        # 建立 bean 物件
        self.bean_map = {}
        self.bean_map['face_recognizer'] = FaceRecognizer()
        self.bean_map['attendance'] = AttendanceController()
        self.bean_map['face_recognition'] = FaceRecognitionController()
        self.bean_map['attendance_record_DAO'] = AttendanceRecordDAO()
        self.bean_map['room_DAO'] = RoomDAO()
        self.bean_map['person_DAO'] = PersonDAO()

        # 注入物件 依賴關係
        for bean_id in beans:
            bean_info = beans[bean_id]
            if 'property_beans' in bean_info:
                destination_bean = self.bean_map[bean_id]
                bean_property = bean_info['property_beans']
                for property_parameter_name in bean_property:
                    property_bean_id = bean_property[property_parameter_name]
                    property_bean = self.bean_map[property_bean_id]
                    try:
                        if hasattr(destination_bean, property_bean_id):
                            setattr(destination_bean, property_bean_id, property_bean)
                        else:
                            # error dest_class has no property_name
                            raise Exception(f'Bean "{bean_id}" class has no parameter "{property_bean_id}"')
                    except Exception as error:
                        print('BeanFactory error: ' + repr(error))

    def get_bean(self, _id: str):
        if _id in self.bean_map:
            return self.bean_map[_id]
