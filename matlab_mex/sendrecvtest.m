
%{
	Copyright 2012 Institute for Communications and Navigation, TUM

	This file is part of uhdinterface.

    uhdinterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    uhdinterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uhdinterface.  If not, see <http://www.gnu.org/licenses/>.
%}

% choose USRP device
uhd = uhdinterface('init','serial=4e2610f4');   %USRP1
%uhd = uhdinterface('init','serial=4e2611ca');   %USRP1
%uhd = uhdinterface('init','addr=192.168.10.2'); %USRP N210

n_os = 2;
rx_rate = 1e6;
tx_rate = rx_rate/n_os;
freq = 2e9;
gain = 5;

% setparameters
uhdinterface(uhd,'set_tx_freq',freq)
uhdinterface(uhd,'set_tx_rate',tx_rate)
uhdinterface(uhd,'set_tx_gain',gain)
uhdinterface(uhd,'set_rx_freq',freq)
uhdinterface(uhd,'set_rx_rate',rx_rate)
uhdinterface(uhd,'set_rx_gain',0)

% get messages from the UHD lib
uhdinterface('flush');


%length of buffer per loop
m = 50000;

% number of loop cycles
n = 50;

% generate send signal
send = cos(pi*(0:(m-1)))+0j; % alternating +1/-1
%send = ones(m,1)*(1+1j);

% pre-assign buffer
vec = zeros(m*n_os,n);



% start receiving
uhdinterface(uhd,'rx_stream_start')

% throw away the first samples
vec(:,1) = uhdinterface(uhd,'receiven',m*n_os);

% start streaming
uhdinterface(uhd,'tx_stream_start')
%uhdinterface(uhd,'send',send);

% throw away the first samples (which already include the transmit signal
vec(:,1) = uhdinterface(uhd,'receiven',m*n_os);

% send and rceive loop
for i=1:n
    %uhdinterface(uhd,'send',send);
    %vec(:,i) = uhdinterface(uhd,'receiven',m*n_os);
end

% stop streaming
uhdinterface(uhd,'tx_stream_stop');
uhdinterface(uhd,'rx_stream_stop');

% close USRP and unlock mexfile
uhdinterface(uhd,'close')

%vectorize samples
vec = vec(:);

%descr = 'USRP1 connected with cable and 30dB attenuation';
%save sendrecevtest.mat freq rx_rate tx_rate gain;
%save -append sendrecevtest.mat vec descr;
%{
% generate axis label
step = rx_rate/1000/length(vec(:));
freq = -rx_rate/1000/2:step:rx_rate/1000/2-step;

% compute and plot resulting fft
plot(freq,20*log(abs(fftshift(fft(vec(:))))));

figure;
plot(real(vec(:)), imag(vec(:)),'.');
%}