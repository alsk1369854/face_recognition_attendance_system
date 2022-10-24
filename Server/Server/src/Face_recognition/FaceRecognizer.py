import numpy as np
import face_recognition as fr
import cv2

known_face_data_path = r'./src/Face_recognition/Train_face/known_face_data.npy'
# gpu: 'cnn' ; other: 'hog'
detection_model_type = 'hog'  # 'cnn' | 'hog'
landmarks_model = 'large'  # 'large' | 'small'

class FaceRecognizer:
    def __init__(self):
        # load_known_faces
        print('Loading known face data ...')
        with open(known_face_data_path, 'rb') as f:
            self.person_id_list = np.load(f)
            self.known_encoded_face_list = np.load(f)

    def detection_binary_image(self, binary_image_data):
        # result data
        # unknown face value item: {x1, y1, x2, y2}
        # known face value item: {x1, y1, x2, y2, name}
        result = {
            'unknown_face_value_list': [],
            'known_face_value_list': []
        }

        # build image
        np_image_data = np.frombuffer(binary_image_data, dtype=np.uint8)
        # decode image RGB
        image = cv2.imdecode(np_image_data, cv2.IMREAD_COLOR)
        image = cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)

        # 擷取畫面中的人臉特徵
        # gpu face_locations
        # face_location_list = face_recognition.face_locations(image, model='cnn')
        # cpu face_locations
        face_location_list = fr.face_locations(image, model=detection_model_type)

        # 建立影像畫面中的人臉數據
        encoded_face_list = fr.face_encodings(image, face_location_list, model=landmarks_model)

        # 遍歷畫面中所找找到的臉部特徵
        for face_location, encoded_face in zip(face_location_list, encoded_face_list):
            y1, x2, y2, x1 = face_location
            matches = fr.compare_faces(self.known_encoded_face_list, encoded_face, tolerance=0.4)
            face_distance = fr.face_distance(self.known_encoded_face_list, encoded_face)
            print(face_distance)
            matchIndex = np.argmin(face_distance)
            # support = face_distance[matchIndex]

            if matches[matchIndex]:
                # known face
                person_id = self.person_id_list[matchIndex]
                face_value = {
                    'x1': x1,
                    'y1': y1,
                    'x2': x2,
                    'y2': y2,
                    'person_id': person_id
                }
                result['known_face_value_list'].append(face_value)

            else:
                # unknown face
                face_value = {
                    'x1': x1,
                    'y1': y1,
                    'x2': x2,
                    'y2': y2,
                }
                result['unknown_face_value_list'].append(face_value)

        return result



