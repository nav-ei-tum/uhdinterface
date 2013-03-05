
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
%uhd = uhdinterface('init','serial=4e2610f4');   %USRP1
uhd = uhdinterface('init','serial=4e2611ca');   %USRP1
%uhd = uhdinterface('init','addr=192.168.10.2'); %USRP N210

rx_rate = 1e6;
tx_rate = rx_rate;
freq = 2e9;
gain = 0;
% setparameters
uhdinterface(uhd,'set_tx_freq',freq)
uhdinterface(uhd,'set_tx_rate',tx_rate)
uhdinterface(uhd,'set_tx_gain',gain)
uhdinterface(uhd,'set_rx_freq',freq)
uhdinterface(uhd,'set_rx_rate',rx_rate)
uhdinterface(uhd,'set_rx_gain',0)

% get messages from the UHD lib
uhdinterface('flush');

disp 'measuring measurement delay';
% measure the time it takes to measure time
meas_delay = zeros(2000,1);
for i=1:length(meas_delay)
    tic;
    meas_delay(i) = toc;
end

disp 'measuring loop delay';
% measure time it takes to repeat a loop statement
loop_delay = zeros(2000,1);
tic;
for i=1:length(meas_delay)
    loop_delay(i) = toc;
    tic;
end
toc;


disp 'measuring sending delay';
uhdinterface(uhd,'tx_stream_start')
send_delay = zeros(100,500);
for n=1:100
    send = ones(1000*n,1)*(1+0j);
    sprintf('packet size: %d',length(send))
    for i=1:500
        tic;
        uhdinterface(uhd,'send',send);
        send_delay(n,i) = toc;
    end
end
uhdinterface(uhd,'tx_stream_stop')
%%

disp 'measuring reception delay';
uhdinterface(uhd,'rx_stream_start')
recv_delay = zeros(100,500);
for n=1:100
    
    sprintf('packet size: %d',1000*n)
    for i=1:500
        tic;
        r = uhdinterface(uhd,'receiven',1000*n);
        recv_delay(n,i) = toc;
    end
end