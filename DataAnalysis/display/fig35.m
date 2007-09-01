%% Load data files
load rssi.dat
load cali.dat
load base.dat
load hyst.dat

%% Put RSSI data in respective arrays
rssi1tv = rssi(:,1);
rssi1lamp = rssi(:,2);
rssi1coffee = rssi(:,3);
rssi1lamp2 = rssi(:,4);

rssi2tv = rssi(:,5);
rssi2lamp = rssi(:,6);
rssi2coffee = rssi(:,7);
rssi2lamp2 = rssi(:,8);

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
subplot(4, 2, 1), plot(rssi1tv);
axis([0 size(rssi1tv, 1) -50 12]);
subplot(4, 2, 2), plot(rssi2tv);
axis([0 size(rssi2tv, 1) -50 12]);

subplot(4, 2, 3), plot(btoken1tv);
axis([0 size(btoken1tv, 1) -1 2]);
subplot(4, 2, 4), plot(btoken2tv);
axis([0 size(btoken2tv, 1) -1 2]);

subplot(4, 2, 5), plot(ctoken1tv);
axis([0 size(ctoken1tv, 1) -1 2]);
subplot(4, 2, 6), plot(ctoken2tv);
axis([0 size(ctoken2tv, 1) -1 2]);

subplot(4, 2, 7), plot(htoken1tv);
axis([0 size(htoken1tv, 1) -1 2]);
subplot(4, 2, 8), plot(htoken2tv);
axis([0 size(htoken2tv, 1) -1 2]);
