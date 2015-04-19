# -vehicle-automatic-guide


系统分成仿真和拟合两个部分： 

仿真部分用matlab 进行直线的模拟。 

1）gragh.m 完成小车轨迹的绘制。

2）new.m 完成每个时刻相机对拼缝的拍摄。

拟合部分只要完成偏航角度和位移的测量。

1）thinning.cpp完成对单幅图像的直线方程的模拟。

2）lineslope.cpp用类对thinning.cpp的封装。
