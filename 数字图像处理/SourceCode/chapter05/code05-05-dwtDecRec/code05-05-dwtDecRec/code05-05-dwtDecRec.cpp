/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code05-05-dwtDecRec.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月3日
  描  述: 实现离散小波分解和重构操作，小波操作层数和小波基函数
          的选择通过滑动条实现。
  其  它:
  函数列表:
    1. void dwt() 小波变换
    2. void idwt() 小波逆变换
    3. void decomposeWavelet() 单行数据小波分解
    4. void reconstructWavelet() 单行数据小波重构
    5. void genWavelet() 生成小波基低通和高通滤波器
    6. void sampleDown() 下采样
    7. void sampleUp() 上采样
    8. static void onTrackbar() 滑动条回调函数
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      耿楠    2014/03/03     1.0     建立该文件
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

#define CV_HAAR 1
#define CV_SYM2 2
#define CV_DB1 3
#define CV_DAUB 4 //Daubechies

// 函数原型
void dwt(Mat src, Mat &dst, int type, int level) ;
void idwt(Mat src, Mat& dst, int type, int level);
void decomposeWavelet(Mat src, Mat &dstLow, Mat &dstHigh, Mat lowFilter, Mat highFilter);
void reconstructWavelet(Mat src, Mat& dst, Mat lowFilter, Mat highFilter);
void genWavelet(int type, Mat &lowFilter, Mat &highFilter);
void sampleDown(Mat src, Mat& dst);
void sampleUp(Mat src, Mat& dst);
// 滑动条回调函数
static void onTrackbar(int pos, void * param);

// 滑动条位置
int Level = 0, WaveletType = 0;

int main(int argc, char **argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, 1);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    Mat dst;

    // 转换为灰度图像
    if (img.channels() >= 3)
        cvtColor(img, img, CV_RGB2GRAY);

    namedWindow("OrigImg", CV_WINDOW_AUTOSIZE);
    imshow("OrigImg",img);

    createTrackbar("Level", "OrigImg", &Level, 5, onTrackbar, &img);
    createTrackbar("Wave", "OrigImg", &WaveletType, 2, 0);
    // 小波变换
    dwt(img, dst, CV_DAUB, 2);
    // 显示变换结果
    imshow("ImgDec", Mat_<uchar>(dst));
    // 逆变换
    idwt(dst, img, CV_DAUB, 2);
    // 显示逆变换结果
    imshow("ImgRec", Mat_<uchar>(img));

    waitKey(0);

    return 0;
}

/*************************************************
  名    称: static void onTrackbar(int pos, void * param)
  描    述: Trackbar回调函数
  调用函数: drawFilledCircle()、shift()、updateMag()
            updateResult()(不包含OpenCV函数与C++函数)
  主调函数: Trackbar滑块滑动事件
  输入参数: int pos-->滑动条当前滑块位置            
  输出参数: void * param-->数据指针，void指针，
                           可以指向任何数据，此处指向图像数据
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
static void onTrackbar(int pos, void * param)
{
    Mat* p_img = (Mat*)param;
    Mat img = *p_img;
    Mat dst;
    // 小波变换
    dwt(img, dst, WaveletType + 1, Level + 1);
    // 显示变换结果
    imshow("ImgDec", Mat_<uchar>(dst));
    // 逆变换
    idwt(dst, img, WaveletType + 1, Level + 1);
    // 显示逆变换结果
    imshow("ImgRec", Mat_<uchar>(img));
}

/*************************************************
  名    称: void dwt(Mat src, Mat &dst, int type, int level)
  描    述: 实现离散小波变换
  调用函数: decomposeWavelet()、genWavelet()(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: Mat src-->输入图像，必须是单通道灰度图像
            int type-->小波基函数，可取CV_HAAR、CV_SYM2、
                       CV_DB1、CV_DAUB
            int level-->小波分解层数
  输出参数: Mat &dst-->小波分解后的数据矩阵。
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void dwt(Mat src, Mat &dst, int type, int level)
{
    // 仅处理单通道灰度图像
    if (src.empty() || src.channels() >= 3)
        return;

    // 转换为浮点数
    src.convertTo(src, CV_32FC1);
    // 创建结果图像，初始化为0
    dst = Mat::zeros(src.rows, src.cols, src.type());
    // 构建低通和高通滤波器
    Mat lowFilter;
    Mat highFilter;
    // 产生滤波器
    genWavelet(type, lowFilter, highFilter);

    int t=1;
    int rows = src.rows;
    int cols = src.cols;
    Mat rowROI; // 单行感兴趣区域
    Mat colROILow, colROIHigh; // 低频和高频单列感兴趣区域
    Mat rowROIdstLow, rowROIdstHigh; // 低频和高频单行感兴趣区域
    Mat colROIdstLow, colROIdstHigh; // 低频和高频单列感兴趣区域

    while ( t<=level )
    {
        // 变换行
        for ( int i=0; i<rows; i++ )
        {
            rowROI = src(Rect(0, i, cols, 1));
            rowROIdstLow = dst(Rect(0, i, cols / 2, 1));
            rowROIdstHigh = dst(Rect(cols / 2, i, cols / 2, 1));
            decomposeWavelet(rowROI, rowROIdstLow, rowROIdstHigh, lowFilter, highFilter);
        }
        // 变换列
        src = dst;
        for (int j = 0; j < cols / 2; j++)
        {
            // 低频部分各列
            colROILow = src(Rect(j, 0, 1, rows));
            colROIdstLow = dst(Rect(j, 0, 1, rows / 2));
            colROIdstHigh = dst(Rect(j, rows / 2, 1, rows / 2));
            Mat colROIdstLowT = colROIdstLow.t();
            Mat colROIdstHighT = colROIdstHigh.t();
            decomposeWavelet(colROILow.t(), colROIdstLowT, colROIdstHighT, lowFilter, highFilter);
            colROIdstLow = colROIdstLowT.t();
            colROIdstHigh = colROIdstHighT.t();

            // 高频部分各列
            colROIHigh = src(Rect(j + cols / 2, 0, 1, rows));
            colROIdstLow = dst(Rect(j + cols / 2, 0, 1, rows / 2));
            colROIdstHigh = dst(Rect(j + cols / 2, rows / 2, 1, rows / 2));
            colROIdstLowT = colROIdstLow.t();
            colROIdstHighT = colROIdstHigh.t();
            decomposeWavelet(colROIHigh.t(), colROIdstLowT, colROIdstHighT, lowFilter, highFilter);
            colROIdstLow = colROIdstLowT.t();
            colROIdstHigh = colROIdstHighT.t();
        }
        // 更新参数
        rows = rows / 2;
        cols = cols / 2;
        t++;
        src = dst;
    }
    //normalize(dst, dst, 0, 255, NORM_MINMAX);
    // 若需要存储该结果为图像文件，请取消该注释
    //imwrite("dwt.png", dst);
}

/*************************************************
  名    称: void idwt(Mat src, Mat &dst, int type, int level)
  描    述: 实现离散小波逆变换
  调用函数: reconstructWavelet()、genWavelet()(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: Mat src-->输入数据，小波分解后的数据矩阵
            int type-->小波基函数，可取CV_HAAR、CV_SYM2、
                       CV_DB1、CV_DAUB
            int level-->小波分解层数
  输出参数: Mat &dst-->小波重构后的图像。
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void idwt(Mat src, Mat & dst, int type, int level)
{
    // 仅处理单通道灰度图像
    if (src.empty() || src.channels() >= 3)
        return;

    // 转换为浮点数
    src.convertTo(src, CV_32FC1);
    int rows = src.rows;
    int cols = src.cols;

    // 生成滤波器
    Mat lowFilter;
    Mat highFilter;
    genWavelet(type, lowFilter, highFilter);

    int t = level;

    // 逆小波变换
    Mat rowROI, colROI, rowROIDst, colROIDst, colROIDstT;
    Rect rowROIRect, colROIRectLeft, colROIRectRight;
    Size sizeCurrent;

    int row = rows / pow(2., level);
    int col = cols / pow(2., level);
    Mat tempDst;
    Mat srcROI;

    while (t >= 1)
    {
        sizeCurrent = Size(2 * col, 2 * row);
        tempDst = Mat::zeros(sizeCurrent, src.type());
        dst = Mat::zeros(sizeCurrent, src.type());

        // 小波列逆变换
        for (int j = 0; j < col; j++)
        {
            // 取出一列，低频部分
            colROIRectLeft = Rect(j, 0, 1, row * 2);
            colROI = src(colROIRectLeft);
            colROIDst = tempDst(colROIRectLeft);
            colROIDstT = colROIDst.t();
            reconstructWavelet(colROI.t(), colROIDstT, lowFilter, highFilter);
            colROIDst = colROIDstT.t();

            //取出一列，高频部分
            colROIRectRight = Rect(j + col, 0, 1, row * 2);
            colROI = src(colROIRectRight);
            colROIDst = tempDst(colROIRectRight);
            colROIDstT = colROIDst.t();
            reconstructWavelet(colROI.t(), colROIDstT, lowFilter, highFilter);
            colROIDst=colROIDstT.t();
        }

        //小波行逆变换
        for (int i = 0; i < 2 * row; i++)
        {
            //一次处理一行
            rowROIRect = Rect(0, i, col * 2, 1);
            rowROI = tempDst(rowROIRect);
            rowROIDst = dst(rowROIRect);
            reconstructWavelet(rowROI, rowROIDst, lowFilter, highFilter);
        }
        // 更新数据
        srcROI = src(Rect(0, 0, 2 * col, row * 2));
        dst.copyTo(srcROI);
        row *= 2;
        col *= 2;
        t--;
    }
    normalize(dst, dst, 0, 255, NORM_MINMAX);
    // 若需要存储该结果为图像文件，请取消该注释
    //imwrite("idwt.png", dst);
}

/*************************************************
  名    称: void decomposeWavelet(Mat src,
                                  Mat &dstLow, Mat &dstHigh,
                                  Mat lowFilter, Mat highFilter)
  描    述: 实现单行数据小波分解
  调用函数: sampleDown()(不包含OpenCV函数与C++函数)
  主调函数: dwt()
  输入参数: Mat src-->输入数据，单行要进行小波分解的数据矩阵
            Mat lowFilter-->低通滤波器，要求与高通滤波器互为正交
            Mat highFilter-->高通滤波器，要求与低通滤波器互为正交
  输出参数: Mat &dstLow-->小波分解后的低频分量
            Mat &dstHigh-->小波分解后的高频分量
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void decomposeWavelet(Mat src, Mat &dstLow, Mat &dstHigh, Mat lowFilter, Mat highFilter)
{
    // 数据有效性检验
    assert(src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1);
    assert(src.cols >= lowFilter.cols && src.cols >= highFilter.cols);

    int len = src.cols;

    // 将数据转换为浮点数
    src.convertTo(src, CV_32FC1);
    lowFilter.convertTo(lowFilter, CV_32FC1);
    highFilter.convertTo(highFilter, CV_32FC1);

    // 准备低通和高通滤波后的矩阵，初始化为0
    Mat LowFiltered = Mat::zeros(1, len, src.type());
    Mat HighFiltered = Mat::zeros(1, len, src.type());

    // 执行滤波操作
    filter2D(src, LowFiltered, -1, lowFilter);
    filter2D(src, HighFiltered, -1, highFilter);

    // 下采样
    sampleDown(LowFiltered, dstLow);
    sampleDown(HighFiltered, dstHigh);
}

/*************************************************
  名    称: void reconstructWavelet(Mat src, Mat &dst,
                                    Mat lowFilter, Mat highFilter)
  描    述: 实现单行数据小波重构
  调用函数: sampleUp()(不包含OpenCV函数与C++函数)
  主调函数: idwt()
  输入参数: Mat src-->输入数据，单行要进行小波重构数据矩阵
            Mat lowFilter-->低通滤波器，要求与高通滤波器互为正交
            Mat highFilter-->高通滤波器，要求与低通滤波器互为正交
  输出参数: Mat &dst-->小波重构后的数据矩阵
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void reconstructWavelet(Mat src, Mat &dst, Mat lowFilter, Mat highFilter)
{
    // 数据有效性检验
    assert(src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1);
    assert(src.cols >= lowFilter.cols && src.cols >= highFilter.cols);

    // 转换为浮点型数据
    src.convertTo(src, CV_32FC1);
    int len = src.cols;
    lowFilter.convertTo(lowFilter, CV_32FC1);
    highFilter.convertTo(highFilter, CV_32FC1);

    // 插值矩阵
    Mat UpLow = Mat::zeros(1, len, src.type());
    Mat UpHigh = Mat::zeros(1, len, src.type());
    // 线性插值
    Mat roiLow(src, Rect(0, 0, len / 2, 1));
    Mat roiHigh(src, Rect(len / 2, 0, len / 2, 1));
    // 上采样
    sampleUp(roiLow, UpLow);
    sampleUp(roiHigh, UpHigh);

    // 低通和高通滤波
    Mat dstLow=Mat::zeros(1, len, src.type());
    Mat dstHigh=Mat::zeros(1, len, src.type());

    filter2D(UpLow, dstLow, -1, lowFilter);
    filter2D(UpHigh, dstHigh, -1, highFilter);

    dst = dstLow + dstHigh;
}

/*************************************************
  名    称: void genWavelet(int type,
                            Mat &lowFilter, Mat &highFilter)
  描    述: 生成用于小波分解和重构的滤波器
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: dwt()、idwt()
  输入参数: int type-->小波基函数，可取CV_HAAR、CV_SYM2、
                        CV_DB1、CV_DAUB
  输出参数: Mat &lowFilter-->低通滤波器，要求与高通滤波器互为正交
            Mat &highFilter-->高通滤波器，要求与低通滤波器互为正交
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void genWavelet(int type, Mat &lowFilter, Mat &highFilter)
{
    if (type == CV_HAAR || type == CV_DB1)
    {
        int N = 2;
        lowFilter = Mat::zeros(1, N, CV_32FC1);
        highFilter = Mat::zeros(1, N, CV_32FC1);

        lowFilter.at<float>(0, 0) = 1 / sqrtf(N);
        lowFilter.at<float>(0, 1) = 1 / sqrtf(N);
        highFilter.at<float>(0, 0) = -1 / sqrtf(N);
        highFilter.at<float>(0, 1) = 1 / sqrtf(N);
    }

    if ( type==CV_SYM2 )
    {
        int N = 4;
        float h[] = {-0.483, 0.836, -0.224, -0.129};
        float l[] = {-0.129, 0.224,  0.836,  0.483};

        lowFilter = Mat::zeros( 1, N, CV_32FC1 );
        highFilter = Mat::zeros( 1, N, CV_32FC1 );

        for (int i = 0; i < N; i++ )
        {
            lowFilter.at<float>(0, i) = l[i];
            highFilter.at<float>(0, i) = h[i];
        }
    }


    if ( type==CV_DAUB )
    {
        int N = 6;
        float l[] = {.332670552950,  .806891509311,
                     .459877502118, -.135011020010,
                     -.085441273882,  .035226291882
                    };
        float h[] = {.035226291882,  .085441273882,
                     -.135011020010, -.459877502118,
                     .806891509311, -.332670552950
                    };

        lowFilter = Mat::zeros( 1, N, CV_32FC1 );
        highFilter = Mat::zeros( 1, N, CV_32FC1 );

        for (int i = 0; i < N; i++ )
        {
            lowFilter.at<float>(0, i) = l[i];
            highFilter.at<float>(0, i) = h[i];
        }
    }
}

/*************************************************
  名    称: void sampleDown(Mat src, Mat& dst)
  描    述: 实现下采样
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: decomposeWavelet()
  输入参数: Mat src-->单行被下采样数据矩阵
  输出参数: Mat& dst-->进行下采样后得到的单行数据矩阵
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void sampleDown(Mat src, Mat& dst)
{
    /// 仅处理1行数据
    if (src.empty() || src.rows != 1 || dst.rows != 1)
        return;

    /// resize函数实现下采样
    resize(src, dst, dst.size());
}

/*************************************************
  名    称: void sampleUp(Mat src, Mat& dst)
  描    述: 实现上采样
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: reconstructWavelet()
  输入参数: Mat src-->单行被上采样数据矩阵
  输出参数: Mat& dst-->进行上采样后得到的单行数据矩阵
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月3日
*************************************************/
void sampleUp(Mat src, Mat& dst)
{
    if (src.empty() || src.rows != 1 || dst.rows != 1 || dst.empty() || 2 * src.cols != dst.cols)
        return;

    /// resize函数实现上采样
    /*
    interpolation method:
    INTER_NEAREST - a nearest-neighbor interpolation
    INTER_LINEAR - a bilinear interpolation (used by default)
    INTER_AREA - resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire’-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method.
    INTER_CUBIC - a bicubic interpolation over 4x4 pixel neighborhood
    INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood
    */
    resize(src, dst, dst.size(), 0, 0, INTER_CUBIC);
}
