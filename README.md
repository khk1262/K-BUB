# K-BUB
#### autonomous car contest preparing
#### koreatech

## 1. 현재 각 분야 진행 상황

* 라이다
> 유클리디안 클러스터링을 이용하여 장애물을 분류, 해당 장애물로부터 centoid 검출하여 라이다와의 거리와 각도 출력
> ccw 알고리즘 이용하여 주차공간 내에 point가 검출이 되면 주차 불가, 검출 되지 않을시 주차 가능하도록 bool값 출력 및 시각화

* 비전
> [YOLO v4](https://github.com/AlexeyAB/darknet)를 이용한 표지판, 신호등 인식 학습(mAP 96.88%)  
> Open CV 를 이용하여 이미지를 전처리 한 후, 슬라이딩 윈도우를 이용하여 차선인식 진행중  

> [2020/08/01 가중치 파일 다운로드](https://koreatechackr-my.sharepoint.com/:u:/g/personal/jun752601_koreatech_ac_kr/EUX4ebVUXcdDituoYFIJ0_QBPDAALaskfOO81BMw7soqHw?e=yqGHH4)  
> [2020/08/01 이미지 파일 다운로드(용량 매우큼 14.9GB)](https://koreatechackr-my.sharepoint.com/:u:/g/personal/jun752601_koreatech_ac_kr/EY2g1l3JskpPjfq7mptidfEBGpwcxWpImXruilVjKeyHNQ?e=b6OPT3)

* 플래닝
> [2020/08/17](https://koreatechackr-my.sharepoint.com/:u:/g/personal/jun752601_koreatech_ac_kr/EY_P6HrQh45JoHGBB67nYUUBLCQaM419OGnzAJ9BuYF2Gg?e=5gqXwY)

##### 표지판 인식 모습

<img src="/Vision/vision/image/recognition.jpg" width="900px" height="450px" title="vision" alt="recognition"></img><br/>

##### 차선 인식 모습
<img src="/Vision/vision/image/image01.png" width="450px" height="225px" title="vision" alt="recognition"></img><br/>

* 로컬
> hd map의 json 파일 내용을 ROS 환경에서 읽어오는 작업 및 IMU 센서 데이터 추출 작업
