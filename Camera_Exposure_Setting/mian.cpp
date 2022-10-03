#include<opencv2/opencv.hpp>
#include<iostream>
#include<time.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture camera_Video;
	camera_Video.open(0);
	if (!camera_Video.isOpened())
	{
		cout << "Falied to open the camera!Please check whether the camera is broken." << endl;
		return -1;
	}

	Mat frame;
	double frame_Time = 0.0;	// 用于存储相关语句的运行时间 
	double fps = 0.0;					// 用于存储视频帧率
	double fps_putText = 0.0;	// 用于存储putText文本显示的视频帧率
	int cameraExposure_Value = 0;			// 用于设置摄像头曝光值
	int cameraExposure_Value_Flag = 0;	// 设置与摄像头曝光值对应的标志值
	int cameraExposure_Time_ms = 0;		//	保存摄像头的曝光值所对应的曝光时间	
	int count1 = 100, count2 = 0;			// 计数器

	while (1)
	{
		if (100 == count1++)
		{// 此处设置计数目的在于达到计数次数后调整摄像头曝光值，即相当于计时自动调节
		 //	 若修改此处的判断条件次数，则相应要修改count1定义声明时所赋的初值
		 // 以保证程序开始执行时就能进入该判断语句执行
			switch (cameraExposure_Value_Flag)
			{
			case 0:
				cameraExposure_Value = -7;	// 对应10ms曝光
				cameraExposure_Time_ms = 10;
				break;
			case 1:
				cameraExposure_Value = -5;	// 对应40ms曝光
				cameraExposure_Time_ms = 40;
				break;
			case 2:
				cameraExposure_Value = -4;	// 对应80ms曝光
				cameraExposure_Time_ms = 80;
				break;
			default:	// 匹配失败的相应处理
				cout << "The Camera Exposure Value Flag switch Failed to match! Value Flag="
						<< cameraExposure_Value_Flag << endl;
				break;
			}
			camera_Video.set(CAP_PROP_EXPOSURE, cameraExposure_Value);
			count1 = 0;
			(++cameraExposure_Value_Flag) %= 3;	//循环曝光标志值
		}

		//获取视频单帧时长
		frame_Time = (double)getTickCount();
		camera_Video >> frame;
		frame_Time = ((double)getTickCount() - frame_Time) / getTickFrequency();
		if (frame.empty())
		{
			cout << "Falied to get the frame!" << endl;
			break;
		}
		//获取帧率
		fps = 1.0 / frame_Time;
		if (2 == count2++)
		{
			fps_putText = fps;
			count2 = 0;
		}
		//DOS窗口上显示
		cout << "Video Width:" << frame.cols
			<< "	Video Height:" << frame.rows
			<< " FPS:" << fps
			<< "	Exposure Time:" << cameraExposure_Time_ms	// 显示当前摄像头曝光时间
			<< endl;

		//imshow窗口
		String show_FPS_Text = "FPS:" + to_string(fps_putText);
		String show_Exposure_Time_Text = "Exposure Time:" + to_string(cameraExposure_Time_ms);
		putText(frame, show_FPS_Text, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
		putText(frame, show_Exposure_Time_Text, Point(5, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
		imshow("Camera", frame);

		if (27 == waitKey(1)) // 如若在此图像显示循环的期间按下ESC键则退出显示循环
			break;
	}

	camera_Video.release();
	destroyAllWindows();
	return 0;
}
