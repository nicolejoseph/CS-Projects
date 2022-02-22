clc;
close all;
clear;

%% Question 1 
b = [0 2/5 1/4 1/7];
a = [1/3 0 -1/8 3/2];

[z,p,k] = tf2zpk(b,a);
% computed zeroes and poles
% zeroes:  -0.3125 + 0.5094i and -0.3125 - 0.5094i
% poles: -1.7266 + 0.0000i ; 0.8633 + 1.3642i ; and 0.8633 - 1.3642i

figure;
zplane(b, a)
title('Poles and Zeros Plot')   
  
[h,t] = impz(b,a,50);    
figure;
impz(b,a);            
title('Impulse Response')
xlabel('n')
ylabel('Amplitude')

n = 0:50;
x1 = (-4/5).^n;
figure;
subplot(2,1,1);
stem(0: length(x1)-1, x1);
title('Before Filter');
xlabel('n')
ylabel('Amplitude')
xlim([0 7]);

subplot(2,1,2);
y1 = filter(b,a,x1);
stem(0: length(y1)-1, y1);
title('After Filter');
xlabel('n')
ylabel('Amplitude')
xlim([0 7]);           

y2 = conv(x1,h);
figure;
stem(0: length(y2)-1, y2);
title('Convolution');
xlabel('n')
ylabel('Amplitude')
xlim([0 7]);

%% Question 2
zer = [-1; 1];
pol = [0.9*exp(j*(pi/2)); 0.9*exp(-j*(pi/2)); 0.95*exp(j*(5*pi/12)); 0.95*exp(-j*(5*pi/12)); 0.95*exp(j*(7*pi/12)); 0.95*exp(-j*(7*pi/12))]; 
figure;
zplane(zer,pol)
title('Poles and Zeros Plot') 

n = 1024;
[b2,a2] = zp2tf(zer,pol,0.01);
[H,w] = freqz(b2,a2);

Hmg = 20*log10(abs(H));
Hph = rad2deg(unwrap(angle(H)));
figure;
subplot(2,1,1);
plot(w,Hmg);
xlabel('\omega (radians)');
ylabel('Magnitude (dB)');;
xlim([0 pi]);
xticks([0 pi/2 pi]);
xticklabels({'0', '\pi/2', '\pi'});
title('Magnitude');

subplot(2,1,2);
plot(w,Hph);
xlabel('\omega (radians)');
ylabel('Phase (degrees)');
xlim([0 pi]);
xticks([0 pi/2 pi]);
xticklabels({'0', '\pi/2', '\pi'});
title('Phase');
