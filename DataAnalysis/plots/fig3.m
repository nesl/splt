%% Load data files
load hyst.dat
load power.dat

%% Put data for Results using Hysteresis in respective arrays
htoken1tv     = hyst(:,1);
htoken1lamp   = hyst(:,2);
htoken1coffee = hyst(:,3);
htoken1lamp2  = hyst(:,4);

htoken2tv     = hyst(:,5);
htoken2lamp   = hyst(:,6);
htoken2coffee = hyst(:,7);
htoken2lamp2  = hyst(:,8);

%% Put power consumption data in respective arrays
powertv = power(:,1);
powerlamp = power(:,2);
powercoffee = power(:,3);
powerlamp2 = power(:,4);

%% Power consumption split
power1tv     = (powertv .* htoken1tv)         - (powertv .* ((htoken1tv & htoken2tv)/2));
power1tv     = abs(power1tv);
power2tv     = (powertv .* htoken2tv)         - (powertv .* ((htoken1tv & htoken2tv)/2));
power2tv     = abs(power2tv);

power1lamp   = (powerlamp   .* htoken1lamp)   - (powerlamp .* ((htoken1lamp & htoken2lamp)/2));
power1lamp   = abs(power1lamp);
power2lamp   = (powerlamp   .* htoken2lamp)   - (powerlamp .* ((htoken1lamp & htoken2lamp)/2));
power2lamp   = abs(power2lamp);

power1coffee = (powercoffee .* htoken1coffee) - (powercoffee .* ((htoken1coffee & htoken2coffee)/2));
power1coffee = abs(power1coffee);
power2coffee = (powercoffee .* htoken2coffee) - (powercoffee .* ((htoken1coffee & htoken2coffee)/2));
power2coffee = abs(power2coffee);

power1lamp2  = (powerlamp2  .* htoken1lamp2)  - (powerlamp2 .* ((htoken1lamp2 & htoken2lamp2)/2));
power1lamp2  = abs(power1lamp2);
power2lamp2  = (powerlamp2  .* htoken2lamp2)  - (powerlamp2 .* ((htoken1lamp2 & htoken2lamp2)/2));
power2lamp2  = abs(power2lamp2);

power1tot = power1tv + power1coffee + power1lamp + power1lamp2;
power2tot = power2tv + power2coffee + power2lamp + power2lamp2;

% Total Energy is converted to KWh
energy1tot = cumsum(power1tot) / (60 * 60 * 1000);
energy2tot = cumsum(power2tot) / (60 * 60 * 1000);

%% Plot the results
subplot(3, 2, 1), h = area([power1tv power2tv]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(a) TV Power Consumption');
legend('User200', 'User201');
ylabel('Power (W)');

subplot(3, 2, 2), h=area([power1coffee power2coffee]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(b) Coffee Maker Power Consumption');
legend('User200', 'User201');
ylabel('Power (W)');

subplot(3, 2, 3), h=area([power1lamp power2lamp]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(c) Living Room Lamp Power Consumption');
legend('User200', 'User201');
ylabel('Power (W)');

subplot(3, 2, 4), h=area([power1lamp2 power2lamp2]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(d) Bedroom Lamp Power Consumption');
legend('User200', 'User201');
ylabel('Power (W)');

subplot(3, 2, 5), h=area([power1tot power2tot]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(e) Total Power Consumption');
xlabel('Time (secs)');
legend('User200', 'User201');
ylabel('Power (W)');

subplot(3, 2, 6), h=area([energy1tot energy2tot]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
set(h(1),'FaceColor',[0.86 0.22 0.07])
set(h(2),'FaceColor',[1 0.6 0.0])
set(gca,'Layer','top');
title('(f) Total Energy Consumption');
xlabel('Time (secs)');
legend('User200', 'User201');
ylabel('Energy (KWh)');

%{
subplot(3, 2, 6), plot(energy1tot, 'r');
hold on;
plot(energy2tot, 'y')
%axis([0 length(htoken1tv) 0 150]);
grid on;
title('Total Energy Consumption');
%}

%% Plot the results
%{
subplot(2, 1, 1), area([energy1tot energy2tot]);
%axis([0 length(htoken1tv) 0 150]);
grid on;
colormap autumn;
set(gca,'Layer','top');
title('(a) Total Energy Consumption');
xlabel('Time (secs)');
legend('User200', 'User201', 'Location', 'BestOutside');
legend('boxoff');

subplot(2, 1, 2), plot(energy1tot, 'r');
hold on;
plot(energy2tot, 'b')
%axis([0 length(htoken1tv) 0 150]);
grid on;
title('(b) Total Energy Consumption');
xlabel('Time (secs)');
legend('User200', 'User201', 'Location', 'BestOutside');
legend('boxoff');
%}