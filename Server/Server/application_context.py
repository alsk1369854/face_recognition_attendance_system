beans = {
    # Face recognition ============================================================================
    "face_recognizer": {
        "class": "src.Face_recognition.FaceRecognizer"
    },

    # Server => Controller ========================================================================
    # Dispatcher 中的 servlet_path = build, 則由 AttendanceController 來處理
    # 在 BeanFactory 中 bean_map 對於 bean_class 對應的 bean_id
    "attendance": {
        # 對應的 class 位置, 用於 bean 的建立(在此僅提示作用, 實際建立在 BeanFactory 執行)
        "class": "src.Attendance.Controller.AttendanceController",
        # "依賴屬性名稱" : "引用的 bean id", 用於依賴注入
        "property_beans": {
            "attendance_record_DAO": "attendance_record_DAO",
            "room_DAO": "room_DAO",
        }
    },

    "face_recognition": {
        "class": "src.Attendance.Controller.FaceRecognitionController",
        "property_beans": {
            "face_recognizer": "face_recognizer",
            "attendance_record_DAO": "attendance_record_DAO",
            "room_DAO": "room_DAO",
            "person_DAO": "person_DAO"
        }
    },

    # Server => Database access object ===========================================================
    "attendance_record_DAO": {
        "class": "src.Attendance.Database_access_object.AttendanceRecordDAO",
        "property_beans": {
            "person_DAO": "person_DAO",
            "room_DAO": "room_DAO"
        }
    },

    "room_DAO": {
        "class": "src.Attendance.Database_access_object.RoomDAO"
    },

    "person_DAO": {
        "class": "src.Attendance.Database_access_object.PersonDAO"
    },

}

# for bean in beans:
#     print(bean)
#     bean_info = beans[bean]
#     if 'property_beans' in bean_info:
#         bean_property = bean_info['property_beans']
#         for property in bean_property:
#             print("\t" + property)
