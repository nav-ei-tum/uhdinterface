
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
%uhd = uhdinterface('init','serial=4e2611ca');   %USRP1
uhd = uhdinterface('init','addr=192.168.10.2'); %USRP N210

% setparameters
uhdinterface(uhd,'set_tx_freq',2e9)
uhdinterface(uhd,'set_tx_rate',0.5e6)
uhdinterface(uhd,'set_tx_gain',25)

% get messages from the UHD lib
uhdinterface('flush');

% generate send signal
vec = cos(pi*(0:50000-1))+0*1j; % alternating +1/-1

% start streaming
uhdinterface(uhd,'tx_stream_start')

% send wave form
for i=1:200
    uhdinterface(uhd,'send',vec);
end

% stop streaming
uhdinterface(uhd,'tx_stream_stop')

% close USRP and unlock mexfile
uhdinterface(uhd,'close')