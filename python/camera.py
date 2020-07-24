import cv2
import threading
import time


class Camera:
    def __init__(self,fps=25,video_source=0):
        #Set up the camera parameters
        self.fps = fps
        self.video_source = video_source
        self.camera = cv2.VideoCapture(self.video_source)
        self.max_frames = 5
        self.frames = []
        self.isrunning = False
    def run(self):
        #When the program runs
        global thread
        try:
            #Start a thread for itself
            thread = threading.Thread(target=self._capture_loop)
            print("Starting thread...")
            thread.daemon=True
            thread.start()
            self.isrunning = True
        except (KeyboardInterrupt, SystemExit):
            print ('exit')
    def _capture_loop(self):
        #Capture the images depends on the fps
        dt = 1/self.fps
        print("Observing...")
        while self.isrunning:
            v,im = self.camera.read()
            if v:
                if len(self.frames)==self.max_frames:
                    self.frames = self.frames[1:]
                self.frames.append(im)
            time.sleep(dt)
    def stop(self):
        self.isrunning = False
    def get_frame(self, bytes=True):
        if len(self.frames)>0:
            if bytes:
                img = cv2.imencode('.png',self.frames[-1])[1].tobytes()
            else:
                img = self.frames[-1]
        else:
            with open("images/not_found.jpeg","rb") as f:
                img = f.read()
        return img
        