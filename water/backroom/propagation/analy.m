clear all
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
