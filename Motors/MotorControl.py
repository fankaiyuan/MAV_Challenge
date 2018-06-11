#!/usr/bin/python
#coding: utf8

import sys
import time
import tornado.ioloop
import tornado.web
import tornado.httpserver
import tornado.options
import smbus
import math
import socket
from smbus import SMBus
from time import sleep
from tornado.options import define,options
from RPIO import PWM

define("port",default=8010,)
i2cbus = SMBus(1)
servo1 = PWM.Servo()
servo1.__init__(1, 20000, 10)

move = 0
valh = 0
MainMotor1 = 8 
MainMotor2 = 7
tail = 4
speed1 = 1150
speed2 = 1150
speed3 = 0

#servo1.set_servo(tail, speed3)

#initial movement
def init():
    print("Initialing......Take off")
    servo1.set_servo(MainMotor1, speed1)
    servo1.set_servo(MainMotor2, speed2)
def test1():
    print("test function 1")
    
def test2():
    print("test function 2")
    
#move forward
def forward():
    print("Motor 2 UP")
    global speed1, speed2
    #speed1 = speed1 - 10
    speed2 = speed2 + 10    
    servo1.set_servo(MainMotor2, speed2)
    #servo1.set_servo(MainMotor1, speed1)
    
#move backward
def backward():
    print("Motor 2 DOWN")
    global speed1, speed2
    #speed1 = speed1 - 10
    speed2 = speed2 - 10    
    servo1.set_servo(MainMotor2, speed2)
    #servo1.set_servo(MainMotor1, speed1)
#turn left
def left():
    print("MOTOR 1 up")
    global speed1, speed2
    speed1 = speed1 + 10
    #speed2 = speed2 + 10    
    #servo1.set_servo(MainMotor2, speed2)
    servo1.set_servo(MainMotor1, speed1)    
#turn right
def right():
    print("motor 1 down")
    global speed1, speed2
    speed1 = speed1 - 10
    #speed2 = speed2 - 10    
    #servo1.set_servo(MainMotor2, speed2)
    servo1.set_servo(MainMotor1, speed1)
#rise up
def up():
    print("Both up")
    global speed1, speed2
    speed1 = speed1 + 10
    speed2 = speed2 + 10    
    servo1.set_servo(MainMotor2, speed2)
    servo1.set_servo(MainMotor1, speed1)
#go down 
def down():
    print("Both down")
    global speed1, speed2
    speed1 = speed1 - 10
    speed2 = speed2 - 10    
    servo1.set_servo(MainMotor2, speed2)
    servo1.set_servo(MainMotor1, speed1)
#shut down
def cut():
    print("shutting down")
    global speed1, speed2
    speed1 = 1150
    speed2 = 1150
    speed3 = 0
    servo1.set_servo(MainMotor1, speed1)
    servo1.set_servo(MainMotor2, speed2)
    #servo1.set_servo(tail, speed3)

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
                elif(arg=='z'):
                        test1()
                elif(arg=='x'):
                        test2()
                else:
                        return False
                self.write(arg)
                
if __name__ == '__main__':
        tornado.options.parse_command_line()
        app = tornado.web.Application(handlers=[(r"/",IndexHandler)])
        http_server = tornado.httpserver.HTTPServer(app)
        http_server.listen(options.port)
        tornado.ioloop.IOLoop.instance().start()


   

