%% Load all the arrays first
load splt-base.txt
%load splt-noradio.txt
load splt-wakeup.txt
%load splt-accel-base-nomove.txt
%load splt-accel-base-move.txt
%load splt-accel-noradio-nomove.txt
%load splt-accel-noradio-move.txt
load splt-accel-wakeup-nomove.txt
load splt-accel-wakeup-move.txt

%% Battery
battery_v = 3.0;
battery_mah = 1500;
% Total Battery Energy in Joules (assuming full capacity is usable)
battery_e = battery_v * battery_mah * 60 * 60 / 1000;

%% Base case
% Keep radio ON always and send beacon every second
tag_v = splt_base(:,1);
tag_i = splt_base(:,2);
tag_p = tag_v .* tag_i;
figure(1)
plot(tag_p);
%tag_e = cumsum(tag_p);
%plot(tag_e);
tag_e_tot = sum(tag_p);
tag_p_avg = tag_e_tot / size(tag_p,1);
tag_lifetime = battery_e / tag_p_avg / (60 * 60 * 24)

%% Wakeup Radio
% Keep radio OFF and turn it ON when required
hold on;
tag_v = splt_wakeup(:,1);
tag_i = splt_wakeup(:,2);
tag_p = tag_v .* tag_i;
plot(tag_p, 'm');
%tag_e = cumsum(tag_p);
%plot(tag_e);
tag_e_tot = sum(tag_p);
tag_p_avg = tag_e_tot / size(tag_p,1);
tag_lifetime = battery_e / tag_p_avg / (60 * 60 * 24)

%% Use Accelerometer to Wakeup Radio
%figure(2)
tag_v = splt_accel_wakeup_move(:,1);
tag_i = splt_accel_wakeup_move(:,2);
tag_p = tag_v .* tag_i;
plot(tag_p,'r');
%tag_e = cumsum(tag_p);
%plot(tag_e);
tag_e_tot = sum(tag_p);
tag_p_avg = tag_e_tot / size(tag_p,1);
tag_lifetime = battery_e / tag_p_avg / (60 * 60 * 24)

%% Use Accelerometer to Wakeup Radio
%figure(2)
tag_v = splt_accel_wakeup_nomove(:,1);
tag_i = splt_accel_wakeup_nomove(:,2);
tag_p = tag_v .* tag_i;
plot(tag_p,'y');
%tag_e = cumsum(tag_p);
%plot(tag_e);
tag_e_tot = sum(tag_p);
tag_p_avg = tag_e_tot / size(tag_p,1);
tag_lifetime = battery_e / tag_p_avg / (60 * 60 * 24)
