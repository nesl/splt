clear
close all
load data1.txt
load data2.txt
load data3.txt
load data4.txt
load data5.txt
load data6.txt
Water = [ 2.6 4.0 6.8 11.4 15.8 29.4];
Water = Water*0.456/35.1572;

%X = [var(data1(10000:11000,1)) var(data2(10000:11000,1)) var(data3(10000:11000,1)) var(data4(11000:20000,1)) var(data5(11000:20000,1)) var(data6(11000:20000,1))];
%X = [var(data1(:,1)) var(data2(:,1)) var(data3(:,1)) var(data4(:,1)) var(data5(:,1)) var(data6(:,1))];
%Y = [var(data1(:,2)) var(data2(:,2)) var(data3(:,2)) var(data4(:,2)) var(data5(:,2)) var(data6(:,2))];
%Z = [var(data1(:,3)) var(data2(:,3)) var(data3(:,3)) var(data4(:,3)) var(data5(:,3)) var(data6(:,3))];

for i=1:20
    X(i,:) = [var(data1(i*1000:(i+1)*1000,1)) var(data2(i*1000:(i+1)*1000,1)) var(data3(i*1000:(i+1)*1000,1)) var(data4(i*1000:(i+1)*1000,1)) var(data5(i*1000:(i+1)*1000,1)) var(data6(i*1000:(i+1)*1000,1))];
    Y(i,:) = [var(data1(i*1000:(i+1)*1000,2)) var(data2(i*1000:(i+1)*1000,2)) var(data3(i*1000:(i+1)*1000,2)) var(data4(i*1000:(i+1)*1000,2)) var(data5(i*1000:(i+1)*1000,2)) var(data6(i*1000:(i+1)*1000,2))];
    Z(i,:) = [var(data1(i*1000:(i+1)*1000,3)) var(data2(i*1000:(i+1)*1000,3)) var(data3(i*1000:(i+1)*1000,3)) var(data4(i*1000:(i+1)*1000,3)) var(data5(i*1000:(i+1)*1000,3)) var(data6(i*1000:(i+1)*1000,3))];
end

plot(Water, mean(X))
hold on
title('Vibration Variance and Water Flow')
xlabel('Liter/second')
ylabel('variance')
legend('X-axis', 'Y-axis', 'Z-axis')
plot(Water, mean(Y),'r')
plot(Water, mean(Z),'m')
grid on
axis([0 0.5 0 170])
FX1 = fft(data1(:,1));
FX2 = fft(data2(:,1));
FX3 = fft(data3(:,1));
FX4 = fft(data4(:,1));
FX5 = fft(data5(:,1));
FX6 = fft(data6(:,1));

figure
title('Frequency Domain Response')
subplot(3,2,1)
plot(abs(FX1(1000:24000)))
xlabel('0.0337 L/S')
axis([0 24000 0 10000])
subplot(3,2,2)
plot(abs(FX2(1000:24000)))
xlabel('0.0519 L/S')
axis([0 24000 0 10000])
subplot(3,2,3)
plot(abs(FX3(1000:24000)))
xlabel('0.0882 L/S')
axis([0 24000 0 10000])
subplot(3,2,4)
plot(abs(FX4(1000:24000)))
xlabel('0.1479 L/S')
axis([0 24000 0 10000])
subplot(3,2,5)
plot(abs(FX5(1000:24000)))
xlabel('0.2049 L/S')
axis([0 24000 0 10000])
subplot(3,2,6)
plot(abs(FX6(1000:24000)))
xlabel('0.3813 L/S')
axis([0 24000 0 10000])