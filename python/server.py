from flask import Flask, Response
from camera import Camera
import time
camera = Camera()
time.sleep(1)
camera.run()
time.sleep(1)
app = Flask(__name__)


#Define a generator
def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/png\r\n\r\n' + frame + b'\r\n')

@app.route("/")
def video_feed():
    return Response(gen(camera),
        mimetype="multipart/x-mixed-replace; boundary=frame")

if __name__=="__main__":
    #Run the server with local host address with port 5000
    app.run(host='0.0.0.0',port='5000')
    
    

