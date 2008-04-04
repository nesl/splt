clear
load copper3.txt
V = zeros(4,63);

for i=1:63
    V(:,i) = [copper3(i,2)^(1/3);copper3(i,2)^(1/2);copper3(i,2);1];
end

V=V';

f = [zeros(1,4) ones(1,63)]';
A = [V -eye(63);
    -V -eye(63)];
b = [copper3(:,1);-copper3(:,1)];
X=linprog(f,A,b);

cvx_begin
    variables x(4);
    minimize(norm(((copper3(:,1)))-((V*x)),1))
    %x >= 0
cvx_end

%X=x;
scale = 0:0.1:9;

for i=1:91
    Y(i) = X(1)*scale(i)^(1/3)+X(2)*scale(i)^(1/2)+X(3)*scale(i)+X(4);
end

figure
plot(copper3(:,2),copper3(:,1),'r*')
hold on
plot(scale,Y)