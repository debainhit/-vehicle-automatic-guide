#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

struct MyStruct
{
	int x1;//最左面
	int x2;//最右面
};

struct MyStruct topbottom;

class MyClass
{
	//成员
public:
	vector<pair<int, int> > fit_point;//点序列  不断填充的
	vector<Point2i> vPoints;//用来拟合的点序列

	Vec4f line1;
	Vec4f line2;

	//用到的变量
	int record;//第一个检测点的位置,在不断的更新
	int diff;//差值
	int wd;//窗口大小10
	int count, i, m, n;//计数点  下一个点的X坐标 i一直在动
	int x = 1;//对应的上一个点的X坐标

	int preverify;



	//函数
public:
	MyClass::MyClass()
	{
		record = 0;//第一个检测点的位置,在不断的更新
		diff = 15;//差值
		wd = 10;//窗口大小10
		count = 0;
		preverify = 500;

	};

	int MyClass::slope(Mat &out, int befor_cols, int after_cols)
	{
		//不会调用它
		//};
		////重载构造函数  
		record = befor_cols;


		//第一条直线
		while (true)
		{
			count = 0;
			i = record + 1; //每次迭代，记录第一个点的位置
			// to find the start point

			while (i < after_cols)
			{
				//std::cout << "列数" << out.rows << "行数" << out.cols << endl;
				m = int(out.at<uchar>(30, i + 1));
				n = int(out.at<uchar>(30, i));
				if (abs(m - n)>diff)
				{
					std::cout << m << "    ^" << n << endl;
					std::cout << "abs(m - n)=" << abs(m - n) << endl;
					record = i - 1;
					x = i;
					//fit_point.push_back(make_pair(255,i));
					break;
				}
				i++;
			}//while

			std::cout << "start" << endl;
			while (count < 20)
			{
				std::cout << "第" << count << "次" << endl;
				int sign = 0;
				i = x - wd;//左窗口,        由上一个时刻i的位置决定
				if (i < 0)
				{
					std::cout << "次数:" << i << "    x + wd:" << x + wd << endl;
					i = 0;
				}
				while (i < x + wd && i<500)
				{//在这个窗口中找到
					int m = int(out.at<uchar>(30 + 20 * count, i + 1));
					int n = int(out.at<uchar>(30 + 20 * count, i));

					if (abs(m - n)>diff)
					{
						std::cout << "行   列" << 30 + 20 * count << "---" << i + 1 << "后一点m = " << m << "前一点n = " << n << endl;
						std::cout << "亮度差值:" << abs(m - n) << endl;
						fit_point.push_back(make_pair(30 + 20 * count, i + 1));
						x = i;
						sign = 1;
						break;
					}//if
					i++;
				}//while 

				//for 用标识符判断如果区间内没有检测到一个点，那就直接退出
				if (sign == 1)
				{
					count++;
				}
				else
				{
					break;
				}
			}//不断的找点知道满足20个要求
			if (fit_point.size() > 13)
			{
				std::cout << "直线拟合点集：" << endl;
				for (vector<pair<int, int> >::iterator i = fit_point.begin(); i != fit_point.end(); ++i)
				{
					std::cout << (i->first) << "   " << i->second << endl;
					vPoints.push_back(Point2i(i->second, (i->first)));
				}
				cv::fitLine(vPoints, line1, CV_DIST_L1, 0, 0.01, 0.01);
				//(like vec4f) - (vx, vy, x0, y0)
				std::cout << "line1: (" << line1[0] << "," << line1[1] << ")(" << line1[2] << "," << line1[3] << ")\n";
				//画一个线段
				float x0 = line1[2];
				float y0 = line1[3];
				float x1 = x0 - 2000 * line1[0];
				float y1 = y0 - 2000 * line1[1];

				//line1(out, cv::Points(line1[2], line1[3]), Points(line1[2] + line1[0] * 50, line1[3] + line1[1] * 50), cv::Scalar(255, 100, 100), 9, 0);
				cv::line(out, Point(x0, y0), cv::Point(x1, y1), cv::Scalar(255, 100, 100), 1, 0);

				//cv::line(out, Point(0, 0), cv::Point(100, 5000), cv::Scalar(255, 100, 100), 1, 0);
				float slope = line1[1] / line1[0];
				float dis = line1[3] - slope*line1[2];
				cout << "第1条直线斜率：" << endl;
				std::cout << "---斜率：" << slope << "---截距：" << dis << "----直线方程：" << endl;
				topbottom.x1 = (511 - dis) / slope+1;
				std::cout << "x1 =" << topbottom.x1 << endl;
				cv::imshow("@@", out);
				break;
			}
			else
			{
				fit_point.clear();//清空了是为了下次准备，防止fit_point已经有点
			}//拟合了第一条直线的直线方程
		}//第一条直线的while

		//第二条直线，过程类似可以优化
		fit_point.clear();
		vPoints.clear();
		record = befor_cols;
		//确定了第一个点的位置后找点
		while (true)
		{
			count = 0;
			i = record + 1; //每次迭代，记录第一个点的位置

			while (i <after_cols)
			{
				//std::cout << "列数" << out.rows << "行数" << out.cols << endl;
				m = int(out.at<uchar>(30, out.cols - i - 1));//前面一列 亮度
				n = int(out.at<uchar>(30, out.cols - i));//后面一列
				if (abs(m - n)>diff)
				{
					//	std::cout << "前面一列列数" << out.cols - i  << "亮度值：" << m << "  后面一列亮度值：" << n << endl;
					//		std::cout << "abs(m - n)=" << abs(m - n) << endl;
					record = out.cols - i;//多往前一行也行
					x = out.cols - i;
					//fit_point.push_back(make_pair(255,i));
					break;
				}
				i++;
			}//while
			//	std::cout << "start" << endl;
			while (count < 20)
			{
				//		std::cout << "第" << count << "次" << endl;
				int sign = 0;
				i = x + wd;//右窗口,   由上一个时刻i的位置决定 
				//	std::cout << "最右边窗口： " << i << endl;
				if (i<0)
				{
					//	std::cout << "窗口开始位置:" << i << " 窗口终止位置 x + wd:" << x + wd << endl;
					i = 0;
				}
				while (i > x - wd && i > 0)
				{//在这个窗口中找到

					m = int(out.at<uchar>(30 + 20 * count, i));
					n = int(out.at<uchar>(30 + 20 * count, i - 1));
					//std::cout << "行数：" << 255 + 20 * count << "列数：" << i - 1 << "前一个点的亮度值：" << m << "后一个点的亮度值：" << n << endl;

					if (abs(n - m) > diff)
					{
						//	std::cout << "行   列" << 255 + 20 * count << "---" <<  i - 1 << "后一点亮度m = " << m << "前一点亮度n = " << n << endl;
						//	std::cout << "亮度差值:" << abs(m - n) << endl;
						fit_point.push_back(make_pair(30 + 20 * count, i));
						x = i;   //i决定了下一个位置的 中间位置
						sign = 1;
						break;
					}//if
					i--;
				}//while 

				//for 用标识符判断如果区间内没有检测到一个点，那就直接退出
				if (sign == 1)
				{
					count++;
				}
				else
				{
					break;
				}
			}
			//存入点集，并在原图上显示
			if (fit_point.size() > 9)
			{
				std::cout << "直线拟合点集：" << endl;

				for (vector<pair<int, int> >::iterator i = fit_point.begin(); i != fit_point.end(); ++i)
				{
					std::cout << (i->first) << "   " << i->second << endl;
					vPoints.push_back(Point2i(i->second, (i->first)));
				}
				fitLine(vPoints, line2, CV_DIST_L1, 0, 0.01, 0.01);
				//(like vec4f) - (vx, vy, x0, y0)
				cout << "line: (" << line2[0] << "," << line2[1] << ")(" << line2[2] << "," << line2[3] << ")\n";

				float x0 = line2[2];
				float y0 = line2[3];
				float x1 = x0 - 3000 * line2[0];
				float y1 = y0 - 3000 * line2[1];

				//line(out, Points(line[2], line[3]), Points(line[2] + line[0] * 50, line[3] + line[1] * 50), cv::scalar(255, 100, 100), 9, 0);
				cv::line(out, Point(x0, y0), cv::Point(x1, y1), cv::Scalar(255, 100, 100), 1, 0);
				float slope = line2[1] / line2[0];
				float dis = line2[3] - slope*line2[2];
				cout << "第2条直线斜率：" << endl;
				cout << "---斜率：" << slope << "---截距：" << dis << "----直线方程：" << endl;
				topbottom.x2 = (0 - dis) / slope + 1;
				std::cout << "x2 =" << topbottom.x2 << endl;
				cv::imshow("@@", out);
				imshow("@@", out);
				break;
			}
			else
			{
				fit_point.clear();
			}

		}//第二条直线的while


		double distance = abs(line1[0] * (line2[3] - line1[3]) - line1[1] * (line2[2] - line1[2])); //注意,vx,vy已经正规化了  
		std::cout << "两条直线之间的距离为: " << distance << std::endl;
		preverify = abs((topbottom.x1 - topbottom.x2)/3);
		return  preverify;

	};//MyClass

};

//这是理论上的不用
int main()
{
	Mat image;
	Mat out;
	int p=0;//存储先验值
	
	for (size_t i = 1; i < 50; i++)
	{
		std::stringstream ss;
		std::string str;
		ss << i;
		ss >> str;
		std::string picpath = "D:\\gragh\\";
		picpath += str;
		picpath += ".bmp";
	
		image = imread(picpath, 0);
		GaussianBlur(image, out, Size(1, 7), 0, 0);
		//前面是读取图像，传参数进去
		MyClass my;
		p = my.slope(out, p, out.cols);
		cout << "先验值" << p << endl;
		cout << picpath << endl;
		cvWaitKey(0);

	}



	
 	return 0;
}


