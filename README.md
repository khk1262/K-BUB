# K-BUB
#### autonomous car contest preparing
#### koreatech

## 1. 현재 각 분야 진행 상황

* 라이다
> 유클리디안 클러스터링을 이용하여 장애물을 분류, 해당 장애물로부터 centoid 검출하여 라이다와의 거리와 각도 출력

* 비전
> 현재 yolo를 이용한 표지판 인식, 신호등 인식 등을 대략 완성시켰고 차선 인식 및 ROS 적용을 진행 중

<img src="/Vision/vision/image/recognition.jpg" width="450px" height="300px" title="px(픽셀) 크기 설정" alt="recognition"></img><br/>
###### ==표지판 인식 모습==

* 로컬
> hd map의 json 파일 내용을 ROS 환경에서 읽어오는 작업 및 IMU 센서 데이터 추출 작업
