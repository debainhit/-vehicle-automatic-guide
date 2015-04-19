clc;
clear;
close all;
A =  xlsread('a');%读取excl表格中的数据

for f =1:1:1;
   set (gcf,'Position',[0,0,512,512]);% 改变在当前在桌面上的位置和设定图像大小
    set(gca,'Position',[0,0,1,1]);
    x= 0:0.1:10;
    y = A(1,f)*x ;
   % h = plot(x,y, 'LineWidth',4,'LineSmoothing',  'on');
   h = plot(x,y, 'LineWidth',4);
   
    box off;
    axis([0 1 -1 1]);
    axis off;
    figure(1);
    k = getframe(gcf);
    
    B = imrotate(k.cdata,90,'loose'); 
    gausFilter = fspecial('gaussian',[20 20],3);
    
    C=imfilter(B,gausFilter,'replicate');
    imshow(C);
    I=rgb2gray(C);
    
    I1 = int8(double(I)/255); 
    I3 = I1;
    [m,n] = size(I); 
    I1 = zeros(size(I));
    for i= 1:m
        for j= 1:n
            if(I(i,j)<190)
                I1(i,j) = 1;
            else
                I1(i,j) =0;
            end
        end
    end
        
    gausFilter = fspecial('gaussian',[15 15],3);
    I1=imfilter(I1,gausFilter,'replicate');

%    % I1 = double(int8(I1));
%     I1 = ~I1;
    figure(2);
    set (gcf,'Position',[0,0,512,512]);% 改变在当前在桌面上的位置和设定图像大小
    set(gca,'Position',[0,0,1,1]);
    imshow(I1);
    
    k = size(I1);
    I1 = double(I1);
    m = k(1,1);
    n = k(1,2);
    Add=zeros(m,n);

    for count=1:1:100;
        p = ceil(abs(sqrt(0.3)*(randn(1,2)).*[m,n]));
        %判断4点坐标不超出坐标系 并将在坐标系统的点进行反向处理
        if( p(1)+2<=m && p(1)-2>=1 && p(2)+2<=n && p(2)-2>=1 );
            if( Add(p(1),p(2)) == 0 )
                Add(p(1)-2:p(1)+2,p(2)-2:p(2)+2) = [  0, 1, 1, 1, 0;
                                                     1, 1, 1 ,1, 1;
                                                     1, 1, 1, 1, 1;
                                                     1, 1, 1, 1, 1;
                                                     0, 1, 1, 1, 0 ];
            else
              count = count -1;
            end
         else
            count = count -1;
         end;
    end;
    
    figure(3);
    set (gcf,'Position',[0,0,512,512]);% 改变在当前在桌面上的位置和设定图像大小
    set(gca,'Position',[0,0,1,1]);
    gausFilter = fspecial('gaussian',[20 20],10);
    Add=imfilter(Add,gausFilter,'replicate');
    imshow(Add);
   % 叠加图像的滤波
    F = Add + I1;
    figure(4);
    set(gca,'Position',[0,0,1,1]);
    set (gcf,'Position',[0,0,512,512]);% 改变在当前在桌面上的位置和设定图像大小
    imshow(F);
end;
