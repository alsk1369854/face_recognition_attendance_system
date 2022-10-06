import numpy as np
import face_recognition as fr
import cv2

known_faces_path = r'known_faces.npy'
# gpu: 'cnn' ; other: 'hog'
detection_model_type = 'hog'  # 'cnn' | 'hog'
landmarks_model = 'large'  # 'large' | 'small'
# frame size
frame_size = None # None | (width, height)

# 開啟影片檔案
cap = cv2.VideoCapture(0)


def load_known_faces():
    with open(known_faces_path, 'rb') as f:
        person_names = np.load(f)
        encoded_images = np.load(f)
    return (person_names, encoded_images)


if __name__ == '__main__':
    # 讀取已知的人臉
    (person_names, known_faces) = load_known_faces()
    print(person_names)

    # Start detection...
    while True:
        # 獲取鏡頭畫面
        ret, frame = cap.read()
        # 水平反轉影像
        frame = cv2.flip(frame, 1)

        # 設置畫面大小
        if frame_size:
            frame = cv2.resize(frame, frame_size)

        # 準備辨識人臉影像
        image = cv2.resize(frame, (0, 0), None, 0.25, 0.25)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # 擷取畫面中的人臉特徵
        # gpu face_locations
        # face_location_list = face_recognition.face_locations(image, model='cnn')
        # cpu face_locations
        face_location_list = fr.face_locations(image, model=detection_model_type)
        encoded_face_list = fr.face_encodings(image, face_location_list, model=landmarks_model)

        # 遍歷畫面中所找找到的臉部特徵
        for face_location, encoded_face in zip(face_location_list, encoded_face_list):
            y1, x2, y2, x1 = face_location
            y1, x2, y2, x1 = y1 * 4, x2 * 4, y2 * 4, x1 * 4
            matches = fr.compare_faces(known_faces, encoded_face, tolerance=0.4)
            face_distance = fr.face_distance(known_faces, encoded_face)
            print(face_distance)
            matchIndex = np.argmin(face_distance)
            suppert = face_distance[matchIndex]

            if matches[matchIndex]:
                person_name = person_names[matchIndex]

                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                # 標示分數
                cv2.putText(frame, str(suppert), (x1, y1),
                            cv2.FONT_HERSHEY_DUPLEX, 0.7, (255, 255, 255), 1, cv2.LINE_AA)
                cv2.putText(frame, person_name, (x1 + 6, y2 - 6),
                            cv2.FONT_HERSHEY_COMPLEX, 1, (255, 255, 255), 2)

            else:
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 255), 2)

        # Display the resulting frame
        cv2.imshow('Face Detection', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()
