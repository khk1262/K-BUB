#!/usr/bin/env python
import rospy
from std_msgs.msg import Int16
import numpy as np
import cv2
#!/usr/bin/env python3.6
import darknet

net = darknet.load_net("/home/usera/catkin_ws/src/signs_sender/scripts/yolov4-custom2.cfg".encode('utf-8'),
                        "/home/usera/catkin_ws/src/signs_sender/scripts/y.weights".encode('utf-8'), 0)
meta = darknet.load_meta("/home/usera/catkin_ws/src/signs_sender/scripts/plate2.data".encode('utf-8'))


sign = 0

def retbox(detections,i,frame) :
    label = detections[i][0].decode('utf-8')
    score = detections[i][1]
    #print(label, score)
    x1 = int(round((detections[i][2][0]) - (detections[i][2][2]/2.0))) # top left x1 
    y1 = int(round((detections[i][2][1]) - (detections[i][2][3]/2.0))) # top left xy 
    x2 = int(round((detections[i][2][0]) + (detections[i][2][2]/2.0))) # bottom right x2 
    y2 = int(round((detections[i][2][1]) + (detections[i][2][3]/2.0))) # bottom right y2 
    
    box = np.array([x1,y1,x2,y2])
    #print(box)
    return label, score, box

def talker():
    global frame
    pub = rospy.Publisher('signs', Int16)
    rospy.init_node('sign_sender', anonymous = True)
    rate = rospy.Rate(10) #10hz
    while not rospy.is_shutdown():
        signs_int, frame = detection(sign, cap, threshold)
        cv2.imshow("Yolov4", frame)
        cv2.waitKey(1)
        rospy.loginfo(signs_int)
        pub.publish(signs_int)
        rate.sleep()

def detection(sign, cap, threshold):
    global frame
    ret, frame = cap.read()
    detections = darknet.detect_image(net,meta, frame , thresh=.5)
   
    
    for i in range(len(detections)) :
        label , score , box  = retbox(detections,i,frame)
        
        print(label, score, box)
	cv2.rectangle(frame, (box[0], box[1]), (box[2], box[3]), (255, 0, 255), 1)
        cv2.putText(frame, label, (box[0], box[1]), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 0, 255), 2, cv2.LINE_AA)
        if score > threshold :
            if label == "GreenLight":
                sign = 1
            elif label == "Redlight":
                sign = 2
            elif label == "GreenLeft":
                sign = 3
            elif label == "LeftSign":
                sign = 4
            elif label == "RightSign":
                sign = 5
            elif label == "StaticObstacle":
                sign = 6
            elif label == "UnexpectedObstacle":
                sign = 7
            elif label == "CrossWalkSign":
                sign = 8
            elif label == "ParkingSign":
                sign = 9
            elif label == "BusSign":
                sign = 10
            elif label == "SchoolSign":
                sign = 11
            elif label == "SpeedBump":
                sign = 12
            else:
                sign = 10
         
            #break
        #else :
        #    left,top,right,bottom=box
        
        
    return sign, frame


######################main###########################        



#cv2 read image from webcame
#cap = cv2.VideoCapture("/home/mun/darknet/tttt.mp4")
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
threshold = 0.7

if __name__=='__main__':
    try:
         talker()

    except rospy.ROSInterruptException:
         pass




       # print(456)

        

    cv2.imshow('frame',frame)
    k = cv2.waitKey(1)
    #if k == 27 :
    	#break

