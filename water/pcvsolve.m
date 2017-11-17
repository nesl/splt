clear
load pvc.txt
pvc(:,1)=pvc(:,1)*0.004;
V = zeros(4,72);

for i=1:72
    V(:,i) = [pvc(i,2)^(1/3);pvc(i,2)^(1/2);pvc(i,2);1];
    %V(:,i) = [0;pvc(i,2)^(2);pvc(i,2);1];
end
% 
% V=V';
% 
% f = [zeros(1,4) ones(1,72)]';
% A = [V -eye(72);
%     -V -eye(72)];
% b = [pvc(:,1);-pvc(:,1)];
% X=linprog(f,A,b);


V=V';

cvx_begin
    variables x(4);
    Vtemp = V*x;
    minimize(norm(pvc(:,1)-Vtemp,1))
    %x >= 0
cvx_end

X=x

scale = 0:0.1:12;

for i=1:121
    Y(i) = X(1)*scale(i)^(1/3)+X(2)*scale(i)^(1/2)+X(3)*scale(i)+X(4);
end

figure
plot(pvc(:,2),pvc(:,1),'r*')
title('Vibration to Water Flow Rate')
xlabel('Vibration')
ylabel('Water Flow Rate (L/s)')
hold on
grid on
plot(scale,Y,'k')


for i = 1:72
    F(i) = X(1)*pvc(i,2)^(1/3)+X(2)*pvc(i,2)^(1/2)+X(3)*pvc(i,2)+X(4);
end

figure
plot(0:71,pvc(:,1))
hold on
plot(0:71,F,'r')