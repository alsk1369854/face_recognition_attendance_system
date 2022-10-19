from flask import Flask, render_template, request, Response
import json
import numpy as np
import cv2
import uuid

LISTEN_POST = 8888

app = Flask(__name__)


# 設計 API 接口
@app.route('/')  # web router
def index():
    # response Web Page
    return 'test'


@app.route('/imageUpdate', methods=['POST'])
def imageUpdate():
    # save image
    with open('test.jpeg', 'wb') as f:
        f.write(request.data)

    # build image
    nparr = np.frombuffer(request.data, dtype=np.uint8)
    # decode image
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    # build response json
    result = [{'x1': 0, 'y1': 10, 'x2': 10, 'y2': 20, 'name': 'ming'}]
    response = json.dumps(result)

    return Response(response=response, status=200, mimetype="application/json")


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=LISTEN_POST)
