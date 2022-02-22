% Nicole Joseph
% ECE 211 Homework 3

close all; 
clear all; 
clc;
%% Question 1
t = -5:0.01:5;
h = 3*(heaviside(t+2)-heaviside(t-1));
x = exp(-2*t).*heaviside(t);
y = conv(h,x,'same').*diff(t(1:2));

hold on;
plot(t,h, 'r');
plot(t,x, 'b');
plot(t,y, 'g');
ylim([-0.5,4]);
xlim([-5,5]);
title('Convolution of h and x')
legend('h','x', 'y=h*x')
hold off;
%% Question 2 
x = struct;
x.data = [2 1 0 -1 3];
x.start = -2;
h = struct;
h.data = [2 1 3];
h.start = 1;

subplot(3,1,1);
stem_plotter(x);
subplot(3,1,2);
stem_plotter(h);
y = d_conv(h,x);
subplot(3,1,3);
stem_plotter(y);

x_len = length(x.data);
h_len = length(h.data);
y_len = length(y.data);
fprintf("Length of x: %d\n",x_len);
fprintf("Support of x: %d\n");
fprintf("[-2,2]\n");
fprintf("Length of h: %d\n",h_len);
fprintf("Support of h: %d\n");
fprintf("[1,3]\n");
fprintf("Length of y: %d\n",y_len);
fprintf("Support of y: %d\n");
fprintf("[-1,5]\n");

function stem_plotter(d_signal)
    d_signal.data = [0 0 d_signal.data 0 0];
    max_ind = size(d_signal.data, 2) + d_signal.start - 1;
    t = [d_signal.start-2:max_ind-2];
    xlim([d_signal.start, max_ind]);
    stem(t, d_signal.data);
end

function y = d_conv(H, X)
    y.start = H.start + X.start;
    y.data = conv(H.data,X.data);
end

%% Question 3
% Comment: based on the plot of y I can confirm that the 
% answer I computed by hand for Question 3 is correct