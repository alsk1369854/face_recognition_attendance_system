import numpy as np
import face_recognition as fr
import cv2
import os

image_folder_path = r'known_face_image'
known_face_data_path = 'known_face_data.npy'
num_jitters = 50
landmarks_model = 'large'  # 'large' | 'small'


def encoding_image_list(image_list, person_id_list):
    encoded_image_list = []
    total_case = len(person_id_list)

    for i, (image, person_id) in enumerate(zip(image_list, person_id_list)):
        img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        print(str(int(i / total_case * 100)) + '%')
        encode = fr.face_encodings(img, num_jitters=num_jitters, model=landmarks_model)[0]
        encoded_image_list.append(encode)
    print('100%')
    return encoded_image_list


def save_known_faces(person_id_list, encoded_image_list):
    with open(known_face_data_path, 'wb') as file:
        np.save(file, person_id_list)
        np.save(file, encoded_image_list)


if __name__ == '__main__':
    image_list = []
    person_id_list = []
    known_face_image_files = os.listdir(image_folder_path)
    print(len(known_face_image_files), known_face_image_files)

    for image_file_name in known_face_image_files:
        curImg = cv2.imread(f'{image_folder_path}/{image_file_name}')
        image_list.append(curImg)
        person_id_list.append(os.path.splitext(image_file_name)[0])

    print('Encoding...\n')
    encoded_image_list = encoding_image_list(image_list, person_id_list)
    save_known_faces(person_id_list, encoded_image_list)
    print('\nDone')
