#!/usr/bin/python
#coding: utf8

import sys
import time
import tornado.ioloop
import tornado.web
import tornado.httpserver
import tornado.options
from tornado.options import define,options

define("port",default=8010,type=int)


#initial movement
def init():
    print("Initialing......Take off")
#move forward
def forward():
    print("moving forward")
#move backward
def backward():
    print("moving Backward")
#turn left
def left():
    print("Turning Left")
#turn right
def right():
    print("Turning Right")
#rise up
def up():
    print("Rising up")
#go down 
def down():
    print("Going down")
#shut down
def cut():
    print("shutting down")

class IndexHandler(tornado.web.RequestHandler):
        def get(self):
                self.render("index.html")
        def post(self):
                arg = self.get_argument('k')
                if (arg == 'i'):
                        init()
                elif(arg=='w'):
                        forward()
                elif(arg=='s'):
                        backward()
                elif(arg=='a'):
                        left()
                elif(arg=='d'):
                        right()
                elif(arg=='q'):
                        up()
                elif(arg=='e'):
                        down()
                elif(arg=='p'):
                        cut()
                else:
                        return False
                self.write(arg)
                
if __name__ == '__main__':
        tornado.options.parse_command_line()
        app = tornado.web.Application(handlers=[(r"/",IndexHandler)])
        http_server = tornado.httpserver.HTTPServer(app)
        http_server.listen(options.port)
        tornado.ioloop.IOLoop.instance().start()


   

