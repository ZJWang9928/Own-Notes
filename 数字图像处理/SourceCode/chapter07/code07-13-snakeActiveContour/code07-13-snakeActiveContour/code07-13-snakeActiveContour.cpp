/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-13-snakeActiveContour.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月14日
  描  述: 实现活动轮廓模型（Snakes-Active Contour Model），
          先通过鼠标绘制初始轮廓，再迭代演化收敛到目标轮廓。
  其  它:
  参考文献:
    1. Kass et al. Snakes: Active Contour Models
	2. Ritwik Kumar, Harvard University 2010 http://seas.harvard.edu/~rkkumar
	3. gujinjin http://blog.csdn.net/gujinjinseu/article/details/28894023
  函数列表:
    1. Mat cycleShift() 循环平移矩阵的行列数据
    2. Mat_<T> bilinearInterp() 矩阵数据的双线性插值
    3. void onMouseEvent() 鼠标事件回调函数
    4. void snakeImage() 对图像进行Snake主动轮廓模型分割
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/14     1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/// 定义相关数据对象
// 输入图像
Mat srcImg;
// 输出图像
Mat dstImg;
// 图像蒙板（用于获取初始轮廓）
Mat maskImg;
// 工作图像（用于计算图像能量）
Mat workImg;
// 初始轮廓（用于鼠标绘制）
vector<Point> initPoints;
// 目标轮廓（用于曲线演化）
vector<Vec2f> snakePoints;
// 指示初始轮廓是否设定完毕的标志
bool bInitialFinished = false;
// 工作图像窗口名称
const string workWinName = "Contour evolution";

/// Snakes模型参数
struct SnakeParam
{
	SnakeParam(float _alpha, float _beta, float _gamma, float _kappa, float _wl, float _we, float _wt)
	{
		alpha = _alpha;
		beta = _beta;
		gamma = _gamma;
		kappa = _kappa;
		wl = _wl;
		we = _we;
		wt = _wt;
	}

	float alpha;		// 弹性系数
	float beta;			// 刚度系数
	float gamma;		// 步长
	float kappa;		// 图像能量系数
	float wl;			// 直线能量的权重
	float we;			// 边缘能量的权重
	float wt;			// 端点能量的权重
};

/*************************************************
  名    称: Mat cycleShift(Mat m, int xoff, int yoff)
  描    述: 实现矩阵数据循环平移指定的行数和列数
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: snakeImage()
  输入参数: Mat m-->输入矩阵
            int xoff-->水平方向移动的列数（正数表示右移，负数表示左移）
            int yoff-->垂直方向移动的行数（正数表示下移，负数表示上移）
  输出参数: 无
  返    回: 平移后的矩阵
  其    它: 无
  作    者：龙满生
  日    期：2014年8月14日
*************************************************/
Mat cycleShift(Mat m, int xoff, int yoff)
{
	/// 确定输入矩阵与输出矩阵之间的行列对应关系
	int pos;
	vector<int> xIdx, yIdx;
	for(int x = 0; x < m.cols; x++)
	{
		pos = (x + xoff) % m.cols;
		pos = pos >= 0 ? pos : pos + m.cols;
		xIdx.push_back(pos);
	}
	for(int y = 0; y < m.rows; y++)
	{
		pos = (y + yoff) % m.rows;
		pos = pos >= 0 ? pos : pos + m.rows;
		yIdx.push_back(pos);
	}

	/// 按照对应关系拷贝矩阵数据
	Mat n(m.size(), m.type());
	for(int y = 0; y < m.rows; y++)
	{
		for(int x = 0; x < m.cols; x++)
		{
			uchar *p = m.ptr<uchar>(y) + x * m.elemSize();
			uchar *q = n.ptr<uchar>(yIdx[y]) + xIdx[x] * m.elemSize();
			for(int k = 0; k < m.elemSize(); k++)
			{
				q[k] = p[k];
			}
		}
	}

	return n;
}

/*************************************************
  名    称: template<typename T>
            Mat_<T> bilinearInterp(const Mat_<T>& m, const Mat2f& xy)
  描    述: 针对一组指定点，在图像数据中进行双线性插值
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: snakeImage()
  输入参数: const Mat_<T> m-->输入矩阵
            const Mat2f& xy-->待插值的点坐标（列向量，每个元素的类型为Vec2f：x，y）
  输出参数: 无
  返    回: 与插值点相对应的插值结果（列向量，元素类型与输入矩阵元素相同）
  其    它: T --> 类型参数，指定矩阵数据的类型（要求支持+、-、*等运算符）
  作    者：龙满生
  日    期：2014年8月14日
*************************************************/
template<typename T> Mat_<T> bilinearInterp(const Mat_<T>& m, const Mat2f& xy)
{
	Mat_<T> z(xy.size());

	float u, v;
	int r0, r1, c0, c1;
	for(int i = 0; i < xy.rows; i++)
	{
		c0 = cvFloor(xy(i)[0]);
		r0 = cvFloor(xy(i)[1]);
		u = xy(i)[0] - c0;
		v = xy(i)[1] - r0;

		/// 防止行号、列号越界
		c0 = abs(c0 % m.cols);
		r0 = abs(r0 % m.rows);
		c1 = (c0 + 1) % m.cols;
		r1 = (r0 + 1) % m.rows;

		/// 双线性插值
		z(i) = (1 - v) *((1 - u) * m(r0, c0) + u * m(r0, c1)) + v * ((1 - u) * m(r1, c0) + u * m(r1, c1));
	}

	return z;
}

/*************************************************
  名    称: void onMouseEvent(int event, int x, int y, int flags, void *param)
  描    述: 鼠标事件的回调函数，实现鼠标绘制初始轮廓（左键移动绘制，右键闭合轮廓）
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: int event-->鼠标事件类型
            int x-->鼠标水平位置
            int y-->鼠标垂直位置
            int flags-->鼠标事件标志
            void *param-->附加数据
  输出参数: 无
  返    回: 无
  其    它: 
  作    者：龙满生
  日    期：2014年8月14日
*************************************************/
void onMouseEvent(int event, int x, int y, int flags, void *param)
{
	if(maskImg.empty() || bInitialFinished)
		return;

	if(x < 0 || x >= maskImg.cols || y < 0 || y >= maskImg.rows)
		return;

	if(event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{
		/// 左键移动时，存储并绘制轮廓
		 Point pt(x, y);
		if(initPoints.size() == 0)
		{
			initPoints.push_back(pt);
		}
		else
		{
			line(maskImg, initPoints.back(), pt, Scalar(255, 0, 0));
			line(dstImg, initPoints.back(), pt, Scalar(255));
			initPoints.push_back(pt);
			imshow(workWinName, dstImg);
		}
	}
	else if(event == CV_EVENT_RBUTTONDOWN)
	{
		/// 右键闭合初始轮廓
		bInitialFinished = true;

		line(maskImg, initPoints.back(), initPoints.front(), Scalar(255, 0, 0));
		line(dstImg, initPoints.back(), initPoints.front(), Scalar(255));
		imshow(workWinName, dstImg);

		/// 从图像蒙板中获取所绘制的初始轮廓
		vector<vector<Point> > contours;
		vector<Vec4i> hierachy;
		cv::findContours(maskImg, contours, hierachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		
		/// 存储获取的精细轮廓
		snakePoints.clear();
		int ptNum = contours[0].size();
		snakePoints.reserve(ptNum);
		for(int i = 0; i < ptNum; i++)
		{
			Point &pt = contours[0][i];
			snakePoints.push_back(Vec2f(pt.x, pt.y));
		}
	}	
}

/*************************************************
  名    称: void snakeImage(const Mat& image, Mat2f& points, SnakeParam params, int iters)
  描    述: 基于初始轮廓在图像上迭代求解Kass等的Snakes活动轮廓模型
  调用函数: cycleShift()、bilinearInterp()(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: const Mat& image-->输入图像（CV_32F）
            Mat2f& points-->初始轮廓（元素类型为Vec2f）
            SnakeParam params-->Snakes模型参数
            int iters-->最大迭代次数
  输出参数: Mat2f& points-->收敛轮廓（元素类型为Vec2f）
  返    回: 无
  其    它: 
  作    者：龙满生
  日    期：2014年8月14日
*************************************************/
void snakeImage(const Mat& image, Mat2f& points, SnakeParam params, int iters)
{
	if(image.depth() != CV_32F || image.empty())
	{
		cerr << "Invalid image!" << endl;
		return;
	}

	///=========================================================================
	///        计算图像能量：eImg = wl*eLine + we*eEdge + wt*eTerm
	///-------------------------------------------------------------------------
	/// 计算直线能量
	Mat eLine = image.clone();

	/// 计算端点能量
	Mat cX, cY, cXX, cYY, cXY;
	Mat cxKernel = (Mat_<int>(1, 2) << -1, 1);
	Mat cyKernel = cxKernel.t();
	Mat cxxKernel = (Mat_<int>(1, 3) << 1, -2, 1);
	Mat cyyKernel = cxxKernel.t();
	Mat cxyKernel = (Mat_<int>(2, 2) << 1, -1, -1, 1);
	cv::filter2D(image, cX, CV_32F, cxKernel, Point(0, 0), 0, BORDER_CONSTANT);
	cv::filter2D(image, cY, CV_32F, cyKernel, Point(0, 0), 0, BORDER_CONSTANT);
	cv::filter2D(image, cXX, CV_32F, cxxKernel, Point(-1, -1), 0, BORDER_CONSTANT);
	cv::filter2D(image, cYY, CV_32F, cyyKernel, Point(-1, -1), 0, BORDER_CONSTANT);
	cv::filter2D(image, cXY, CV_32F, cxyKernel, Point(0, 0), 0, BORDER_CONSTANT);
	Mat eTerm(image.size(), CV_32F);
	float Ix, Iy, Ixx, Iyy, Ixy;
	for(int r = 0; r < image.rows; r++)
	{
		for(int c = 0; c < image.cols; c++)
		{
			Ix = cX.at<float>(r, c);
			Iy = cY.at<float>(r, c);
			Ixx = cXX.at<float>(r, c);
			Iyy = cYY.at<float>(r, c);
			Ixy = cXY.at<float>(r, c);

			eTerm.at<float>(r, c) = (Iyy * Ix * Ix - 2.0f * Ixy * Ix * Iy + Ixx * Iy * Iy) / pow(1.0f + Ix * Ix + Iy * Iy, 1.5f);
		}
	}
	
	/// 计算边缘能量
	Mat eEdge;
	magnitude(cX, cY, eEdge);
	eEdge *= -1;

	/// 计算图像能量（外部能量 = 图像能量 + 约束能量，此处未考虑约束能量）
	Mat eExt = params.wl * eLine + params.we * eEdge + params.wt * eTerm;

	/// 计算图像能量的梯度（图像力）
	Mat fX, fY;
	cv::Sobel(eExt, fX, CV_32F, 1, 0);
	cv::Sobel(eExt, fY, CV_32F, 0, 1);

	///=========================================================================
	///              计算五对角状矩阵
	///-------------------------------------------------------------------------
	float b[5];
	b[0] = b[4] = params.beta;
	b[1] = b[3] = -(params.alpha + 4 * params.beta);
	b[2] = 2 * params.alpha + 6 * params.beta;

	Mat A = Mat::zeros(points.rows, points.rows, CV_32F);
	Mat I = Mat::eye(points.rows, points.rows, CV_32F);
	A = b[0] * cycleShift(I, 0, 2);
	A = A + b[1] * cycleShift(I, 0, 1);
	A = A + b[2] * I;
	A = A + b[3] * cycleShift(I, 0, -1);
	A = A + b[4] * cycleShift(I, 0, -2);

	/// 计算矩阵的逆
	Mat Ainv;
	cv::invert(A + params.gamma * I, Ainv);

	///=========================================================================
	///              迭代计算并更新曲线
	///-------------------------------------------------------------------------
	/// 分离x坐标和y坐标（均为列向量）
	vector<Mat1f> xy;
	cv::split(points, xy);

	/// 迭代计算
	Mat1f ssx, ssy;
	for(int i = 0; i < iters; i++)
	{
		ssx = params.gamma * xy[0] - params.kappa * bilinearInterp<float>(fX, points);
		ssy = params.gamma * xy[1] - params.kappa * bilinearInterp<float>(fY, points);

		/// 计算snake的新位置
		xy[0] = Ainv * ssx;
		xy[1] = Ainv * ssy;
		cv::merge(xy, points);
		
		/// 显示snake的新位置
		Mat2i contour = points;
		cvtColor(srcImg, dstImg, CV_GRAY2BGR);
		cv::polylines(dstImg, contour, true, Scalar(255,0,0));
		imshow(workWinName, dstImg);
		waitKey(10);
	}
}

int main(int argc, char **argv)
{
	///=========================================================================
	///              打印帮助信息
	///------------------------------------------------------------------------- 
	cout << "分割前请在图像上用鼠标勾画出初始轮廓！\n"
		"\tESC - 退出应用程序\n"
		"\tr - 清除轮廓，恢复原图\n"
		"\tw or SPACE - 迭代计算\n";

	///=========================================================================
	///              加载图像，准备数据
	///-------------------------------------------------------------------------
	/// 加载图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// 输出图像转换为彩色，工作图像转换为浮点型，图像蒙板初始化为0
	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	srcImg.convertTo(workImg, CV_32F);
	maskImg = Mat::zeros(srcImg.size(), CV_8U);

	///=========================================================================
	///              高斯滤波（消除噪声对图像分割的影响）
	///-------------------------------------------------------------------------
	GaussianBlur(workImg, workImg, Size(3, 3), 1, 1);

	///=========================================================================
	///                        设置初始轮廓线
	/// 鼠标左键添加控制点，鼠标右键停止添加点，绘制出闭合曲线
	///-------------------------------------------------------------------------
	namedWindow(workWinName);
	setMouseCallback(workWinName, onMouseEvent);
	imshow(workWinName, dstImg);

	///=========================================================================
	///              设置Snakes模型参数
	///-------------------------------------------------------------------------
	SnakeParam snakeParam(0.3f, 0.2f, 1.0f, 0.1f, 0.2f, 0.4f, 0.1f);
	int maxIter = 1000;		// 最大迭代次数

	///=========================================================================
	///                        处理键盘事件
	///-------------------------------------------------------------------------
	bool bLoop = true;
	while(bLoop)
	{
		int c = waitKey(0);
		switch (c)
		{
		case 27:	// 退出应用程序
			bLoop = false;
			break;
		case 'r':	// 清除轮廓，恢复原图
			initPoints.clear();
			snakePoints.clear();
			bInitialFinished = false;
			maskImg = Scalar::all(0);
			cvtColor(srcImg, dstImg, CV_GRAY2BGR);
			imshow(workWinName, dstImg);
			break;
		case 'w':
		case ' ':	// 迭代求解
			{
				Mat2f points(snakePoints);	// 基于点数组构造点矩阵（不拷贝数据）
				snakeImage(workImg, points, snakeParam, maxIter);	
			}
			break;
		default:
			break;
		}
	}

	return 0;
}
