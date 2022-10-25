import os
from flask import Flask,  redirect, url_for, request, Response, session, send_from_directory
from datetime import timedelta
import json
# import uuid
from src.Server_manager.MVC.DispatcherServlet import DispatcherServlet
from src.Server_manager.Database_connect.PymysqlUtil import PymysqlUtil
from src.Face_recognition.FaceRecognizer import FaceRecognizer

dispatcher_servlet = DispatcherServlet()


app = Flask(__name__, static_folder='front_end/build')
app.secret_key = 'attendance_server'
app.permanent_session_lifetime = timedelta(days=31)


# Serve React App
@app.route('/', defaults={'path': ''})
@app.route('/<path:path>')
def serve(path):
    if path != "" and os.path.exists(app.static_folder + '/' + path):
        return send_from_directory(app.static_folder, path)
    else:
        return send_from_directory(app.static_folder, 'index.html')

@app.route('/attendance', methods=['POST', 'GET'])  # templates router
def attendance():
    # if request.method == "POST":
    #     user = request.form['name']
    #     session['user'] = user
    print('here')

    db = PymysqlUtil.get_connect()
    connect = db['connect']

    operate = None
    value = None
    try:

        operate, value = dispatcher_servlet.service(request=request, session=session, servlet_path='attendance')

        connect.commit()
    except Exception as error:
        print('app error: ' + repr(error))
        connect.rollback()

    PymysqlUtil.close_connect()

    if 'redirect' == operate:
        return redirect(url_for(value))
    elif 'render_template' == operate:
        return send_from_directory(app.static_folder, value['template'])
    else:
        return value


@app.route('/face_recognition/<webcam_device_id>', methods=['POST'])
def face_recognition(webcam_device_id):
    print(f'face_recognition: {webcam_device_id}')

    session['webcam_device_id'] = 'A001'

    db = PymysqlUtil.get_connect()
    connect = db['connect']

    operate = None
    value = None
    try:

        # print(request.args.get('test'))
        operate, value = dispatcher_servlet.service(request, session, servlet_path='face_recognition')

        connect.commit()
    except Exception as error:
        print('app error: ' + repr(error))
        connect.rollback()

    PymysqlUtil.close_connect()

    return value


if __name__ == '__main__':
    app.secret_key = 'super secret key'
    app.config['SESSION_TYPE'] = 'filesystem'

    # app.run(use_reloader=True, port=5000, threaded=True)
    app.run(host="0.0.0.0", port=8888, use_reloader=True, threaded=True)
