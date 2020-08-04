#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import math
import matplotlib.pyplot as plt
import numpy as np
from control_to_serial.msg import control2serial
from std_msgs.msg import Float32
from std_msgs.msg import Float64
from nav_msgs.msg import Odometry
k = 1 #6 #k는 반드시 전륜차 속도의 변화에 따라 변화해야 한다. k= a*v+b, 계수는 스스로 속도를 조절해야 한다
kp = 0 #20
ki = 0 #0.0001
dt = 0.02
L = 1.8
Lambda = 1 #stanley는 각도 출력의 무게를 계산해 크면 클수록 코너링 효과는 좋지만 직선으로 걸었을 때 효과가 떨어진다
delta = 0
ex_cx = 0
ex_cy = 0
cur_cx = 0
cur_cy = 0
targetV = 3.0

class VehicleState:
    def __init__(self,x,y,yaw,v):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v
        self.a = 0.0
        #self.e = []


    def Update(self):
        self.yaw = self.NormalizeAngle(self.yaw)

    def NormalizeAngle(self,angle):
        if (angle > math.pi):
            angle = angle - 2*math.pi
        elif (angle < -math.pi):
            angle = 2*math.pi + angle
        else:
            angle = angle

        return angle

    def AxisTrans(self,x,y):
        tx = (x-self.x) * math.cos(self.yaw) + (y - self.y) * math.sin(self.yaw)
        ty = -(x -self.x) * math.sin(self.yaw) + (y - self.y) * math.cos(self.yaw)
        return tx,ty

    def PID(self,targetV):
        self.a = kp * (targetV - self.v) + ki*(targetV - self.v)*dt


    def Calculate(self,cx,cy):

#----------------------------------------------------------------------

#        dx = [self.x - ix for ix in cx ]
#        dy = [self.y - iy for iy in cy]
#        d = [math.sqrt(tx**2 + ty**2) for (tx,ty) in zip(dx,dy)]
#        e = min(d)
#        targetIndex = d.index(e)
#        preViewInd = targetIndex + 3

#        if (preViewInd >= len(cx)):
#            preViewInd = len(cx) - 1

#        preX,preY = self.AxisTrans(cx[preViewInd],cy[preViewInd])
#        preViewDelta = self.NormalizeAngle(math.atan2(preY,preX))

#        tmp_y = -(cx[targetIndex] -self.x)* math.sin(self.yaw) + (cy[targetIndex]-self.y)*math.cos(self.yaw)
#        if (tmp_y < 0):
#            e = -e
#        elif (tmp_y == 0):
#            e = 0
#        else:
#            e = e

#        index = []
#        ind = 0

#        while ind < (len(cx)-1):
#            ind_tmp = math.atan2(cy[ind+1]-cy[ind],cx[ind+1]-cx[ind])
#            ind_tmp = self.NormalizeAngle(ind_tmp)
#            index.append(ind_tmp)
#            ind += 1

#        if targetIndex < len(cx)-1:
#            targetYaw = index[targetIndex]
#        else:
#            targetYaw = index[len(cx)-2]

#--------------------------------------------------------------------------
        targetIndex = 0
        preViewDelta = 0

        dx = self.x - cur_cx
        dy = self.y - cur_cy
        e = math.sqrt(dx**2 + dy**2)

        tmp_y = -(cur_cx -self.x)* math.sin(self.yaw) + (cur_cy-self.y)*math.cos(self.yaw)
        if (tmp_y < 0):
            e = -e
        elif (tmp_y == 0):
            e = 0
        else:
            e = e

        targetYaw = math.atan2(cur_cy-ex_cy,cur_cx-ex_cx)


        return e,targetYaw,targetIndex,preViewDelta

    def StanleyControl(self,e,targetYaw,preViewDelta):
        phi = 0.0
        deltaStanley = 0.0
        deltaPreview = 0.0

        deltaMax = 28 * math.pi /180

        if (targetYaw - self.yaw) < math.pi:
            phi = targetYaw- self.yaw
        else:
            if (targetYaw > 0):
                phi = -2*math.pi + targetYaw - self.yaw
            else:
                phi = 2*math.pi - targetYaw + self.yaw

# ---------------------------------------------------------------------

        phi = self.NormalizeAngle(phi)
        back = False

        if (abs(phi) > math.pi / 2 ):
            back = True

        if (back == True and phi > 0):                  # 후진일 경우
            phi = math.pi - phi
        else:
            if (back == True and phi < 0):
                phi = -math.pi - phi
            else:
                phi = phi

# ----------------------------------------------------------------------

        deltaStanley = phi + math.atan2(k * e , self.v )
        delta = Lambda * deltaStanley + (1-Lambda)* preViewDelta


        if (delta > deltaMax):
            delta = deltaMax
        elif (delta < -deltaMax):
            delta = -deltaMax
        else:
            delta = delta


        return delta

def callback(msg):

    global ex_cx
    global ex_cy
    global cur_cx
    global cur_cy


    ex_cx = cur_cx
    ex_cy = cur_cy

    cur_cx = msg.pose.pose.position.x  # path x
    cur_cy = msg.pose.pose.position.y  # path y


def callback2(msg):

    global delta

    state = VehicleState(x =msg.pose.pose.position.x  , y = msg.pose.pose.position.y ,yaw = yaw_data, v = 3.0)
    state.Update() # gps present pose
    e, targetYaw, targetIndex,preViewDelta = state.Calculate(cur_cx,cur_cy)

    delta = state.StanleyControl(e,targetYaw,preViewDelta) *180 /math.pi

    print(" delta = ", delta)

    delta = delta + targetV * 1000

    print("ex", ex_cx,ex_cy)
    print("cur",cur_cx,cur_cy)

def callback4(msg):

    global yaw_data

    yaw_data = msg.data

def main():


    rospy.init_node('stanley', anonymous=True)
    pub = rospy.Publisher('delta',Float32,queue_size=10)

    rospy.Subscriber("gps_meas", Odometry, callback)
    rospy.Subscriber("gps_meas2", Odometry, callback2)
    #rospy.Subscriber("vel", Float64, callback3)
    rospy.Subscriber("yaw", Float64, callback4)


    rate = rospy.Rate(8)

    while not rospy.is_shutdown():


        delta_float = delta # % rospy.get_time()
        rospy.loginfo(delta_float)
        pub.publish(delta_float)
        rate.sleep()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
