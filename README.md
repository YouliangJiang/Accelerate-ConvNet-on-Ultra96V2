# 2021年定制计算冬令营项目 #
## 基于Ultra96-V2平台加速一种卷积神经网络 ##
### 项目成员 ###
姜有亮（Youliang Jiang, youliangj@zju.edu.cn）
### 项目内容 ###
本项目在Ultra96-V2平台上构建了一种卷积神经网络，通过对网络进行量化减少参数量与计算量，利用FPGA并行化、流水化的优势，实现了对卷积神经网络的加速推理，提高了检测的实时性。
### 主要工作 ###
1. 使用Pytorch框架搭建卷积神经网络并进行训练，并按照硬件需求完成网络的参数量化工作，得到最终部署在Ultra96-V2平台上的网络模型。
1. 根据Pytorch生成的网络模型，使用HLS工具进行卷积神经网路的硬件实现，包括卷积、Padding、滑窗、BN、激活、池化等。最终生成Vivado设计所需的IP核文件。
1. 搭建Vivado Block Design，生成Bitstream，将上板所需文件拷贝至Ultra96-V2平台，在Ultra96-V2平台进行调试与测试。
### 板卡型号 ###
Ultra96-V2
### 使用的工具 ###
1. Python 3.7
1. Pytorch 1.7.1
1. Vivado HLS 2018.3
1. Vivado 2020.1

"# Accelerate-ConvNet-on-Ultra96V2" 
"# Accelerate-ConvNet-on-Ultra96V2" 
