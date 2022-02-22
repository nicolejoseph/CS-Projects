% Nicole Joseph
% ECE 211 Homework 3

close all; 
clear all; 
clc;

%% Question 3
fp = [12e3, 15e3];
fs = [10e3, 16e3];
rp = 1.5;
rs = 30;
[n,wn] = ellipord(2*pi*fp, 2*pi*fs, rp, rs, 's');
[z,p,k] = ellip(n,rp,rs,wn,'s');
[b,a] = zp2tf(z,p,k);

% Poles and Zeros Plot
figure;
zplane(z, p)
title('Poles and Zeros Plot for Analog Case') 
% the value n returned by ellipord is 4
% there are 8 poles shown on the Poles and Zeros Plot
% this checks that 2n is the filter order

% Frequency Response Graph
w = linspace(0,20e3,1000); 
w = 2*pi*w; 
H = freqs(b,a,w);
Hdb = 20*log10(abs(H));
Hph = unwrap(angle(H)*180/pi);

% Magnitude Response
figure
subplot(2,1,1)
plot(w/(2*pi),Hdb)
title('Magnitude Response')
xlabel('Frequency in Hertz')
ylabel('Magnitude (dB)')
axis([0 2e4 -60 1])

yline(-30,'--','Stopband')
yline(0,'--','Passband')
yline(-1.5,'--', 'Passband')

% Phase Response
subplot(2,1,2)
plot(w/(2*pi),Hph)
axis([0 2e4 -400 0])
title('Phase Response')
xlabel('Frequency in Hertz')
ylabel('Unwrapped Phase (degrees)')

%% Question 4
fp = [12e3,15e3];
fs = [10e3,16e3];
rp = 1.5;
rs =30;
fsamp = 40e3;
[nd,wnd] = ellipord(fp/(fsamp/2), fs/(fsamp/2), rp, rs);
[zd,pd,kd] = ellip(nd, rp, rs, wnd);
[bd,ad] = zp2tf(zd, pd, kd);

% 2nd is 6 and is less than 2n, which is 8

% Poles and Zeros Plot
figure;
zplane(zd,pd);
title('Poles and Zeros Plot for Digital Case')

% Frequency Response Graph
w2 = linspace(0,20e3,1000);
wd = 2*pi*w2;
Hz = freqz(bd,ad,wd);
Hdb2 = 20*log10(abs(Hz));
hph2 = unwrap(angle(Hz)*180/pi);

figure;                            
subplot(2,1,1);
plot(w2,Hdb2);
title('Magnitude Response (Digital Filter)')
xlabel('Hertz')
ylabel('Magnitude(dB)')
axis([-2e3 22e3 -400 50]) 
                  
subplot(2,1,2);
plot(w2,hph2);
title('Phase Response (Digital Filter)');
xlabel('Hertz');
ylabel('Phase (degrees)');
axis([-2e3 22e3 -200 100])
