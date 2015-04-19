% 偏航角由自己定义用随机数产生的办法生成（利用图片中模拟出的角度）
% 利用偏航角 给小车偏移量 运动的时间和角度都定了
% 角度的选取使小车左右运动

clc;
close all;
clear;
number = 1000
clc;
b = normrnd(0,1,1,number);
p_s =0 
index = 1
slope_matrix = zeros(2,number);

factor = 0.3
for t=0:.04:(number-1)*0.04;
    p_e = 0.3*sin(factor*t);
    y_offset=normrnd(0,0.01);
    %计算斜率并将斜率保存到slope_matrix中
    slope = (p_e+y_offset-p_s)/0.04;
    %直线方程就有了
    slope_matrix(1,index) = slope;
    slope_matrix(2,index) = b(index);

    figure(1);
    plot([t,t+0.04],[p_s,p_e+y_offset],t,0);
    %box off;
    %axis off;
    
    hold on;
    %将现在的终点点确定为下一个起始点hh
    p_s = p_e+y_offset;
    index = index +1;
end
% figure(1);
% x=0:0.1:2*pi/factor;
% plot(x,0.3*sin(factor*x));
% figure(3);
% hist(slope_matrix(1,:));
%     
    
  
