clear
close all
load data_pipe1.txt
data_pipe1=data_pipe1(60:size(data_pipe1,1),:);
length = size(data_pipe1,1);
V = zeros(4,length);

for i=1:length
    V(:,i) = [data_pipe1(i,2)^(1/3);data_pipe1(i,2)^(1/2);data_pipe1(i,2);1];
end

V=V';

cvx_begin
    variables x(4);
    Vtemp = V*x;
    minimize(norm(data_pipe1(:,1)-Vtemp,1))
    x >= 0
cvx_end

X=x;
scale = 0:0.1:3;

for i=1:31
    Y(i) = X(1)*scale(i)^(1/3)+X(2)*scale(i)^(1/2)+X(3)*scale(i)+X(4);
end

figure
plot(data_pipe1(:,2),data_pipe1(:,1),'r*')
hold on
plot(scale,Y)


for i = 1:length
    F(i) = X(1)*data_pipe1(i,2)^(1/3)+X(2)*data_pipe1(i,2)^(1/2)+X(3)*data_pipe1(i,2)+X(4);
end

for i = 1:length
    Tsum(i) = sum(data_pipe1(1:i,1));
    Fsum(i) = sum(F(1:i));
end


figure
subplot(2,1,1)
plot(0:(length-1),data_pipe1(:,1))
hold on
grid on
plot(0:(length-1),F,'r')
subplot(2,1,2)
plot(0:(length-1),data_pipe1(:,1)-F','r')
grid on

figure
subplot(2,1,1)
plot(0:(length-1),Tsum)
hold on
grid on
plot(0:(length-1),Fsum,'r')
subplot(2,1,2)
plot(0:(length-1),Tsum-Fsum,'r')
grid on




