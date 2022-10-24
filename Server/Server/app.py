from flask import Flask, render_template, redirect, url_for, request, Response, session
import json
# import uuid
from src.Server_manager.MVC.DispatcherServlet import DispatcherServlet
from src.Server_manager.Database_connect.PymysqlUtil import PymysqlUtil
from src.Face_recognition.FaceRecognizer import FaceRecognizer

dispatcher_servlet = DispatcherServlet()
face_recognizer = FaceRecognizer()

LISTEN_POST = 8888

app = Flask(__name__)


# 設計 API 接口
@app.route('/attendance', methods=['POST', 'GET'])  # templates router
def index():
    print('servlet_path')
    # if request.method == "POST":
    #     user = request.form['name']
    #     session['user'] = user

    # print(servlet_path)
    db = PymysqlUtil.get_connect()
    connect = db['connect']

    operate, value = dispatcher_servlet.service(request=request, session=session, servlet_path='attendance')

    print('here')
    connect.commit()
    PymysqlUtil.close_connect()

    if 'redirect' == operate:
        return redirect(url_for(value))
    elif 'render_template' == operate:
        return render_template(value['template'], data=value['data'])
    else:
        return value


@app.route('/face_recognition/<webcam_device_id>', methods=['POST'])
def face_recognition(webcam_device_id):
    print(f'face_recognition: {webcam_device_id}')

    session['webcam_device_id'] = webcam_device_id
    request.args.set('operate', 'detection')

    db = PymysqlUtil.get_connect()
    connect = db['connect']

    print(request.args.get('test'))
    operate, value = dispatcher_servlet.service(request, session, servlet_path='face_recognition')

    connect.commit()
    PymysqlUtil.close_connect()

    return value


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=LISTEN_POST)
