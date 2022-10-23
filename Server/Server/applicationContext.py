beans = {
    # Dispatcher 中的 servlet_path = attendance, 則由 AttendanceController 來處理
    "attendance": {
        # 對應的 class 位置, 用於 bean 的建立(在此僅提示作用, 實際建立在 BeanFactory 執行)
        "class": "src.Attendance.Controller.AttendanceController",
        # "依賴屬性名稱" : "引用的 bean id", 用於依賴注入
        "property_beans": {
            "attendance_record_DAO": "attendance_record_DAO",
        }
    },

    "attendance_record_DAO": {
        "class": "src.Attendance.Database_access_object.AttendanceRecordDAO",
        "property_beans": {
            "person_DAO": "person_DAO",
            "room_DAO": "room_DAO"
        }
    },

    "person_DAO": {
        "class": "src.Attendance.Database_access_object.PersonDAO"
    },

    "room_DAO": {
        "class": "src.Attendance.Database_access_object.RoomDAO"
    }
}


# for bean in beans:
#     print(bean)
#     bean_info = beans[bean]
#     if 'property_beans' in bean_info:
#         bean_property = bean_info['property_beans']
#         for property in bean_property:
#             print("\t" + property)
