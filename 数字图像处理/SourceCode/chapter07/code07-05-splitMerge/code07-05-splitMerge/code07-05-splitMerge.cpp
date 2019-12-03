/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-05-splitMerge.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月1日
  描  述: 区域分裂与合并,可自定义分裂与合并的一致性准则。
  其  它:
	1. struct QuadTreeNode 四叉树节点结构
	2. class QuadTree 四叉树类
  函数列表:
    1. bool predicate() 图像合并/分裂的一致性准则
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/01    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


// 四叉树节点结构
struct QuadTreeNode
{
	// 子节点列表
	QuadTreeNode* childs[4];

	// 节点矩形区
	Rect rect;

	// 节点有效性（用于标记节点是否已被合并）
	bool active;

	// 节点属性值（如灰度均值、标准差）
	Scalar prop;

	// 构造函数
	QuadTreeNode(const Rect& roi)
	{
		rect = roi;
		active = true;
		prop = Scalar::all(0);

		for(int i = 0; i < 4; i++)
		{
			childs[i] = NULL;
		}
	}


	// 是否为叶子节点
	bool isLeafNode()
	{
		return (!childs[0] && !childs[1] && !childs[2] && !childs[3]);
	}
};

// 四叉树类
class QuadTree
{
public:
	// 构造函数
	QuadTree(const Mat& srcImg);

	// 析构函数
	~QuadTree();

	// 区域分裂
	void split(QuadTreeNode* &node, bool (*pred)(const Mat&));

	// 区域合并（根据一致性准则合并指定结点下的两个子节点）
	bool merge(QuadTreeNode* childA, QuadTreeNode* childB, bool (*pred)(const Mat&));

	// 区域合并（根据一致性准则递归合并指定结点下的子节点）
	void merge(QuadTreeNode* node, bool (*pred)(const Mat&));

	// 网格绘制
	void draw(Mat& dstImg, QuadTreeNode* node);

	// 销毁节点
	void destroy(QuadTreeNode* &node);

	// 四叉树根节点
	QuadTreeNode *root;

	// 图像数据
	Mat img;
};

/**
 * 构造函数
 * @param srcImg 用于构建四叉树的图像（注：要求图像尺寸为2的整数次幂）
 */
QuadTree::QuadTree(const Mat& srcImg): img(srcImg)
{
	// 创建四叉树的根节点，对应的区域为整个图像
	root = new QuadTreeNode(Rect(0, 0, img.cols, img.rows));
}

/**
 * 析构函数
 */
QuadTree::~QuadTree()
{
	// 销毁四叉树
	if(root != NULL)
	{
		destroy(root);
	}
}

/**
 * 按照指定的一致性准则对指定的图像区域进行四叉树分割
 * @param node 需要分割的图像区域对应的四叉树节点（要求该节点不为空）
 * @param pred 用于判定图像区域是否需要分裂的一致性准则（函数指针）
 */
void QuadTree::split(QuadTreeNode* &node, bool (*pred)(const Mat&))
{
	if(node == NULL)
	{
		cerr << "Error: Current node is NULL!" << endl;
		return;
	}
	
	// 如果当前结点对应的区域不满足一致性要求，则将其分裂为4个子区域
	if(pred(img(node->rect)) != true && (node->rect.width > 1 && node->rect.height > 1))
	{
		int x = node->rect.x;
		int y = node->rect.y;
		int w = node->rect.width / 2;
		int h = node->rect.height / 2;

		node->childs[0] = new QuadTreeNode(Rect(x, y, w, h));
		node->childs[1] = new QuadTreeNode(Rect(x+w, y, w, h));
		node->childs[2] = new QuadTreeNode(Rect(x, y+h, w, h));
		node->childs[3] = new QuadTreeNode(Rect(x+w, y+h, w, h));

		///////////////////////////
		// 区域分裂编码索引
		//   0   ｜   1
		// --------------
		//   2   ｜   3
		///////////////////////////
		split(node->childs[0], pred);
		split(node->childs[1], pred);
		split(node->childs[2], pred);
		split(node->childs[3], pred);
	}
}

/**
 * 按照指定的一致性准则合并两个节点
 * @param childA 需要合并的四叉树节点
 * @param childB 需要合并的四叉树节点
 * @param pred 用于判定是否需要合并的一致性准则（函数指针）
 */
bool QuadTree::merge(QuadTreeNode* childA, QuadTreeNode* childB, bool (*pred)(const Mat&))
{
	if(childA->isLeafNode() && childB->isLeafNode())
	{
		Rect rectAB = childA->rect | childB->rect;

		if(pred(img(rectAB)))
		{
			childA->rect = rectAB;
			childB->active = false;

			return true;
		}
	}

	return false;
}

/**
 * 按照指定的一致性准则合并四叉树/子树
 * @param node 需要合并的四叉树/子树的根节点
 * @param pred 用于判定图像区域是否需要合并的一致性准则（函数指针）
 */
void QuadTree::merge(QuadTreeNode* node, bool (*pred)(const Mat&))
{
	if(node->isLeafNode() != true)
	{
		bool bRow1, bRow2, bCol1, bCol2;

		// 先尝试合并行方向的相邻节点区域
		bRow1 = merge(node->childs[0], node->childs[1], pred);
		bRow2 = merge(node->childs[2], node->childs[3], pred);

		// 再尝试合并列方向的相邻节点区域
		if(!(bRow1 || bRow2))
		{
			bCol1 = merge(node->childs[0], node->childs[2], pred);
			bCol2 = merge(node->childs[1], node->childs[3], pred);
		}

		// 递归合并子孙节点
		for(int i = 0; i < 4; i++)
		{
			if(node->childs[i]->isLeafNode() != true)
			{
				merge(node->childs[i], pred);
			}
		}
	}
}

/**
 * 以网格形式在指定图像上绘制指定的四叉子树
 * @param dstImg 用于绘制四叉树网格的图像，须大于等于创建四叉树的源图像
 * @param node 需要绘制的子树的根结点
 */
void QuadTree::draw(Mat& dstImg, QuadTreeNode* node)
{
	if(node != NULL)
	{
		if(node->active && node->isLeafNode())
		{
			rectangle(dstImg, node->rect, 50, 1);
		}
		else
		{
			draw(dstImg, node->childs[0]);
			draw(dstImg, node->childs[1]);
			draw(dstImg, node->childs[2]);
			draw(dstImg, node->childs[3]);
		}
	}
}

/**
 * 销毁指定节点及其子节点（子树）
 * @param node 需要销毁的子树根节点
 */
void QuadTree::destroy(QuadTreeNode* &node)
{
	if(node->isLeafNode())
	{
		delete node;
		node = NULL;
	}
	else
	{
		destroy(node->childs[0]);
		destroy(node->childs[1]);
		destroy(node->childs[2]);
		destroy(node->childs[3]);
	}
}

/**
 * 图像合并/分裂的一致性准则
 * @param src 图像数据
 * @return 若满足一致性要求则返回true，否则返回false
 */
bool predicate(const Mat& src)
{
	Scalar mean, sigma;
	meanStdDev(src, mean, sigma);

	return sigma[0] <= 5;
}


int main(int argc, char** argv)
{
	/// 加载原始图像
	string srcFileName;
	cout << "Enter the source file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// 将图像宽度与高度向下调整到2的整数次幂
	int imgRows = (int)pow(2.0, (int)(log(srcImg.rows) / log(2)));
	int imgCols = (int)pow(2.0, (int)(log(srcImg.cols) / log(2)));
	Mat workImg = srcImg(Rect(0, 0, imgCols, imgRows));

	/// 构建四叉树
	QuadTree qTree(workImg);

	/// 区域分裂
	qTree.split(qTree.root, predicate);

	/// 绘制分裂结果
	cv::Mat splitImg = workImg.clone();
	qTree.draw(splitImg, qTree.root);
	imshow("Split Result", splitImg);

	/// 区域合并
	qTree.merge(qTree.root, predicate);

	/// 绘制合并结果
	cv::Mat mergeImg = workImg.clone();
	qTree.draw(mergeImg, qTree.root);
	imshow("Merge Image", mergeImg);	
	waitKey(0);

	return 0;
}

