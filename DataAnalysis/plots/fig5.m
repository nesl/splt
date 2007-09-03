%% Load data files
load rssi.dat;
load hyst.dat;
load power.dat;
load hyst-vic.dat;

%% Put RSSI data in respective arrays
rssi1tv = rssi(:,1);
rssi1lamp = rssi(:,2);
rssi1coffee = rssi(:,3);
rssi1lamp2 = rssi(:,4);

rssi2tv = rssi(:,5);
rssi2lamp = rssi(:,6);
rssi2coffee = rssi(:,7);
rssi2lamp2 = rssi(:,8);

%% Put data for Results using Hysteresis in respective arrays
htoken1tv = hyst(:,1);
htoken1lamp = hyst(:,2);
htoken1coffee = hyst(:,3);
htoken1lamp2 = hyst(:,4);

htoken2tv = hyst(:,5);
htoken21lamp = hyst(:,6);
hoken2coffee = hyst(:,7);
htoken2lamp2 = hyst(:,8);

%% Put Vicinity using Hysteresis in respective arrays
htoken1tv_vic = hyst_vic(:,1);
htoken2tv_vic = hyst_vic(:,5);

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
energy1tv = cumsum(power1tv) / (60 * 60 * 1000);
energy2tv = cumsum(power2tv) / (60 * 60 * 1000);
energy1tot = cumsum(power1tot) / (60 * 60 * 1000);
energy2tot = cumsum(power2tot) / (60 * 60 * 1000);

%% Compute wasted power values
htoken1tv_wasted = max(htoken1tv - htoken1tv_vic, 0);
htoken2tv_wasted = max(htoken2tv - htoken2tv_vic, 0);

%% Plot the results
%{
subplot(5, 2, 1), plot(rssi1tv);
grid on;
axis([0 size(rssi1tv, 1) -50 12]);
title('(a) RSSI at TV from User200');
ylabel('RSSI');
subplot(5, 2, 2), plot(rssi2tv);
grid on;
axis([0 size(rssi2tv, 1) -50 12]);
title('(b) RSSI at TV from User201');
ylabel('RSSI');
%}

subplot(3, 2, 1), plot(htoken1tv_vic);
grid on;
axis([0 size(htoken1tv, 1) -0.5 1.5]);
title('(a) Vicinity of User200 from TV (from RSSI)');
subplot(3, 2, 2), plot(htoken2tv_vic);
grid on;
axis([0 size(htoken2tv, 1) -0.5 1.5]);
title('(b) Vicinity of User201 from TV (from RSSI)');

subplot(3, 2, 3), plot(htoken1tv);
grid on;
axis([0 size(htoken1tv, 1) -0.5 1.5]);
title('(c) Token Issued to User200 by TV');
subplot(3, 2, 4), plot(htoken2tv);
grid on;
axis([0 size(htoken2tv, 1) -0.5 1.5]);
title('(d) Token Issued to User200 by TV');

%{
subplot(5, 2, 5), plot(htoken1tv_wasted);
grid on;
axis([0 size(htoken1tv, 1) -0.5 1.5]);
title('(c) Wasted Token');
subplot(5, 2, 6), plot(htoken1tv_wasted);
grid on;
axis([0 size(htoken2tv, 1) -0.5 1.5]);
title('(d) Token');
%}

subplot(3, 2, 5), h=area([(power1tv .* htoken1tv_vic) (power1tv .* htoken1tv_wasted) ]);
%subplot(5, 2, 7), plot(power1tv);
grid on;
axis([0 length(power1tv) 0 max(max(power1tv), max(power2tv))*1.1]);
title('(e) Power Consumed by User200 for TV');
%set(h(1),'FaceColor',[1 0.6 0.0])
%set(h(2),'FaceColor',[0.86 0.22 0.07])
set(h(1),'FaceColor',[0.27 0.89 0.27]);
set(h(2),'FaceColor',[1.0 0.30 0.30]);
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Power (W)');
xlabel('Time (secs)');


subplot(3, 2, 6), h=area([(power2tv .* htoken2tv_vic) (power2tv .* htoken2tv_wasted) ]);
grid on;
axis([0 length(power1tv) 0 max(max(power1tv), max(power2tv))*1.1]);
title('(f) Power Consumed by User201 for TV');
xlabel('Time (secs)');
%set(h(1),'FaceColor',[1 0.6 0.0])
%set(h(2),'FaceColor',[0.86 0.22 0.07])
set(h(1),'FaceColor',[0.27 0.89 0.27]);
set(h(2),'FaceColor',[1.0 0.30 0.30]);
set(gca,'Layer','top');
legend('Useful', 'Wasted');
ylabel('Power (W)');

%{
subplot(5, 2, 9), plot(energy1tv);
grid on;
axis([0 length(power1tv) 0 max(max(energy1tv), max(energy2tv))*1.1]);
title('(g) Energy Consumed');
xlabel('Time (secs)');
subplot(5, 2, 10), plot(energy2tv);
grid on;
axis([0 length(power1tv) 0 max(max(energy1tv), max(energy2tv))*1.1]);
title('(h) Energy Consumed');
xlabel('Time (secs)');
%}


%% Hysteresis Token vs Vicinity
%{
htoken1tv_vic = hyst_vic(:,1);
htoken2tv_vic = hyst_vic(:,5);

subplot(2, 2, 1), plot(htoken1tv_vic);
axis([0 length(htoken1tv_vic) -0.5 1.5]);
subplot(2, 2, 3), plot(htoken1tv);
axis([0 length(htoken1tv) -0.5 1.5]);

subplot(2, 2, 2), plot(htoken2tv_vic);
axis([0 length(htoken2tv_vic) -0.5 1.5]);
subplot(2, 2, 4), plot(htoken2tv);
axis([0 length(htoken2tv) -0.5 1.5]);
%}
