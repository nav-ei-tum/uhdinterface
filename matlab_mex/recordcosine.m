
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

addpath('../../synchronization_matlab');

% choose USRP device
uhdrecv = uhdinterface('init','serial=4e2610f4');   %USRP1
%uhdrecv = uhdinterface('init','serial=4e2611ca');   %USRP1
%uhdrecv = uhdinterface('init','addr=192.168.10.2'); %USRP N210
uhdsend = uhdrecv;

n_os = 1;
rx_rate = 0.5e6;
tx_rate = rx_rate/n_os;
freq_offset = 0e3;
freq = 2e9;
tx_gain = 20;
rx_gain = 20;
% set parameters
uhdinterface(uhdsend,'set_tx_subdev','A:0');
uhdinterface(uhdsend,'set_tx_antenna','TX/RX');
uhdinterface(uhdsend,'set_tx_freq',freq)
uhdinterface(uhdsend,'set_tx_rate',tx_rate)
uhdinterface(uhdsend,'set_tx_gain',tx_gain)
uhdinterface(uhdrecv,'set_rx_subdev','A:0');
uhdinterface(uhdrecv,'set_rx_antenna','RX2');
uhdinterface(uhdrecv,'set_rx_freq_offset',1e6);
uhdinterface(uhdrecv,'set_rx_freq',freq+freq_offset)
uhdinterface(uhdrecv,'set_rx_rate',rx_rate)
uhdinterface(uhdrecv,'set_rx_gain',rx_gain)

% get messages from the UHD lib
uhdinterface('flush');


%length of buffer per loop
m = 5e3;

% number of loop cycles
n = 10;

% generate send signal
lzeros = 15e3;
L_sync = n*m-lzeros;
L_filter = 20;
alpha = 0.5;

sync = cos(pi*(0:L_sync-1)).'*(1+0j);
g_tx_s = rrcostabs(L_filter,alpha,1,2,0);
s = conv(upsample(sync,2),g_tx_s);
s = [zeros(lzeros,1) ; s];
% preassign buffer
vec = zeros(m*n_os,n);

% start receiving
uhdinterface(uhdrecv,'rx_stream_start')

% throw away the first samples
vec(:,1) = uhdinterface(uhdrecv,'receiven',m*n_os);

% start streaming
uhdinterface(uhdsend,'tx_stream_start')
uhdinterface(uhdsend,'send',s(1:2*m));

% throw away the first samples (which already include the transmit signal
vec(:,1) = uhdinterface(uhdrecv,'receiven',m*n_os);
vec(:,1) = uhdinterface(uhdrecv,'receiven',m*n_os);

% send and rceive loop
for i=0:n-1
    uhdinterface(uhdsend,'send',s(i*m+1:(i+1)*m));
    vec(:,i+1) = uhdinterface(uhdrecv,'receiven',m*n_os);
end

% stop streaming
uhdinterface(uhdsend,'tx_stream_stop');
uhdinterface(uhdrecv,'rx_stream_stop');

uhdinterface('flush');

% close USRP and unlock mexfile
uhdinterface(uhdrecv,'close');
if(uhdrecv ~= uhdsend)
    uhdinterface(uhdsend,'close');
end

vec = vec(:);


%{
descr = sprintf('two USRP1s connected with cable and 40dB attenuation and %ddB tx + %ddB rx gain. Frequency offset of: %f',tx_gain, rx_gain,freq_offset);
%descr = sprintf('USRP1 connected with S-band Feed and %ddB tx gain',gain);

save data/sync_seq_ofst5.mat freq rx_rate tx_rate tx_gain vec descr;
%}