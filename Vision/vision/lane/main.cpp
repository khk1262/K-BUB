#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <serial_example/Msg.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>


//ros 헤더파일
//#include <ros/ros.h>
//#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace std;

//함수 선언

Mat preprocessing(Mat img);
Mat bird_eyes_view(Mat img);
Mat mask_filter(Mat img, int w, int h);
Mat mask_filter2(Mat img, int _mask_w, int _mask_h, int thresh);
vector<Point> sliding_window(Mat img);
vector<double> polyFit(vector<Point> px, int i, int degree);
const int Correction = 150;
const int Num_of_sliding = 18; // k-bub -> 18, digist -> 10 슬라이딩윈도우의 개수
int h = 15; //digist -> 10, k-bub -> 11 슬라이딩 윈도우의 세로 크기
const double PI = 3.1415926535897932384626433832795028841971693993751058209;
const int p[11] = { 0, 0, 184, 168, 149, 129, 110, 90, 70, 50, 30 }; // m별 좌표값(2, 4, 6, 8, 10 만 측정)
int sliding_count = 0;
int isStop = 0;
Mat img_white;
Mat temp;
//ros::Publisher isStop_pub_;


int main(int argc, char** argv)
{
	ros::init(argc, argv, "lane_publisher");
	
	
	ros::NodeHandle n;
	ros::Publisher lane_pub = n.advertise<serial_example::Msg>("lane_msg",100);
	ros::Publisher lane_pub2 = n.advertise<std_msgs::Float64>("lane_msg2", 100);
	ros::Publisher isStop_pub_ = n.advertise<std_msgs::Bool>("isStop", 10);
    // isStop_pub_ = n.advertise<std_msgs::Bool>("isStop", 10);

	serial_example::Msg msg;
	
	ros::Rate loop_rate(10);
	//ros::init(argc, argv, "cv_bridge_test");

	//cap1을 받아옴(동영상 : 경로지정, 웹캠 : 웹캠번호 지정(0, 1, 2, ...)
	//VideoCapture cap1("C:\\python_study\\jupyternotebook\\test.mp4");
	VideoCapture cap1(0);
	Mat frame1, left, left_gray;
	cap1.set(cv::CAP_PROP_FRAME_WIDTH, 640); //이미지 사이즈 정해줌
	cap1.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	while (cap1.isOpened())
	{
		
		//cap1로 부터 한프레임을 받아옴
		cap1 >> frame1;
		
		imshow("src", frame1); // 소스 영상 보기
		
		frame1 = preprocessing(frame1); // 이미지 전처리
										//(리사이즈, 가우시안블러, HSV, 노란영역추출, 적분영상, 마스크필터, 버드아이뷰)

		// Point* left_point;
		vector<Point> left_point = sliding_window(frame1); // 슬라이딩 윈도우를 통한 좌표 탐색
		cvtColor(frame1, frame1, COLOR_GRAY2BGR); // Gray to BGR

		vector<Point>::iterator iter;
		for (iter = left_point.begin(); iter != left_point.end(); ++iter) {
			// cout << *iter <<" ";
			circle(frame1, Point(iter->x, iter->y), 3, Scalar(255, 0, 0), 1); // 예상지점 검출
		}
		cout << endl;



		int min = frame1.cols;
		int max = 0;
		vector<Point> left_point1(sliding_count);
		for (int i = 0; i < sliding_count; i++)
		{
			left_point1[i].x = left_point[i].x;
			left_point1[i].y = left_point[i].y;
		}
		
		
		for (int i = 0; i < Num_of_sliding; i++)
		{
			if (min > left_point[i].x && left_point[i].x != 0) min = left_point[i].x;
			if (max < left_point[i].x) max = left_point[i].x;
		}

		//vector<double> left_pos = polyFit(left_point, Num_of_sliding, 1); // 함수 근사
		vector<double> left_pos = polyFit(left_point, sliding_count, 1); // 함수 근사
		//for (int i = 0; i < left_pos.size(); i++) {
			//printf("x^%d : %f  ", i, left_pos[i]);
		//}
		//printf("\n");
		
		int py = 0;
		double x = 0;
		//근사한 함수 그려주는 while문.
		//printf("max = %d, min = %d\n", max, min);
		int width = frame1.cols;
		int height = frame1.rows;
		double y1 = height;
		double x1 = (y1 - left_pos[0]) / left_pos[1] + Correction;
		double y2 = 0;
		double x2 = (y2 - left_pos[0]) / left_pos[1] + Correction;
		//printf("(%f %f) (%f %f)\n", x1, y1, x2, y2);
		line(frame1, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 255), 2);
		line(temp, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 255), 2);
		
		double length = sqrt(pow((x2 - (width / 2)),2) + pow((height-1), 2));
		double angle = acos((height-1)/length)*180/PI;
		
		//double angle = cos(3.1415916);
		//printf("%d %d %f, %f", width, height,length, x2);
		line(temp, Point(width/2, height), Point(x2, 0), Scalar(255,0,255), 1);
		printf("angle : %f", angle);
//2m,4m,6m,8m,10m 좌표값 구하기
		double xy[5];
		for(int j = 0; j < 5; j++)
		{
			double temp_x = (p[j * 2 + 2] - left_pos[0]) / left_pos[1] + 30;
			circle(frame1, Point(temp_x, p[j*2+2]), 3, Scalar(255, 255, 0), -1); // 예상지점 검출
			//xy[j] = Point(temp_x, p[j * 2 + 2]);
			//printf("%d : %f\n", j * 2 + 2, temp_x);
		}

				
		/*     ros 통신 시작   */
		if(x2 < width / 2)	angle *= -1;
		if(angle > 28)	angle = 28.0;
		else if(angle < -28)	angle = -28.0;
		printf("-----------[%f]", angle);
		//printf("%d %d\n", width, height); 
		std_msgs::Float64 angle1;
		std_msgs::Bool isStop_msg;
		isStop_msg.data = (bool)isStop;
		angle1.data = angle;
		lane_pub2.publish(angle1);
		isStop_pub_.publish(isStop_msg);

		msg.data = angle;
		//msg.x0= left_pos[0];
		//msg.x1= left_pos[1];
		//msg.x2= left_pos[2];
		//msg.x3= left_pos[3];
		lane_pub.publish(msg);// 메시지를발행한다
		/*     ros 통신 끝     */
		//
		line(temp, Point(width / 2 - Correction, 0), Point(width / 2 - Correction, height), Scalar(0,255,255), 1);
		line(temp, Point(width / 2 + Correction, 0), Point(width / 2 + Correction, height), Scalar(0,255,255), 1);
		//
		line(frame1, Point(width / 2, 0), Point(width / 2, frame1.rows), Scalar(0, 0, 255), 1); //중앙선(erp-42가 중앙이오도록)
		line(temp, Point(width / 2, 0), Point(width / 2, frame1.rows), Scalar(0, 0, 255), 1); //중앙선(erp-42가 중앙이오도록)
		for(int i = 2; i<11; i++){
			if(i % 2)
			{
				line(frame1, Point(width - 100, p[i]), Point(100, p[i]), Scalar(255,0,0), 1);
			}
			else
			{
				line(frame1, Point(width-1, p[i]), Point(0, p[i]), Scalar(0,255,0), 1);
			}
			
		}
		
		imshow("result", frame1); // 결과영상 출력
		imshow("result2", temp);
		if (waitKey(1) == 27) break; // 'ESC' or 'Ctrl + C' 를 통해 종료
	}
	return 0;
}

Mat preprocessing(Mat img) {
	Mat img_resize;
	cv::resize(img, img_resize, cv::Size(640, 360), 0, 0); //이미지 리사이즈(원본사이즈 -> 640x360)
	cv::GaussianBlur(img_resize, img_resize, Size(5, 5), 1.0); // 가우시안 블러(5,5)
	Mat img_hsv;
	cv::cvtColor(img_resize, img_hsv, COLOR_BGR2HSV); // HSV영상으로 변환
	Mat img_binary; //0,50,120
	cv::inRange(img_hsv, cv::Scalar(0, 50, 120), cv::Scalar(180, 255, 255), img_binary); //노란색 영역 추출
	cv::inRange(img_resize, cv::Scalar(120, 120, 120), cv::Scalar(255, 255, 255), img_white);
	
	Mat temp2;
	cvtColor(img_binary, temp2, COLOR_GRAY2BGR);
	//imshow("123123", temp2);
	Mat img_warp;
	img_warp = bird_eyes_view(img_binary); // 버드아이뷰 전환
	img_white = bird_eyes_view(img_white);
	
	///////////////////////////////////////////////////거리측정 테스트///////////////////
	int width = 640;																
																					
	temp = bird_eyes_view(img_resize);
	for(int i = 2; i<11; i++){
			if(i % 2)
			{
				line(temp, Point(width - 100, p[i]), Point(100, p[i]), Scalar(255,0,0), 1);
			}
			else
			{
				line(temp, Point(width-1, p[i]), Point(0, p[i]), Scalar(0,255,0), 1);
			}
			
		}
	//printf("%d %d --> ", img_resize.cols, img_resize.rows);
	//printf("%d %d\n", temp.cols, temp.rows);
	//temp = Distance(temp);
	//imshow("temp", temp); // 결과영상 출력
	/////////////////////////////////////////////////////////////////////////////////
	
	int height = img_warp.rows;
	width = img_warp.cols;
	//erp42 보이지 않도록 영역 제거
	rectangle(img_warp, Point(width / 2 - 20, height - 140), Point(width / 2 + 20, height-180), Scalar(0, 0, 0), -1);//erp42 보이지 않도록 영역 제거
	rectangle(temp, Point(width / 2 - 20, height - 140), Point(width / 2 + 20, height-180), Scalar(0, 0, 0), -1);//erp42 보이지 않도록 영역 제거

	//imshow("warp_img", img_warp);

	Mat img_integral; // 적분영상
	Mat img_integral_white;
	cv::integral(img_white, img_integral_white);
	cv::integral(img_warp, img_integral);
	img_white = bird_eyes_view(img_white);
	cvtColor(img_white, img_white, COLOR_GRAY2BGR);
	Mat img_mask;
	img_white = mask_filter2(img_integral_white, 5, 5, 85);
	img_mask = mask_filter(img_integral, 5, 5);  // 마스크필터
	//return img_mask; 
	return img_mask;
}

Mat mask_filter2(Mat img, int _mask_w, int _mask_h, int thresh) {
	int height = img.rows;
	int width = img.cols;
	Mat img_maskfilter;
	img_maskfilter = Mat::zeros(height, width, CV_8UC1);
	Mat img_stop;
	img_stop = Mat::zeros(height, width, CV_8UC3);
	float mask[3];
	int sx = 0;

	isStop = 0;
	// imshow("before mask", img);

	uint* image = (uint*)img.data;
	uchar* score_data = (uchar*)img_maskfilter.data;
	int mask_w = _mask_w, mask_h = _mask_h;

	int sy = 0;

	int roi_w = 80;
	int histo = 0;
	//cvtColor(img, img, COLOR_GRAY2BGR);
	for (int y = 20; y < height - 21;  y++) {
		histo = 0;
		for (int x = int(width/2) - roi_w; x <= int(width/2) + roi_w; x++) {
			for (int i = 0; i < 3; i++) {
				sy = y + (2 * mask_h + 1) * (i - 1);
				int dx, cx, bx, ax;
				int dy, cy, by, ay;
				dy = sy + mask_h;
				dx = x + mask_w;
				cy = sy - mask_h - 1;
				cx = x + mask_w;
				by = sy + mask_h;
				bx = x - mask_w - 1;
				ay = sy - mask_h - 1;
				ax = x - mask_w - 1;
				// printf(" dy : %d, cy : %d, by : %d, ay : %d\n", dy, cy, by, ay);
				// printf(" dx : %d, cx : %d, bx : %d, ax : %d\n", dx, cx, bx, ax);
				mask[i] = image[(dy) * width + dx] - image[(cy) * width + cx] - image[(by) * width + bx] + image[(ay) * width + ax];
			}

			float sum = ((mask[1] - mask[0]) + (mask[1] + mask[2])) / 2;
			// printf("sum : %f\n", sum);
			//cout << "sum : " << sum << endl;
			if (sum > 20000) {
				// cout << "sum :" << sum << endl;
				// circle(img_maskfilter, Point2i(x, y), 5, Scalar(255, 0, 0), 3);
				score_data[width * y + x] = 255;
				histo++;
			}
		}
		//printf("histo : %d\n");
		if (histo > thresh) {
			line(img_stop, Point(int(width / 2) - roi_w, y), Point(int(width / 2) + roi_w, y), (255, 0, 255), 3);
			printf("histo : %d\n", histo);
			printf("**************************8stop line!******************************");
			isStop = 1;
			break;
		}
		
		
	}
	//imshow("stop line", img_stop);
	// imshow("after mask", img_maskfilter);
	//std_msgs::Bool isStop_msg;
	//isStop_msg.data = isStop;
	//isStop_pub_.publish(isStop_msg);
	

	return img_maskfilter;
}
vector<Point> sliding_window(Mat img)
{	
	sliding_count = 0;
	//position_initialize();
	vector<Point> left_peak(Num_of_sliding+15);
	int left_pos[Num_of_sliding];
	int right_pos[Num_of_sliding];
	for (int i = 0; i < Num_of_sliding; i++)
	{
		left_pos[i] = 190;
		left_peak[i].x = 0;
		left_peak[i].y = 1;
	}

	int roi_width = 250; // 첫 검출시까지 roi 영역
	int height = img.rows; // 이미지의 height, width
	int width = img.cols;
	int left_roi = width ; // width * 0.6 이후의 지점은 검사하지 않음

	Mat image = img.clone();
	Mat left = image(Range(0, height), Range(0, left_roi));

	int left_count = 0;
	int left_width = width;
	int no_cnt = 0;

	cv::cvtColor(img, img, COLOR_GRAY2BGR);
	
	int prev_temp;
	int cur_temp;
	for (int i = Num_of_sliding-1; i > -1; i--)
	{
		/////////////////////////
		if( i > 10)
		{
			h = 5;
		}
		else if (i > 5) h= 11;
		else
		{
			h = 15;
		}
		

		///////////////////////////
		int max_value = -1;
		int max_index = 0;
		int w1 = left_pos[i] - roi_width / 2;
		int w2 = left_pos[i] + roi_width / 2;
		if (w1 < 0)  w1 = 0;
		else if (w1 > left_width ) w1 = left_width;
		if (w2 > left_width) w2 = left_width;
		else if (w2 < 0) w2 = 0;
		//roi 30 x h, 개수 : Num_of_slinding
		//Mat roi = left(Range((i + 1) * h + 1, (i + 2) * h), Range(w1, w2));
		//rectangle(img, Point(w1, (i + 1) * h + 1), Point(w2, (i + 2) * h), Scalar(255, 255, 255), 1);
		//////////////////////////////////////
		if(i > 10){
			prev_temp = (i+2) * h + 120;
			cur_temp = prev_temp - h + 1;
		}
		else if(i > 5){
			prev_temp = (i+2) * h + 48;
			cur_temp = prev_temp - h + 1;
		}
		else
		{
			prev_temp = (i+2) * h + 20;
			cur_temp = prev_temp - h + 1;
		}
		Mat roi = left(Range( cur_temp,prev_temp), Range(w1, w2));
		rectangle(img, Point(w1, prev_temp), Point(w2, cur_temp), Scalar(255, 0, 255), 1);
		//printf("%d %d %d %d %d\n",i, (i + 1) * h + 1,   (i + 2) * h, prev_temp, cur_temp);
		////////////////////////////////////////
		
		uchar* score_data = (uchar*)roi.data;
		imshow("sliding_window", img);
		//cout << roi.cols << endl;
		for (int x = 0; x < roi.cols; x++)
		{
			// int peaks[31] = { 0, };
			int cnt = 0;
			for (int y = 0; y < roi.rows; y++)
			{
				//roi영역 내에서 흰색 점이 검출될 경우 cnt 증가
				if (score_data[width * y + x] != 0) cnt++;
			}
			//peaks[x] = cnt;
			if (cnt >= max_value)
			{
				max_value = cnt;
				max_index = x;
			}

		}

		if (max_value > 0)
		{
			if (left_count > 0) 
			{
				if (abs(left_peak[left_count - 1].x - (max_index + w1)) < 50) 
				{
					roi_width = 40;
					//peak값 설정
					
					left_peak[left_count].x = max_index + w1;
					left_peak[left_count++].y = (int)((prev_temp + cur_temp)/2);
					sliding_count++;
					//다음 roi의 센터 영역 설정
					if (i > 0)   left_pos[i - 1] = max_index + w1;
				}
			}
			else 
			{
				roi_width = 40;
				//peak값 설정
				
				left_peak[left_count].x = max_index + w1;
				left_peak[left_count++].y = (int)((prev_temp + cur_temp)/2);
				sliding_count++;
				//다음 roi의 센터 영역 설정
				if (i > 0)   left_pos[i - 1] = max_index + w1;
			}
		}
		else 
		{
			if (i > 0)
				left_pos[i - 1] = left_pos[i];
		}
		if (roi_width == 250) 
		{
			no_cnt++;
		}/*
		if (no_cnt > 5) 
		{
			break;
		}*/
		
	}
	//imshow("img", img);
	return left_peak;
}

Mat bird_eyes_view(Mat img) 
{
	int width = img.cols;
	int height = img.rows;
	//이미지의 일부 영역을 제거함(상단 2/5지점까지)
	img = img(Range(height * 2/5, height), Range(0, width)); //k-bub기준
	//img = img(Range(height * 3 / 5, height), Range(0, width)); //digist기준
	width = img.cols;
	height = img.rows;
	Mat warp_matrix;
	Point2f warp_src_point[4];
	Point2f warp_dst_point[4];
	//원본의 좌표(좌하단, 우하단, 좌상단, 우상단)
	warp_src_point[0].x = 0; warp_src_point[0].y = height;
	warp_src_point[1].x = width; warp_src_point[1].y = height;
	warp_src_point[2].x = 0; warp_src_point[2].y = 0;
	warp_src_point[3].x = width; warp_src_point[3].y = 0;

	//목표이미지의 좌표(좌하단, 우하단, 좌상단, 우상단)
	//warp_dst_point[0].x = width * 0.43; warp_dst_point[0].y = height;
	//warp_dst_point[1].x = width * 0.57; warp_dst_point[1].y = height;
	warp_dst_point[0].x = width * 0.36; warp_dst_point[0].y = height;
	warp_dst_point[1].x = width * 0.64; warp_dst_point[1].y = height;
	warp_dst_point[2].x = 0; warp_dst_point[2].y = 0;
	warp_dst_point[3].x = width; warp_dst_point[3].y = 0;
	warp_matrix = cv::getPerspectiveTransform(warp_src_point, warp_dst_point);
	Mat dst;
	cv::warpPerspective(img, dst, warp_matrix, cv::Size(width, height));//버드아이뷰 전환
	return dst;
}

Mat mask_filter(Mat img, int _mask_w, int _mask_h) 
{
	int height = img.rows;
	int width = img.cols;
	Mat img_maskfilter;
	img_maskfilter = Mat::zeros(height, width, CV_8UC1);
	float mask[3];
	int sx = 0;

	// imshow("before mask", img);

	uint* image = (uint*)img.data;
	uchar* score_data = (uchar*)img_maskfilter.data;
	int mask_w = _mask_w, mask_h = _mask_h;



	for (int y = 10; y < height - 10; y++) 
	{
		for (int x = 20; x <= width - 20; x++) 
		{
			for (int i = 0; i < 3; i++) 
			{
				sx = x + (2 * mask_w + 1) * (i - 1);
				mask[i] = image[(y + mask_h) * width + sx + mask_w] - image[(y - mask_h - 1) * width + sx + mask_w] - image[(y + mask_h) * width + sx - mask_w - 1] + image[(y - mask_h - 1) * width + sx - mask_w - 1];
			}

			float sum = ((mask[1] - mask[0]) + (mask[1] + mask[2])) / 2;

			//cout << "sum : " << sum << endl;
			if (sum > 5000) {
				//cout << "sum :" << sum << endl;
				// circle(img_maskfilter, Point2i(x, y), 5, Scalar(255, 0, 0), 3);
				score_data[width * y + x] = 255;
			}
		}
	}

	// imshow("after mask", img_maskfilter);
	return img_maskfilter;
}

// 함수근사(3차식)
vector<double> polyFit(vector<Point> px, int iter, int deg) 
{
	//int X[5];
	vector<double> X(2 * deg + 1);
	for (int i = 0; i < 2 * deg + 1; i++)
	{
		X[i] = 0;
		for (int j = 0; j < iter; j++)
			X[i] = X[i] + pow(px[j].x, i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
	}
	//double B[degree+1][degree+2];
	vector<vector<double>> B(deg + 1, vector<double>(deg + 2));
	vector<double> a(deg + 1);            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
	for (int i = 0; i <= deg; i++)
		for (int j = 0; j <= deg; j++)
			B[i][j] = X[i + j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
	vector<double> Y(deg + 1);
	for (int i = 0; i < deg + 1; i++)
	{
		Y[i] = 0;
		for (int j = 0; j < iter; j++)
			Y[i] = Y[i] + pow(px[j].x, i) * px[j].y;        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
	}
	for (int i = 0; i <= deg; i++)
		B[i][deg + 1] = Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
	int n = deg + 1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations

	for (int i = 0; i < n; i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
		for (int k = i + 1; k < n; k++)
			if (B[i][i] < B[k][i])
				for (int j = 0; j <= n; j++)
				{
					double temp = B[i][j];
					B[i][j] = B[k][j];
					B[k][j] = temp;
				}

	for (int i = 0; i < n - 1; i++)            //loop to perform the gauss elimination
		for (int k = i + 1; k < n; k++)
		{
			double t = B[k][i] / B[i][i];
			for (int j = 0; j <= n; j++)
				B[k][j] = B[k][j] - t * B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
		}
	for (int i = n - 1; i >= 0; i--)                //back-substitution
	{                        //x is an array whose values correspond to the values of x,y,z..
		a[i] = B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
		for (int j = 0; j < n; j++)
			if (j != i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
				a[i] = a[i] - B[i][j] * a[j];
		a[i] = a[i] / B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
	}

	return a;
}
