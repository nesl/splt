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
load data_lp4.txt

data_gp = data_lp4;%(500:3887,:);
P12 = 0.52;
%P12 = 0.15;
P21 = 0.15;
P = [1 P21 ; P12 1];

length = size(data_gp,1);

V = zeros(2,length);
V1out = zeros(4, length);
V2out = zeros(4, length);
for i=1:length
    V(:,i) = abs(P\[data_gp(i,2);data_gp(i,3)]);
end

for i=1:length
    %V1out(:,i) = [(abs(V(1,i)))^(1/3);(abs(V(1,i)))^(1/2);abs(V(1,i));1];
    %V2out(:,i) = [(abs(V(2,i)))^(1/3);(abs(V(2,i)))^(1/2);abs(V(2,i));1];
    V1out(:,i) = [((V(1,i)))^(1/3);((V(1,i)))^(1/2);(V(1,i));1];
    V2out(:,i) = [((V(2,i)))^(1/3);((V(2,i)))^(1/2);(V(2,i));1];

end


% f = [zeros(1,4) zeros(1,4) ones(1,length)]';
% 
Vout = [V1out ; V2out];
% A= [ Vout' -eye(length);
%      -Vout' -eye(length)];
% b= [data_gp(:,1);-data_gp(:,1)];
% 
% X=linprog(f,A,b);
    


% cvx_begin
%     variables x(8);
%     Vtemp = Vout'*x;
% %     windowSize = 5;
% %     tempgp = filter(ones(1,windowSize)/windowSize,1,data_gp(:,1))/5;
% %     tempVout =  filter(ones(1,windowSize)/windowSize,1,Vout)/5;
%     
%     for i = 1:length
%        temp(i) = sum(data_gp(1:i,1))-sum(Vtemp(1:i));
%     end
%     minimize(norm(temp,Inf))
% %     minimize(norm(tempgp-tempVout'*x,Inf))
%     %x >= 0
% cvx_end
%  


cvx_begin
    variables x(8);
    Vtemp = Vout'*x;
    minimize(norm(data_gp(:,1)-Vtemp,Inf))
    x >= 0
cvx_end

X=x

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

figure
plot((1:length)*0.5, data_gp(:,1))
hold on
plot((1:length)*0.5, F2+F1, 'r*')

for i=1:length
    F1sum(i) = sum(F1(1:i));
    F2sum(i) = sum(F2(1:i));
    data_gp_sum(i) = sum(data_gp(1:i,1));
end

figure
plot((1:length)*0.5, data_gp_sum(:))
hold on
plot((1:length)*0.5, F2sum+F1sum, 'r.')
plot((1:length)*0.5, F1sum, 'y.')
plot((1:length)*0.5, F2sum, 'm.')

figure 
plot((1:length)*0.5, F1, 'y.')
hold on
plot((1:length)*0.5, F2, 'm.')
plot((1:length)*0.5, F2+F1, 'r.')
plot((1:length)*0.5, data_gp(:,1), '.')


windowSize = 30;
F1filtered = filter(ones(1,windowSize)/windowSize,1,F1);
F2filtered = filter(ones(1,windowSize)/windowSize,1,F2);
data_gp_filtered = filter(ones(1,windowSize)/windowSize,1,data_gp);

figure 
plot((1:length)*0.5, F1filtered, 'y')
hold on
plot((1:length)*0.5, F2filtered, 'm')
plot((1:length)*0.5, (F2filtered+F1filtered), 'r')
plot((1:length)*0.5, data_gp_filtered(:,1))

% normalized

for i = 1:length
    F1normalized(i) = F1filtered(i)/(F1filtered(i)+F2filtered(i))*data_gp(i,1);
    F2normalized(i) = F2filtered(i)/(F1filtered(i)+F2filtered(i))*data_gp(i,1);
end
figure
plot((1:length)*0.5, F1normalized, 'y')
hold on
plot((1:length)*0.5, F2normalized, 'm')
plot((1:length)*0.5, (F2normalized+F1normalized), 'r')
plot((1:length)*0.5, data_gp(:,1))


for i=1:length
    F1normalizedsum(i) = sum(F1normalized(1:i));
    F2normalizedsum(i) = sum(F2normalized(1:i));
    data_gp_sum(i) = sum(data_gp(1:i,1));
end

figure
plot((1:length)*0.5, data_gp_sum(:))
hold on
plot((1:length)*0.5, F2normalizedsum+F1normalizedsum, 'r.')
plot((1:length)*0.5, F1normalizedsum, 'y.')
plot((1:length)*0.5, F2normalizedsum, 'm.')

