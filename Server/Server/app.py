from flask import Flask, render_template, request, Response, session
import json
# import uuid
from src.Server_manager.MVC.DispatcherServlet import DispatcherServlet
from src.Face_recongnition.FaceRecognizer import FaceRecognizer

dispatcher_servlet = DispatcherServlet()
face_recognizer = FaceRecognizer()

LISTEN_POST = 8888

app = Flask(__name__)


# 設計 API 接口
@app.route('/<servlet_path>', methods=['POST', 'GET'])  # web router
def index(servlet_path):
    print('servlet_path')

    # if request.method == "POST":
    #     user = request.form['name']
    #     session['user'] = user

    operate, value = dispatcher_servlet.service(request, session, servlet_path)
    # response Web Page
    return value


@app.route('/imageUpload/<device_id>', methods=['POST'])
def imageUpload(device_id):
    print(device_id)
    # # save image
    # with open('test.jpeg', 'wb') as f:
    #     f.write(request.data)
    #
    # # build response json
    # result = [{'x1': 0, 'y1': 10, 'x2': 10, 'y2': 20, 'name': 'ming'}]

    result = face_recognizer.detection_binary_image(request.data)
    response = json.dumps(result)

    return Response(response=response, status=200, mimetype="application/json")


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=LISTEN_POST)
