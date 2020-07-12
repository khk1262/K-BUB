# 2020.02 ~ 2020.05.
# 김용빈
# 차선인식 알고리즘
# https://www.youtube.com/watch?v=eLTLtUVuuy4
# 위의 유튜브 사이트 참고
# 파이썬 3.8
# openCV 4.12 사용

# -------알고리즘 순서---------
# 프레임 받아오기 
# 프레임 사이즈 변환
# 케니 엣지 디텍션 (canny)
# ROI 설정 (region_of_interest)
# 허프변환
# 예상 차선 성분들 검출 (average -> make_points)
# 차선 직선으로 그려주고, 현재각 받아오기 (draw_line -> display_base_line -> stabilize_angle)
# -----------------------------



# --------------------------------- 함수 정의 ------------------------------------------------
import cv2
import numpy as np
import math
from matplotlib import pyplot as plt

lower_yellow = np.array([0, 70, 120], dtype=np.uint8)
upper_yellow = np.array([179, 255, 255], dtype=np.uint8)

lower_white = np.array([150, 150, 150], dtype=np.uint8)
upper_white = np.array([255, 255, 255], dtype=np.uint8)



def bird_eyes_view(frame):
    image = frame[int(frame.shape[0]*3/5):frame.shape[0], 0:frame.shape[1]]
    
    height,width = image.shape[0:2]

    src = np.float32(
        [[0, height], [width, height], [0, 0], [width, 0]])
    dst = np.float32(
        [[width*2/5, height], [width*3/5, height], [0, 0], [width, 0]])
    M = cv2.getPerspectiveTransform(src, dst)  # The transformation matrix
    Minv = cv2.getPerspectiveTransform(dst, src)  # Inverse transformation

    bird_image = cv2.warpPerspective(image, M, (width, height))
    
    return bird_image
    
# 캐니 엣지 디텍션 (그레이 변환, 가우시안블러(커널사이즈 5), 캐니엣지 디텍션)
def canny(image):
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    canny_image = cv2.Canny(blur, 50, 150)
    return canny_image

# ----------------------- 프로그램 main부분 ------------------------------


cap = cv2.VideoCapture("challenge.mp4")
#cap = cv2.VideoCapture("lane_keep_test.mp4")
while(cap.isOpened()):
    _, frame = cap.read()
    
    image = cv2.resize(frame, dsize=(640, 360))
    bird_image = bird_eyes_view(image)
    #gray_image = cv2.cvtColor(bird_image, cv2.COLOR_RGB2GRAY)
    #blur_image = cv2.GaussianBlur(gray_image, (5, 5), 0)
    #binary_image = cv2.adaptiveThreshold(blur_image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 21, 5)
    
    hsv_image = cv2.cvtColor(bird_image, cv2.COLOR_BGR2HSV)

    # HSV, RGB 필터링으로 영상을 이진화 함
    yellow_image = cv2.inRange(hsv_image, lower_yellow, upper_yellow)
    
    #yellow_image = yellow_image[:, int(yellow_image.shape[1]/5):int(yellow_image.shape[1]*5/9)]
        
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    morph_image = cv2.morphologyEx(yellow_image, cv2.MORPH_CLOSE, kernel, iterations=3)
    
    """
    for i in range(0,morph_image.shape[0]):
        for j in range(10,morph_image.shape[1]):
            if (morph_image[i,morph_image.shape[1]-j-1] == 255):
                #cv2.circle(morph_image, (morph_image.shape[1]-j-1, i), 5, (255,0,0), 3)
                break
    """
    
    #canny_image = cv2.Canny(yellow_image, 50, 150)
    #white_image = cv2.inRange(bird_image, lower_white, upper_white)
    

    cv2.imshow("src", morph_image)
    cv2.imshow("result", bird_image)
    cv2.imshow("binary", image)
    cv2.waitKey(1)

# -------------------------------------------------------------------------