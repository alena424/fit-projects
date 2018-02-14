% Alena Tesarova (xtesar36) - ISS projekt, 01.01.2018

% 1.priklad
[y, Fs] = audioread('xtesar36.wav');
vzorky = length(y);
cas = vzorky / Fs;

% 2.priklad
fou = fft(y); % fast fourier transform %
absfou = abs(fou);
f = linspace(0, Fs/2-1, Fs/2); % linspace - , Fs/2 vzorku od 0 do Fs/2-1 
plot(f, absfou(1:Fs/2)); xlabel 'f[Hz]'; 

% 3.priklad
[maxf, max_index] = max( absfou );
%max_index = 936 Hz;


% 4.priklad
a = [1, 0.2289, 0.4662];
b = [0.2324, -0.4112, 0.2324];
fvtool(b,a,'polezero');

% 5.priklad
fvtool(b,a,'magnitude'); 

% 6.priklad
filtr = filter(b,a,y);
fourier = fft(filtr);
absfourier = abs(fourier);
plot( f, absfourier(1:Fs/2) ); xlabel 'f[Hz]';

% 7.priklad
[module, index_max] = max(absfourier) % index_max, kde se nachází maximum

% 8.priklad -
% 9.priklad
[kor, lag] = xcorr(y, 'biased');
plot(lag, kor); xlim( [ -50 50 ] );

% 10.priklad
matice = zeros(20); % vytvorime matici nul, do kt. budeme ukladat pocty

signal_nuly_zac = cat(1, zeros(10, 1), y); % konkatenace nul a puvodniho signalu nakonec
signal_nuly_konec = cat(1, y, zeros(10, 1)); % konkatenace nul na zacatek

signal_nuly_konec = signal_nuly_konec * 10;
signal_nuly_zac = signal_nuly_zac * 10;
%indexy
puvodni_vzorek = 0;
posunuty_vzorek = 0;

for i = 1:(vzorky+10) % projizdim od 1 do 16010
puvodni_vzorek = round(signal_nuly_konec(i)) + 10; % puvodni neposonuty vzorek 
posunuty_vzorek = round(signal_nuly_zac(i)) + 10; % posunuty vzorek
matice(posunuty_vzorek, puvodni_vzorek) = matice(posunuty_vzorek, puvodni_vzorek) + 1; % inkrementujeme
end

matice = matice/(vzorky+10); % delime poctem realizaci
surf(-1:0.1:0.9, -1:0.1:0.9, matice); % krok = 0.1

%11. priklad
S = sum(sum(matice,2))