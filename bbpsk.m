%% Code that uses low autocorrelation binary sequences to modulate a signal using binary phase shift keying (BPSK)

clc, clear all, close all;

data = [0 1 0 0 1 1 1];
pData = 2*data - 1;
data2 = [0 0 0 0 0 0 0];
pData2 = 2*data2 - 1;

Define carrier
f = 1000;
fs = f*10;
T = 1/f;
Ts = 1/fs;
M = 1;
n = M*length(data);
t = 0:Ts:n*T;
car = cos(2*pi*f*t);
car(end) = [];
subplot(2,1,1);
stem(pData);
subplot(2,1,2);
plot(car);

Convert impulse data to pulse data
tp = 0:Ts:T*M;
exdata = [];
exdata2 = [];
for i=1: length(data)
    for j=1: length(tp)-1
        exdata = [exdata pData(i)];
        exdata2 = [exdata2 pData2(i)];
    end
end
figure ;
plot(exdata, 'r-', 'LineWidth', 4 );
hold on;
plot(exdata2, 'b-', 'LineWidth', 4);
hold on;
plot(car, 'g-');
grid on;
hold off;

Modulation
mSig = exdata.*car;
mSig2 = exdata2.*car;
figure;
plot(mSig);
hold on;
plot(mSig2);
hold off;

Channel
SNR = 15;
seed = 12345;
rng(seed);
rx = awgn(mSig, SNR);
rng(seed);
rx2 = awgn(mSig2, SNR);
figure;
plot(rx, 'g-', 'LineWidth', 1);
hold on;
plot(rx2, 'r-', 'LineWidth',1)
grid on;
hold off;

r = abs(xcorr(rx, rx));
r2 = abs(xcorr(rx2, rx2));
figure;

subplot(1,2,1);
plot(r);
h = refline(0,15);
h.Color = 'k';
legend('suboptimal', 'Threshold level');

subplot(1,2,2);
plot(r2, 'r-');
h1 = refline(0,15);
h1.Color = 'k';
legend('optimal', 'Threshold level');
hold off;

subplot(1,2,1)
legend("Position",[0.12536,0.92577,0.29861,0.069444])
xlabel("Frequency")
ylabel("Autocorrelation")
subplot(1,2,2)
legend("Position",[0.6024,0.93749,0.24653,0.069444])
xlabel("Frequency")
ylabel("Autocorrelation")

Demodulation
figure;
autocorr(rx);
autocorr(rx2);
dem = rx.*car;
figure;
plot(dem);
grid on;

%%Decoding
k = 1;
rcv = [];
for (i=1: length(data))
    sm = 0;
    for(j=1: length(tp)-1)
        sm = sm+dem(k);
        k =  k + 1;
    end
    if(sm>0)
        rcv=[rcv 1];
    else
        rcv = [rcv 0];
    end
end
clc;
data
rcv
