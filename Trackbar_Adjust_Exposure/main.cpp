#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;
static void trackbar_CallBack(int, void*);

int exposure_Trackbar_Value = 12;							// 后续用作滑动条的当前值	
int exposure_Trackbar_Max_Value = 12;					// 后续用作滑动条的最大值，12将对应CaptureExposure值的最大值
																					// 由于滑动条对应值为0~MaxValue共MaxValue+1个值
																					// 而恰好后续要对应CaptureExposure13个值，所以这里MaxValue取12即可得13个值
int value_CaptureExposure = 0;								// 存储CaptureExposure曝光时间对照表中的CaptureExposure值
double value_CaptureExposure_Time_ms = 640.0;	// 对应exposure_Trackbar_Value，用于存储当前摄像头曝光时间
																					//此时曝光时间对应exposure_Trackbar_Max_Value，为640.0ms

int main(int argc, char** argv)
{
	VideoCapture camera_Video;
	camera_Video.open(0);
	if (!camera_Video.isOpened())
	{
		cout << "Falied to open the camera!Please check whether the camera is broken." << endl;
		return -1;
	}

	namedWindow("Camera", WINDOW_AUTOSIZE);	// 创建一个自适应图像大小的窗口
	createTrackbar("Exposure", 
							 "Camera", 
							 &exposure_Trackbar_Value,
							 exposure_Trackbar_Max_Value,
							 trackbar_CallBack, 
							 (void*)(&camera_Video)
							);
		/* creatTrackbar()函数执行tips:
		*	@ param1: 滑动条的名称
		*	@ param2: 滑动条要粘附的窗口的名称
		*	@ param3: 滑动条拖动时变化的值或者说是所修改的参数的地址
		*		 当我们在窗口拖动时就会修改这个地址所对应的参数中的值
		*		 同时该地址所对应的参数的值就是滑动条初加载时的初值
		*	@ param4: 拖动滑动条被拖动发生变化时所要执行的函数的指针/地址
		*		 当滑动条被拖动时就会触发该函数的第4个参数，去执行该参数（地址）所对应的函数
		*		 拖动变化发生则在一个时间周期后就会执行该函数一次，即不是拖动变化一个值就执行一次
		*			其执行需要一个检测周期的时间，只有在这个检测周期内发生拖动了滑动条才会执行对应的函数一次
		*		 所对应的函数称为回调函数callBack(int, void*)，其函数原型为void callBack(int, void*)
		*	@ param5: 要传入回调函数callBack(int void*)中void*位置的变量的指针/地址
		*		 要传入的变量value必须取其地址(&value)，并且转化为空指针类型(void*(&value))
		*		 在callBack函数中使用该变量时，根据需要将其转化为原主调函数中其对应的数据类型的指针，或其他类型指针
		*	@ !!!Attention!!!:	1.createTrackbar()在开始执行时就已经调用了其回调函数callBack()1次
		*								2.createTrackbar()函数执行一次就会创建滑动条一次，所以该函数不可以放入死循环中
		*/

	Mat frame;
	String show_Exposure_Time_Text;// 存储要显示到窗口上的putText文本
	while (1)
	{
		camera_Video >> frame;
		if (frame.empty())
		{
			cout << "Falied to get the frame!" << endl;
			break; 
		}
		show_Exposure_Time_Text = "Exposure Time:" + to_string(value_CaptureExposure_Time_ms) + "ms";
		// 该语句放在循环中，用于实时改变该Text中的value_CaptureExposure_Time_ms内容
		//cout << show_Exposure_Time_Text << endl;

		putText(frame, show_Exposure_Time_Text, Point(5, 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255,255,255));
		imshow("Camera", frame);
		cout << "CaptureExposure Value:" << value_CaptureExposure
				<< "	Exposure Time:" << value_CaptureExposure_Time_ms << "ms"
				<< endl;

		if (27 == waitKey(1))
			break;
	}

	camera_Video.release();
	destroyAllWindows();
	waitKey(0);
	return 0;
}

static void trackbar_CallBack(int, void* camera_Video_addr)
{
	value_CaptureExposure = (exposure_Trackbar_Value - 13);
	// 转化exposure_Trackbar_Value为对应的CaptureExposure值（查表：CaptureExposure曝光时间对照表.png）
	//cout << value_CaptureExposure << endl;

	((VideoCapture*)camera_Video_addr)->set(CAP_PROP_EXPOSURE, value_CaptureExposure);
	// 将主调函数中传来的camera_Video对象（摄像头）的空类型指针/地址
		// 转化为该变量在主调函数中的所对应数据类型的指针/地址
	// 并通过结构体指针/类指针的形式，调用该地址所对应的对象中的set()方法以设置摄像头的曝光度
	
	switch ((int)(((VideoCapture*)camera_Video_addr)->get(CAP_PROP_EXPOSURE)))
	{// get()获取当前摄像头的CaptureExposure值，并强转为int类型，将该值通过case转化为对应的曝光时间
	 // 即转化为对应的value_CaptureExposure_Time_ms，之后用于main()的while()中的putText()显示
	case -13:
		value_CaptureExposure_Time_ms = 0.150;
		break;
	case -12:
		value_CaptureExposure_Time_ms = 0.312;
		break;
	case -11:
		value_CaptureExposure_Time_ms = 0.650;
		break;
	case -10:
		value_CaptureExposure_Time_ms = 1.250;
		break;
	case -9:
		value_CaptureExposure_Time_ms = 2.500;
		break;
	case -8:
		value_CaptureExposure_Time_ms = 5.000;
		break;
	case -7:
		value_CaptureExposure_Time_ms = 10.000;
		break;
	case -6:
		value_CaptureExposure_Time_ms = 20.000;
		break;
	case -5:
		value_CaptureExposure_Time_ms = 40.000;
		break;
	case -4:
		value_CaptureExposure_Time_ms = 80.000;
		break;
	case -3:
		value_CaptureExposure_Time_ms = 160.000;
		break;
	case -2:
		value_CaptureExposure_Time_ms = 320.000;
		break;
	case -1:
		value_CaptureExposure_Time_ms = 640.000;
		break;
	default:// switch(value_CaptureExposure)匹配失败时的处理
		cout << "The CaptureExposure Value Switch Failed to match! value_CaptureExposure ="
			<< value_CaptureExposure << endl;
		break;
	}
	//cout << value_CaptureExposure_Time_ms << endl;
}
