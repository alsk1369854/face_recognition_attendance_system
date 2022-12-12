# Face Recongnition Attendance System

## 系統介紹
```
本項目包含三端
1. Webcam 端: 負責拍攝臉部影像並傳送至 Server 辨識簽到。(詳細原始碼請看目錄 /Webcam/Webcam)

2. Server 端: 負責辨識 Webcam 所傳送的影像，並將辨識結果回傳以及存儲至 DB，也同時負責響應 Client 端所發出的出勤記錄查詢。(詳細原始碼請看目錄 /Server/Server)

3. Client 端: 負責提供給使用者考勤記錄的查詢與下載功能。(詳細原始碼請看目錄 /Server/Server/front_end)

其他:
    DataBase 相關請看目錄: /Database
```
<br/>

## 項目畫面
### WebCam 
<p align="center">
<img height='550' src='https://raw.githubusercontent.com/alsk1369854/face_recognition_attendance_system/master/screenshots/Webcam.gif'/>
</p>
<br/>

### Client
<p align="center">
<img width='700' src='https://raw.githubusercontent.com/alsk1369854/face_recognition_attendance_system/master/screenshots/Client.gif'/>
</p>
<br/>

<br/>

#### END