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
	double frame_Time, fps, fps_putText;
	frame_Time = 0.0;	// 用于存储相关语句的运行时间 
	fps = 0.0;					// 用于存储视频帧率
	fps_putText = 0.0;	// 用于存储putText文本显示的视频帧率
	int count = 0;			// 计数器
	
	while (1)
	{
		frame_Time = (double)getTickCount();
			// getTickCount():用于获得当前系统的已运行周期数t1
		camera_Video >> frame;
		frame_Time = ((double)getTickCount() - frame_Time) / getTickFrequency();
			// 获得从t被getTickCount()赋值起，直至进行上式运算时系统所经历的时间t5(s)(t3 = (t2 -t1),t5 = t3/t4)
			// 相应语句执行时经过的周期数/每秒内系统运行的周期数 = 相应语句执行经过的时间
			// 即获得二者中间所夹语句执行的时间
			// getTickFrequency():用于获得系统每秒内运行的周期数
		if (frame.empty())
		{
			cout << "Falied to get the frame!" << endl;
			break;
		}

		fps = 1.0 / frame_Time;
			// 视频帧率 = 1s / 每帧图像的获取时间
		if (2 == count++)
		{// 计数器，用于将fps更新放缓后赋值给fps_putText，用于putText()在窗口实时显示帧率
		 // 放缓显示是为了putText()在窗口显示时更新不要过快，便于用户观察
			fps_putText = fps;
			count = 0;
		}

		//在小黑DOS窗口上显示视频图像的宽度、高度和实时帧率
		cout << "Video Width:" << frame.cols		
				<< " Video Height:" << frame.rows
				<< " FPS:" << fps
				<< endl;

		//imshow窗口视频图像+帧率
		String show_FPS_Text = "FPS:" + to_string(fps_putText);
		putText(frame, show_FPS_Text, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
		imshow("Camera", frame);

		if (27 == waitKey(1)) // 如若在此图像显示循环的期间按下ESC键则退出显示循环
			break;					
	}

	camera_Video.release();
	destroyAllWindows();
	return 0;
}
