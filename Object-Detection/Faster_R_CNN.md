# Faster R-CNN

优化了候选框选择算法  
  
RPN接Fast R-CNN   

   
## 具体步骤：
1. 对整张图进行CNN特征提取，得到feature map
2. feature map输入RPN（region proposal network），快速生成候选区域（anchor机制）
3. 通过交替训练，使RPN和Fast-RCNN网络共享参数
4. 应用分类和回归
