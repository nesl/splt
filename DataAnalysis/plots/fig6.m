%% Load data files
load hyst.dat;
load power.dat;
load hyst-vic.dat;

%% Put data for Results using Hysteresis in respective arrays
htoken1tv = hyst(:,1);
htoken1lamp = hyst(:,2);
htoken1coffee = hyst(:,3);
htoken1lamp2 = hyst(:,4);

htoken2tv = hyst(:,5);
htoken2lamp = hyst(:,6);
htoken2coffee = hyst(:,7);
htoken2lamp2 = hyst(:,8);

%% Put Vicinity using Hysteresis in respective arrays
htoken1tv_vic = hyst_vic(:,1);
htoken1lamp_vic = hyst_vic(:,2);
htoken1coffee_vic = hyst_vic(:,3);
htoken1lamp2_vic = hyst_vic(:,4);

htoken2tv_vic = hyst_vic(:,5);
htoken2lamp_vic = hyst_vic(:,6);
htoken2coffee_vic = hyst_vic(:,7);
htoken2lamp2_vic = hyst_vic(:,8);

%% Put power consumption data in respective arrays
powertv = power(:,1);
powerlamp = power(:,2);
powercoffee = power(:,3);
powerlamp2 = power(:,4);

%% Compute wasted power values
htoken1tv_wasted = max(htoken1tv - htoken1tv_vic, 0);
htoken1lamp_wasted = max(htoken1lamp - htoken1lamp_vic, 0);
htoken1coffee_wasted = max(htoken1coffee - htoken1coffee_vic, 0);
htoken1lamp2_wasted = max(htoken1lamp2 - htoken1lamp2_vic, 0);

htoken2tv_wasted = max(htoken2tv - htoken2tv_vic, 0);
htoken2lamp_wasted = max(htoken2lamp - htoken2lamp_vic, 0);
htoken2coffee_wasted = max(htoken2coffee - htoken2coffee_vic, 0);
htoken2lamp2_wasted = max(htoken2lamp2 - htoken2lamp2_vic, 0);

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
energy1tv = cumsum(power1tv) / (60 * 60 * 1000);
energy2tv = cumsum(power2tv) / (60 * 60 * 1000);
energy1tot = cumsum(power1tot) / (60 * 60 * 1000);
energy2tot = cumsum(power2tot) / (60 * 60 * 1000);

%% Compute wasted power
power1tot = power1tv + power1lamp + power1coffee + power1lamp2;
power2tot = power2tv + power2lamp + power2coffee + power2lamp2;

power1tot_useful = power1tv .* htoken1tv_vic + power1lamp .* htoken1lamp_vic + power1coffee .* htoken1coffee_vic + power1lamp2 .* htoken1lamp2_vic;
power2tot_useful = power2tv .* htoken2tv_vic + power2lamp .* htoken2lamp_vic + power2coffee .* htoken2coffee_vic + power2lamp2 .* htoken2lamp2_vic;

power1tot_wasted = power1tv .* htoken1tv_wasted + power1lamp .* htoken1lamp_wasted + power1coffee .* htoken1coffee_wasted + power1lamp2 .* htoken1lamp2_wasted;
power2tot_wasted = power2tv .* htoken2tv_wasted + power2lamp .* htoken2lamp_wasted + power2coffee .* htoken2coffee_wasted + power2lamp2 .* htoken2lamp2_wasted;

energy1tot_useful = cumsum(power1tot_useful) / (60 * 60 * 1000);
energy2tot_useful = cumsum(power2tot_useful) / (60 * 60 * 1000);

energy1tot_wasted = cumsum(power1tot_wasted) / (60 * 60 * 1000);
energy2tot_wasted = cumsum(power2tot_wasted) / (60 * 60 * 1000);

%% Plot the results

subplot(2, 2, 1), h=area([power1tot_useful power1tot_wasted]);
grid on;
axis([0 length(power1tv) 0 max(max(power1tot), max(power2tot))*1.1]);
title('(a) Power Consumed by User200');
set(h(1),'FaceColor',[0.27 0.89 0.27])
set(h(2),'FaceColor',[1.0 0.30 0.30])
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Power (W)');

subplot(2, 2, 2), h=area([power2tot_useful power2tot_wasted]);
grid on;
axis([0 length(power1tv) 0 max(max(power1tot), max(power2tot))*1.1]);
title('(b) Power Consumed by User201');
set(h(1),'FaceColor',[0.27 0.89 0.27]);
set(h(2),'FaceColor',[1.0 0.30 0.30]);
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Power (W)');

subplot(2, 2, 3), plot(energy1tot_useful, 'g');
hold on;
plot(energy1tot_wasted, 'r');
axis([0 length(power1tv) 0 max(max(energy1tot), max(energy2tot))*1.1]);
title('(c) Energy Consumed by User200');
grid on;
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Energy (KWh)');
xlabel('Time (secs)');

subplot(2, 2, 4), plot(energy2tot_useful, 'g');
hold on;
plot(energy2tot_wasted, 'r');
axis([0 length(power1tv) 0 max(max(energy1tot), max(energy2tot))*1.1]);
title('(c) Energy Consumed by User201');
grid on;
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Energy (KWh)');
xlabel('Time (secs)');


%{
subplot(2, 2, 3), h=area([energy1tot_useful energy1tot_wasted]);
grid on;
axis([0 length(power1tv) 0 max(max(energy1tot), max(energy2tot))*1.1]);
title('(c) Energy Consumed by User200');
set(h(1),'FaceColor',[1 0.72 0.30])
set(h(2),'FaceColor',[0.86 0.40 0.30])
set(gca,'Layer','top');
%legend('Useful', 'Wasted', 'Location', 'BestOutside');
ylabel('Energy (KWh)');
xlabel('Time (secs)');

subplot(2, 2, 4), h=area([energy2tot_useful energy2tot_wasted]);
grid on;
axis([0 length(power1tv) 0 max(max(energy1tot), max(energy2tot))*1.1]);
title('(d) Energy Consumed by User201');
xlabel('Time (secs)');
set(h(1),'FaceColor',[1 0.72 0.30])
set(h(2),'FaceColor',[0.86 0.40 0.30])
set(gca,'Layer','top');
%legend('Useful', 'Wasted');
ylabel('Energy (KWh)');
%}
