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


# 캐니 엣지 디텍션 (그레이 변환, 가우시안블러(커널사이즈 5), 캐니엣지 디텍션)
def canny(image):
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    canny_image = cv2.Canny(blur, 50, 150)
    return canny_image

# ROI(관심영역) 설정
def region_of_interest(image):
    height, width = image.shape[0:2]
    
    roi = [(-width/3, height), (width*3/7, height / 2), (width*4/7, height / 2), (width+width/3, height)]
    
    vertices = np.array([roi], np.int32)
    mask = np.zeros_like(image)
    cv2.fillPoly(mask, vertices, 255)
    masked_image = cv2.bitwise_and(image, mask)
    return masked_image
    
# 직선 성분 그려주기
def display_lines(image, lines):
    line_image = np.zeros_like(image)
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line.reshape(4)
            cv2.line(line_image, (x1, y1), (x2, y2), (255, 0, 0), 3)
    return line_image

# 예상 차선 성분들 검출하여 평균내서 왼쪽 차선, 오른쪽 차선을 직선성분으로 만듬
def average(image, lines):
    width = image.shape[1]
    left_lines = []
    right_lines = []
    lane_lines = []
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line.reshape(4)
            parameters = np.polyfit((x1, x2), (y1, y2), 1)
            slope = parameters[0]
            intercept = parameters[1]
            
            # 기울기 0.4부터 1.2 까지 필터링 하고 x1,x2가 너비의 2/7 이상인 값들을 오른쪽 차선 성분으로 예상
            if slope > 0.4 and slope < 1.2 and x2 > width*2/7 and x1 > width*2/7:
                right_lines.append((slope, intercept))
            elif slope < -0.4 and slope > -1.2 and x2 < width*5/7 and x2 < width*5/7:
                left_lines.append((slope, intercept))
    
    if len(left_lines) > 0:
        left_line = np.average(left_lines, axis=0)
        
        # 검출된 차선을 포인터 형태로 변환해줌(직선 시작점 (x1, y1), 직선 끝점(x2, y2))
        lane_lines.append(make_points(image, left_line))
    if len(right_lines) > 0:
        right_line = np.average(right_lines, axis=0)
        lane_lines.append(make_points(image, right_line))

    return lane_lines

# 검출된 차선을 포인터 형태로 변환해줌(직선 시작점 (x1, y1), 직선 끝점(x2, y2))
def make_points(frame, line):
    height, width, _ = frame.shape
    slope, intercept = line
    y1 = height
    y2 = int(y1 / float(2))
    if slope == 0:
        slope = 1
    x1 = max(-width, min(2 * width, int((y1 - intercept) / float(slope))))
    x2 = max(-width, min(2 * width, int((y2 - intercept) / float(slope))))

    return [[x1, y1, x2, y2]]

# 앞에서 검출된 차선을 이미지에 그려줌
def draw_line(image, lane_lines, current_theta=0):
    height = image.shape[0]
    width = image.shape[1]
    
    # 양쪽 차선 모두 검출시
    if len(lane_lines) == 2:
        _, _, left_x2, _ = lane_lines[0][0]
        _, _, right_x2, _ = lane_lines[1][0]
        mid = int(width / float(2))

        base_x = int(left_x2 + right_x2) / 2 - float(mid)
        base_y = int(height / float(2))

        image, current_theta = display_base_line( image, current_theta, 2, base_x, base_y)
    
    # 한쪽 차선만 검출시 
    elif len(lane_lines) == 1:
        x1, _, x2, _ = lane_lines[0][0]
        
        mid = int(width/ float(2))
        base_x = x2 - float(mid)
        base_x = (x2 + float(mid))/2 - float(mid)
        base_y = int(height / float(2))
        image, current_theta = display_base_line(
            image, current_theta, 1, base_x, base_y)
            
    # 어떤 차선도 검출되지 않은 경우 수직선
    else:
        base_x = width / float(2)
        base_y = int(height / float(2))
        image, current_theta = display_base_line(image, current_theta, 0, base_x, base_y)

    for line in lane_lines:
        for x1, y1, x2, y2 in line:
            cv2.line(image, (x1, y1), (x2, y2), (255, 0, 0), 3)

    return image, current_theta

# 이미지에 직선을 그려주는 함수(이미지 중앙 수직선을 기준으로 직선을 그려줌)
def display_base_line(image, current_theta, line_num, base_x, base_y):
    height = image.shape[0]
    width = image.shape[1]
    
    angle_to_mid_radian = math.atan(base_x / float(base_y))
    angle_to_mid_deg = float(angle_to_mid_radian * 180 / float(3.14))
    
    # 오프셋 각과 최대각이내의 범위에서 조향각을 안정화 시켜줌
    angle_to_mid_deg = stabilize_angle(current_theta, angle_to_mid_deg, number_of_line=line_num)
        
    theta = angle_to_mid_deg + 90
    steering_angle_radian = theta / float(180) * float(3.14)
    x1 = int(width / float(2))
    y1 = height

    if steering_angle_radian == 0:
        x2 = int(width / float(2))
    else:
        x2 = int(x1 - height * 2 / 3 / float(math.tan(steering_angle_radian)))

    y2 = int(height / float(2))

    result = np.zeros_like(image)
    cv2.line(result, (x1, y1), (x2, y2), (0, 255, 0), 3)
    result = cv2.addWeighted(image, 1, result, 1, 1)

    return result, angle_to_mid_deg

# 오프셋 각과 최대각이내의 범위에서 조향각을 안정화 시켜줌(디폴트 오프셋 각 5도, 최대각 20도)
def stabilize_angle(curr_theta, new_theta, number_of_line, max_twoLine_angle_offset=3, max_signleLine_angle_offset=10):
    if number_of_line == 2:
        max_angle_offset = max_twoLine_angle_offset
    elif number_of_line == 1:
        max_angle_offset = max_signleLine_angle_offset
    else:
        max_angle_offset = max_signleLine_angle_offset

    angle_offset = new_theta - curr_theta
    
    if number_of_line == 0:
        fixed_angle = float(curr_theta/2)
    
    else :
        if abs(angle_offset) > max_angle_offset:
            fixed_angle = curr_theta + float(angle_offset*0.3)
        else:
            fixed_angle = curr_theta

        if fixed_angle > 20:
            fixed_angle = 20
        elif fixed_angle < -20:
            fixed_angle = -20

    return fixed_angle
        
# ----------------------- 프로그램 main부분 ------------------------------

current_theta = 0
cap = cv2.VideoCapture("lane_keep_test.mp4")
#cap = cv2.VideoCapture("challenge.mp4")
while(cap.isOpened()):
    _, image = cap.read()
    frame = cv2.resize(image, dsize=(640, 360))
    canny_image = canny(frame)
    
    cropped_image = region_of_interest(canny_image)
    
    lines = cv2.HoughLinesP(cropped_image, 2, np.pi/180,
                            50, np.array([]), minLineLength=10, maxLineGap=20)
    
    average_line = average(frame, lines)
    
    if average_line is not None:
        frame, current_theta = draw_line(frame, current_theta=current_theta, lane_lines=average_line)

    print("steering : ", current_theta, "degree")
    
    cv2.imshow("result", frame)
    cv2.imshow("resu", cropped_image)
    cv2.waitKey(1)
# -------------------------------------------------------------------------
