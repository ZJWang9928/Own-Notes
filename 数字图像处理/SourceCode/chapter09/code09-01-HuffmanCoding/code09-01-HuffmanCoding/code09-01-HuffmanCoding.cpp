/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code09-01-HuffmanCoding.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年5月3日
  描  述: 实现哈夫曼编码和解码。
  其  它: 
  函数列表:
    1. void computeSymbolFreq();计算单通道图像的灰度出现概率
    2. void selectTwoMinNodes();从符号的当前的概率集合中选择两个最小的概率
    3. void initHuffmanTree();初始化哈夫曼树
    4. void createHuffmanTree();创建哈夫曼树
    5. void makeHuffmanTable();生成哈夫曼码表
    6. void HuffmanEncode();哈夫曼编码
	7. void HuffmanDecode();哈夫曼解码
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/05/03    1.0     创建文件
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <bitset>

using namespace cv;
using namespace std;

// 符号数值－符号概率
typedef std::pair<int, float> SymbolFreqPair;

// 哈夫曼树结点
struct HuffmanNode
{
	float weight;	// 权值：符号出现概率
	int parent;		// 父结点：在结点数组中的索引
	int lchild;		// 左子结点：在结点数组中的索引
	int rchild;		// 右子结点：在结点数组中的索引
};

// 哈夫曼编码信息（解码所需信息）
struct HuffmanCodeInfo
{
	int rows;		// 图像行数
	int cols;		// 图像列数
	int type;		// 数据类型
	float cr;		// 压缩比
	std::map<int, std::string> symbolCodeMap;	// 哈夫曼码表：符号数值 --> 符号码字
};


// 计算单通道图像的灰度出现概率
void computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec);

// 从符号的当前的概率集合中选择两个最小的概率
void selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int nextParentIdx, int &minIdx1, int &minIdx2);

// 初始化哈夫曼树
void initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec);

// 创建哈夫曼树
void createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNodeNum);

// 生成哈夫曼码表
void makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap);

// 哈夫曼编码
void HuffmanEncode(cv::Mat &srcMat, HuffmanCodeInfo &codeInfo, std::vector<unsigned char> &codeStream);

// 哈夫曼解码
void HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream);

int main(int argc, char** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if (srcImg.empty() || srcImg.data==NULL)
	{
		cout << "Failed to load image: " << srcFileName << endl;
		return 1;
	}

	/// 析取图像的指定通道
	cv::Mat srcMat;
	cv::extractChannel(srcImg, srcMat, 0);

	/// 哈夫曼编码
	HuffmanCodeInfo codeInfo;
	std::vector<unsigned char> codeStream;
	HuffmanEncode(srcMat, codeInfo, codeStream);


	/// 哈夫曼解码
	cv::Mat dstMat;
	HuffmanDecode(dstMat, codeInfo, codeStream);

	/// 存储解码图像
	cv::imwrite("decodedImage.bmp", dstMat);
	
	return 0;
}

/*************************************************
  名    称: computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec)
  描    述: 计算单通道图像的灰度出现概率
  调用函数: 
  主调函数: 
  输入参数: cv::Mat &srcMat-->待编码的单通道图像
  输出参数: std::vector<SymbolFreqPair> &symbolFreqVec-->灰度的出现概率（概率为0的灰度除外）
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec)
{
	/// 统计所有可能灰度的出现次数
	std::vector<float> freqVec((0x01 << (srcMat.elemSize1()*8)), 0);
	const uchar *p = NULL;
	for(int r = 0; r < srcMat.rows; r++)
	{
		p = srcMat.ptr<uchar>(r);
		for (int c = 0; c < srcMat.cols; c++)
		{
			freqVec[p[c]]++;
		}
	}

	/// 计算灰度的出现概率（概率为0的灰度除外）
	symbolFreqVec.reserve(freqVec.size());
	unsigned int pixels = srcMat.rows * srcMat.cols;
	for(unsigned int i = 0; i < freqVec.size(); i++)
	{
		if(freqVec[i] != 0)
		{
			symbolFreqVec.push_back(SymbolFreqPair(i, freqVec[i]/pixels));
		}
	}
	symbolFreqVec.shrink_to_fit();
}

/*************************************************
  名    称: selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int upIdx, int &minIdx1, int &minIdx2)
  描    述: 从符号的当前的概率集合中选择两个最小的概率
  调用函数: 
  主调函数: 
  输入参数: std::vector<HuffmanNode> &huffmanTree-->哈夫曼树结点数组
            int nextParentIdx-->两个最小概率结点将要合并形成的新结点在结点数组中的索引
  输出参数: int &minIdx1-->最小概率结点在结点数组中的索引
            int &minIdx2-->次最小概率结点在结点数组中的索引
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int nextParentIdx, int &minIdx1, int &minIdx2)
{
	// 最小的两个概率
	float minFreq1 = 1.0f, minFreq2 = 1.0f;

	/// 从当前概率集合（即正在处理的非子结点集合）中找出最小的两个概率
	minIdx1 = minIdx2 = 0;
	for(int j = 0; j < nextParentIdx; j++)
	{
		if(huffmanTree[j].parent == -1)
		{
			if(huffmanTree[j].weight < minFreq1)
			{
				minFreq2 = minFreq1;
				minIdx2 = minIdx1;
				minFreq1 = huffmanTree[j].weight;
				minIdx1 = j;
			}
			else if(huffmanTree[j].weight < minFreq2)
			{
				minFreq2 = huffmanTree[j].weight;
				minIdx2 = j;
			}
		}
	}
}

/*************************************************
  名    称: initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec)
  描    述: 初始化哈夫曼树
  调用函数: 
  主调函数: 
  输入参数: const std::vector<SymbolFreqPair> &symbolFreqVec-->符号的出现概率
  输出参数: std::vector<HuffmanNode> &nodeVec-->哈夫曼树结点数组
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec)
{
	/// 叶子结点数为n的哈夫曼树具有2n-1个结点
	int leafNum = symbolFreqVec.size();
	int nodeNum = 2 * leafNum - 1;
	nodeVec.resize(nodeNum);

	/// 初始化叶子结点
	for(int i = 0; i < leafNum; i++)
	{
		HuffmanNode &curNode = nodeVec[i];
		curNode.weight = symbolFreqVec[i].second;
		curNode.parent = -1;
		curNode.lchild = -1;
		curNode.rchild = -1;
	}

	/// 初始化非叶子结点
	for(int i = leafNum; i < nodeNum; i++)
	{
		HuffmanNode &curNode = nodeVec[i];
		curNode.weight = 0;
		curNode.parent = -1;
		curNode.lchild = -1;
		curNode.rchild = -1;
	}
}

/*************************************************
  名    称: createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNum)
  描    述: 创建哈夫曼树
  调用函数: selectTwoMinNodes()
  主调函数: 
  输入参数: std::vector<HuffmanNode> &nodeVec-->哈夫曼树结点数组
            int leafNodeNum-->叶子结点数目（即待编码符号数目）
  输出参数: std::vector<HuffmanNode> &nodeVec-->哈夫曼树结点数组
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNodeNum)
{
	int minIdx1, minIdx2;
	for(unsigned int i = leafNodeNum; i < nodeVec.size(); i++)
	{
		/// 选择具有最小概率的两个结点
		selectTwoMinNodes(nodeVec, i, minIdx1, minIdx2);

		/// 合并具有最小概率的两个结点
		nodeVec[minIdx1].parent = i;
		nodeVec[minIdx2].parent = i;
		nodeVec[i].weight = nodeVec[minIdx1].weight + nodeVec[minIdx2].weight;
		nodeVec[i].lchild = minIdx2;
		nodeVec[i].rchild = minIdx1;
	}
}

/*************************************************
  名    称:  makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap)
  描    述: 生成哈夫曼码表
  调用函数: 
  主调函数: 
  输入参数: const std::vector<HuffmanNode> &nodeVec-->哈夫曼树结点数组
            const std::vector<SymbolFreqPair> &symbolFreqVec-->符号--概率数组）
  输出参数: std::map<int, std::string> &symbolCodeMap-->哈夫曼码表：符号--码字
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap)
{
	for(unsigned int i = 0; i < symbolFreqVec.size(); i++)
	{
		// 由叶子结点上溯到根结点，左子结点的码字高位添加0，右子结点的码字高位添加1
		int p = nodeVec[i].parent, c = i;
		std::string szCode;
		while(p != -1)
		{
			if(nodeVec[p].lchild == c)
				szCode = "0" + szCode;
			else
				szCode = "1" + szCode;
			c = p;
			p = nodeVec[c].parent;
		}
		symbolCodeMap[symbolFreqVec[i].first] = szCode;
	}
}

/*************************************************
  名    称: HuffmanEncode(cv::Mat &srcMat, std::vector<unsigned char> &codeStream)
  描    述: 对单通道图像进行哈夫曼编码
  调用函数: computeSymbolFreq()、initHuffmanTree()、createHuffmanTree()
            makeHuffmanTable()
  主调函数: 
  输入参数: cv::Mat &srcMat-->待编码的单通道图像
            HuffmanCodeInfo &codeInfo-->哈夫曼编码信息（用于哈夫曼解码）
  输出参数: std::vector<unsigned char> &codeStream-->编码后的压缩位流
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void HuffmanEncode(cv::Mat &srcMat, HuffmanCodeInfo &codeInfo, std::vector<unsigned char> &codeStream)
{
	/// 计算符号概率
	std::vector<SymbolFreqPair> symbolFreqVec;
	computeSymbolFreq(srcMat, symbolFreqVec);

	/// 初始化哈夫曼树
	std::vector<HuffmanNode> huffmanTree;
	initHuffmanTree(huffmanTree, symbolFreqVec);

	/// 创建哈夫曼树
	createHuffmanTree(huffmanTree, symbolFreqVec.size());

	/// 生成哈夫曼码表
	makeHuffmanTable(huffmanTree, symbolFreqVec, codeInfo.symbolCodeMap);

	/// 计算图像总码长
	unsigned int totCodeLen = 0;
	unsigned int totPixels = srcMat.rows * srcMat.cols;
	for(unsigned int i = 0; i < symbolFreqVec.size(); i++)
	{
		totCodeLen = totCodeLen + (unsigned int)(totPixels * symbolFreqVec[i].second) * codeInfo.symbolCodeMap[symbolFreqVec[i].first].length();
	}

	/// 生成码字字符串
	std::string szCodeStream;
	szCodeStream.reserve(totCodeLen);
	const uchar *p = 0;
	for(int r = 0; r < srcMat.rows; r++)
	{
		p = srcMat.ptr<uchar>(r);
		for(int c = 0; c < srcMat.cols; c++)
		{
			std::string szTemp = codeInfo.symbolCodeMap[p[c]];
			szCodeStream.append(szTemp);
		}
	}

	/// 转换为码字位流
	unsigned int codeBytes = (totCodeLen + 7) / 8;
	codeStream.reserve(codeBytes);
	for(unsigned int i = 0; i < codeBytes; i++)
	{
		codeStream.push_back((unsigned char)(std::strtoul(szCodeStream.substr(i*8, 8).c_str(), NULL, 2)));
	}

	/// 保存编码信息
	codeInfo.rows = srcMat.rows;
	codeInfo.cols = srcMat.cols;
	codeInfo.type = srcMat.type();
	codeInfo.cr = srcMat.rows * srcMat.cols * srcMat.elemSize1() * 8 / (float)totCodeLen;

	/// 输出编码信息
	cout << "压缩率：" << codeInfo.cr << endl;
	cout << "哈夫曼码表：\n符号数值\t符号编码" << endl;
	std::map<int, std::string>::iterator codeIter = codeInfo.symbolCodeMap.begin();
	for(;codeIter != codeInfo.symbolCodeMap.end(); ++codeIter)
	{
		cout << codeIter->first << "\t" << codeIter->second << endl;
	}
}

/*************************************************
  名    称: HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream)
  描    述: 将位流进行哈夫曼解码
  调用函数: 
  主调函数: 
  输入参数: std::vector<unsigned char> &codeStream-->编码后的压缩位流
            HuffmanCodeInfo &codeInfo-->哈夫曼编码信息（用于哈夫曼解码）
  输出参数: cv::Mat &dstMat-->待译码的单通道图像
  返    回: 无
  其    它: 无
  作    者：龙满生
  日    期：2014年5月3日
*************************************************/
void HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream)
{
	/// 反射哈夫曼码表，计算最大码长
	std::map<std::string, int> codeMap;	// 符号编码 -- 符号数值
	unsigned int maxCodeLen = 0; // 最大码长
	std::map<int, std::string>::iterator iter = codeInfo.symbolCodeMap.begin();
	for(; iter != codeInfo.symbolCodeMap.end(); ++iter)
	{
		if(maxCodeLen < iter->second.length())
		{
			maxCodeLen = iter->second.length();
		}

		codeMap[iter->second] = iter->first;
	}

	/// 根据编码信息创建输出图像
	dstMat.create(codeInfo.rows, codeInfo.cols, codeInfo.type);

	/// 基于哈夫曼码表对码流解码
	std::string szCode;	// 码字
	unsigned int decodedSymbols = 0; // 已解码字数目
	unsigned int totPixels = codeInfo.rows * codeInfo.cols; // 图像像素数目
	std::map<std::string, int>::const_iterator codeIter;
	for(unsigned int i = 0; i < codeStream.size(); i++)
	{
		/// 从码流中取出一字节，转换为二进制字符串
		std::string szByte = std::bitset<8>(codeStream[i]).to_string();

		/// 每次从字符串中取出1位组成码字，依据哈夫曼码表译码
		for(int j = 0; j < 8 && decodedSymbols < totPixels; j++)
		{
			/// 从字符串中取出1位组成码字
			szCode.push_back(szByte[j]);

			/// 若在哈夫曼码表中找到对应码字，则输出对应符号值（灰度）到图像
			codeIter = codeMap.find(szCode);
			if(codeIter != codeMap.end())
			{
				dstMat.at<uchar>(decodedSymbols / codeInfo.cols, decodedSymbols % codeInfo.cols) = codeIter->second;
				decodedSymbols++;
				szCode.clear();
			}
			else if(szCode.length() > maxCodeLen)
			{
				cerr << "Incorrect code stream!" << endl;
				exit(1);
			}
		}
	}
}
