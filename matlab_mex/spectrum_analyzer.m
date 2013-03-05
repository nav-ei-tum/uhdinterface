
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

clear all;
close all;
clc;

% choose USRP device
%uhd = uhdinterface('init','serial=4e2610f4');   %USRP1
%uhd = uhdinterface('init','serial=4e2611ca');   %USRP1
uhd = uhdinterface('init','addr=192.168.10.2'); %USRP N210

% data rate in samples per second
rate = 2e6;
% freq = 1541.6e6; % Inmarsat 4F2
% freq = 1878.5e6; % Meteosat 9
freq = 2e9;
gain = 20;
update_time = 0.1;
avrg_len = 10;

% setparameters
uhdinterface(uhd,'set_rx_freq',freq)
uhdinterface(uhd,'set_rx_rate',rate)
uhdinterface(uhd,'set_rx_gain',gain)

% get messages from the UHD lib
uhdinterface('flush');

%length of buffer
N = 10e3;

% number of loop cycles to skip before recomputing the fft
n = ceil(update_time/N*rate);

% frequency vector
f = (((0:N-1) - ceil(N/2)) / N * rate)/1000;
% generate axis label
figurehandle = figure(1);

col = {[0/256   101/256 189/256], ... %blue
    [162/256 173/256   0/256], ... %green
    [227/256 114/256  34/256], ... %orange
    [105/256   8/256  90/256], ... %lila
    [196/256   7/256  27/256]      %red
    };

% set(figurehandle, 'Windowstyle', 'modal');
plothandle = plot(f,zeros(size(f)),'Color',col{1});
xlabel(sprintf('Frequency [kHz] - %2.3f GHz',freq*1e-9));
y_max = 0;
y_min = -150;
x_max = max(f);
x_min = min(f);
axis([x_min x_max y_min y_max]);

% start receiving
uhdinterface(uhd,'rx_stream_start');

% throw away the first N samples
vec = uhdinterface(uhd,'receiven',N);

i = 1;
reset_avrg = true;
% receive wave form
while true
    vec = uhdinterface(uhd,'receiven',N);
    if not(ishandle(plothandle))
        break;
    end
    if(i >= n)
        i = 1;
        newfftvec = 20*log10(abs(fftshift(fft(vec))/N));
        if reset_avrg
            fftvec = newfftvec;
            reset_avrg = false;
        else
            fftvec = (avrg_len*fftvec + newfftvec)/(avrg_len+1);
        end
        set(plothandle,'YData',fftvec);  % Update the plot line
        drawnow;
    end

%     pressedKey = get(figurehandle, 'CurrentCharacter');
%     if pressedKey == 'q'
%         break;
%     end
    i = i+1;
end

% stop streaming
uhdinterface(uhd, 'rx_stream_stop');

% close USRP and unlock mexfile
uhdinterface(uhd, 'close');

% set(figurehandle, 'Windowstyle', 'normal');
