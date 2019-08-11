# SSP-Net

## R-CNN存在的问题：
+ 通过对图像的裁剪crop或缩放warp，使得输入图片的信息缺失或变形，降低了图片识别的准确率  
+ 对每个RP进行卷积计算，算力过大  
  
  
SPP-Net是对rcnn的改进，spatial Pyramid Pooling    
  
  
## 主要观点：
1. 共用特征卷积图
2. 空间金字塔化，有效解决了不同尺度的图片在全连接层输出不一致的问题  

## 网络结构对比：
### R-CNN:
image --> crop/warp --> conv layers --> fc layers --> output   
### SPP-Net
image --> conv layers --> spatial pyramid pooling --> fc layers --> output    

## SPP-Net优势：
只对原图进行卷积处理，引入SPP池化进行特征图候选框的维度统一    
 
