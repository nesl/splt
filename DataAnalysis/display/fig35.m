%% Load data files
load rssi.dat
load ground.dat
load cali.dat
load base.dat
load hyst.dat
load hyst-vic.dat

%% Put RSSI data in respective arrays
rssi1tv = rssi(:,1);
rssi1lamp = rssi(:,2);
rssi1coffee = rssi(:,3);
rssi1lamp2 = rssi(:,4);

rssi2tv = rssi(:,5);
rssi2lamp = rssi(:,6);
rssi2coffee = rssi(:,7);
rssi2lamp2 = rssi(:,8);

%% Put ground truth data in respective arrays
% Ground truth data is sampled once per 10sec and is aligned at the 33rd
% sample from the RSSI values
temp = ground(:,1);
tempts = timeseries(temp);
tempts = resample(tempts, 0:0.1:length(temp), 'zoh');
gtoken1tv = [zeros(33,1); tempts.Data; zeros(length(rssi1tv)-length(tempts.Data)-33, 1)];

gtoken1lamp = ground(:,2);
gtoken1coffee = ground(:,3);
gtoken1lamp2 = ground(:,4);

temp = ground(:,5);
tempts = timeseries(temp);
tempts = resample(tempts, 0:0.1:length(temp), 'zoh');
gtoken2tv = [zeros(33,1); tempts.Data; zeros(length(rssi2tv)-length(tempts.Data)-33, 1)];

gtoken21lamp = ground(:,6);
gtoken2coffee = ground(:,7);
gtoken2lamp2 = ground(:,8);

%% Put data for Base in respective arrays
btoken1tv = base(:,1);
btoken1lamp = base(:,2);
btoken1coffee = base(:,3);
btoken1lamp2 = base(:,4);

btoken2tv = base(:,5);
btoken21lamp = base(:,6);
btoken2coffee = base(:,7);
btoken2lamp2 = base(:,8);

%% Put data for Calibrated Results in respective arrays
ctoken1tv = cali(:,1);
ctoken1lamp = cali(:,2);
ctoken1coffee = cali(:,3);
ctoken1lamp2 = cali(:,4);

ctoken2tv = cali(:,5);
ctoken21lamp = cali(:,6);
ctoken2coffee = cali(:,7);
ctoken2lamp2 = cali(:,8);

%% Put data for Results using Hysteresis in respective arrays
htoken1tv = hyst(:,1);
htoken1lamp = hyst(:,2);
htoken1coffee = hyst(:,3);
htoken1lamp2 = hyst(:,4);

htoken2tv = hyst(:,5);
htoken21lamp = hyst(:,6);
hoken2coffee = hyst(:,7);
htoken2lamp2 = hyst(:,8);

%% Plot the results
subplot(5, 2, 1), plot(rssi1tv);
axis([0 size(rssi1tv, 1) -50 12]);
title('(a) RSSI at TV from User200');
ylabel('RSSI');
subplot(5, 2, 2), plot(rssi2tv);
axis([0 size(rssi2tv, 1) -50 12]);
title('(b) RSSI at TV from User201');
ylabel('RSSI');

subplot(5, 2, 3), plot(btoken1tv);
axis([0 size(btoken1tv, 1) -0.5 1.5]);
title('(c) Token issued to User200 using Simple Thresholding');
subplot(5, 2, 4), plot(btoken2tv);
axis([0 size(btoken2tv, 1) -0.5 1.5]);
title('(d) Token issued to User201 using Simple Thresholding');

subplot(5, 2, 5), plot(ctoken1tv);
axis([0 size(ctoken1tv, 1) -0.5 1.5]);
title('(e) Calibrated Thresholding');
subplot(5, 2, 6), plot(ctoken2tv);
axis([0 size(ctoken2tv, 1) -0.5 1.5]);
title('(f) Calibrated Thresholding');

subplot(5, 2, 7), plot(htoken1tv);
axis([0 size(htoken1tv, 1) -0.5 1.5]);
title('(g) Calibrated Thresholding with Hysteresis');
subplot(5, 2, 8), plot(htoken2tv);
axis([0 size(htoken2tv, 1) -0.5 1.5]);
title('(h) Calibrated Thresholding with Hysteresis');

subplot(5, 2, 9), plot(gtoken1tv);
axis([0 size(gtoken1tv, 1) -0.5 1.5]);
title('(i) Ground Truth');
xlabel('Time (secs)');
subplot(5, 2, 10), plot(gtoken2tv);
axis([0 size(gtoken2tv, 1) -0.5 1.5]);
title('(j) Ground Truth');
xlabel('Time (secs)');

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
