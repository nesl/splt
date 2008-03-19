clear
load pvc.txt
V = zeros(4,72);

for i=1:72
    V(:,i) = [pvc(i,2)^(1/3);pvc(i,2)^(1/2);pvc(i,2);1];
end

V=V';

f = [zeros(1,4) ones(1,72)]';
A = [V -eye(72);
    -V -eye(72)];
b = [pvc(:,1);-pvc(:,1)];
X=linprog(f,A,b);

scale = 0:0.1:12;

for i=1:121
    Y(i) = X(1)*scale(i)^(1/3)+X(2)*scale(i)^(1/2)+X(3)*scale(i)+X(4);
end

figure
plot(pvc(:,2),pvc(:,1),'r*')
hold on
plot(scale,Y)