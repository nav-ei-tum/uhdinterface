
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

% data rate in samples per second
rate = 1e6;

% setparameters
uhdinterface(uhd,'set_rx_freq',2e9)
uhdinterface(uhd,'set_rx_rate',rate)
uhdinterface(uhd,'set_rx_gain',25)

% get messages from the UHD lib
uhdinterface('flush');

% number of loop cycles
n = 50;

%length of buffer per loop
m = 10e3;

% preassign buffer
vec = zeros(m,n);

% start receiving
uhdinterface(uhd,'rx_stream_start');

% throw away the first m samples
vec(:,1) = uhdinterface(uhd,'receiven',m);

% receive wave form
for i=1:n
    vec(:,i) = uhdinterface(uhd,'receiven',m);
end

% stop streaming
uhdinterface(uhd,'rx_stream_stop')

% close USRP and unlock mexfile
uhdinterface(uhd,'close')

% generate axis label
step = rate/1000/length(vec(:));
freq = -rate/1000/2:step:rate/1000/2-step;
xlabel('BB frequency [kHz]');

% compute and plot resulting fft
%plot(freq,20*abs(fftshift(fft(vec(:)))));

%figure;
%plot(real(vec(:)), imag(vec(:)),'.');
plot(abs(vec(:)))