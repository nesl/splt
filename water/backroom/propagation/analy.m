clear all
close all
load data_copper_on.txt
load data_pvc_on.txt

plot(data_copper_on(:,2),data_copper_on(:,3),'*')
title('Vibration Propagation Parameter, pipe 1 to pipe 2')
xlabel('Variance of Vibration of Pipe 1')
ylabel('Variance of Vibration of Pipe 2')
figure
plot(data_copper_on(:,2),data_copper_on(:,4),'*')
title('Vibration Propagation Parameter, pipe 1 to pipe 3')
xlabel('Variance of Vibration of Pipe 1')
ylabel('Variance of Vibration of Pipe 3')
figure
plot(data_pvc_on(:,3),data_pvc_on(:,2),'*')
title('Vibration Propagation Parameter, pipe 2 to pipe 1')
xlabel('Variance of Vibration of Pipe 2')
ylabel('Variance of Vibration of Pipe 1')

figure
plot(data_pvc_on(:,3),data_pvc_on(:,4),'*')
title('Vibration Propagation Parameter, pipe 2 to pipe 3')
xlabel('Variance of Vibration of Pipe 2')
ylabel('Variance of Vibration of Pipe 3')

load data_gp.txt

P12 = 0.52;
P21 = 0.15;
P = [1 P12 ; P21 1];

length = 358;

V = zeros(2,length);
V1out = zeros(4, length);
V2out = zeros(4, length);
for i=1:length
    V(:,i) = abs(P\[data_gp(i,2);data_gp(i,3)]);
end

for i=1:length
    V1out(:,i) = [(abs(V(1,i)))^(1/3);(abs(V(1,i)))^(1/2);abs(V(1,i));1];
    V2out(:,i) = [(abs(V(2,i)))^(1/3);(abs(V(2,i)))^(1/2);abs(V(2,i));1];
end


f = [zeros(1,4) zeros(1,4) ones(1,length)]';

Vout = [V1out ; V2out];
A= [ Vout' -eye(length);
     -Vout' -eye(length)];
b= [data_gp(:,1);-data_gp(:,1)];

X=linprog(f,A,b);

 


scale = 0:0.1:9;


for i=1:length
    F1(i) = X(1)*(abs(V(1,i)))^(1/3)+X(2)*(abs(V(1,i)))^(1/2)+X(3)*(abs(V(1,i)))+X(4);
    F2(i) = X(1)*(abs(V(2,i)))^(1/3)+X(2)*(abs(V(2,i)))^(1/2)+X(3)*(abs(V(2,i)))+X(4);
end

plot(data_gp(:,1),F1, '*')
hold on
plot(data_gp(:,1),F2, 'r*')
figure
plot(data_gp(:,1),F2+F1, 'r*')
hold on
plot(data_gp(:,1),data_gp(:,1))
