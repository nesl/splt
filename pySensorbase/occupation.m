clear all
%load laura.dat
load akhi.dat
laura=akhi;
occupancy = 0;
for i=2:size(laura)
    if laura(i)-laura(i-1) > 10
        if occupancy == 0
            occupancy = 1;
            laura(i,2) = occupancy;
        elseif occupancy == 1
            occupancy = 0;
            laura(i,2) = occupancy;
        end
    end
end


figure
hold on
for i=2:size(laura,1)
    if laura(i,2)-laura(i-1,2) == -1
        fill([laura(i-1,1) laura(i,1) laura(i,1) laura(i-1,1)],[0 0 1 1],'m')
%        fill([laura(28,1) laura(29,1) laura(29,1) laura(28,1)],[0 0 1 1],'r')
    end

end

axis([laura(1,1) laura(size(laura,1),1) 0 2])
title('Akhi')
xlabel('TimeStamp')
ylabel('Occupancy')
grid on