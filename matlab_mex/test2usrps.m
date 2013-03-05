
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

disp 'initialize usrp1'
r1 = uhdinterface('init','serial=4e2611ca')
uhdinterface('flush');

disp 'initialize usrp n210'
r2 = uhdinterface('init','add=r192.168.10.2')
uhdinterface('flush');

disp 'set frequency for usrp1'
uhdinterface(r1,'set_rx_freq',1.49e9)
uhdinterface('flush');

disp 'set frequency for usrp n210'
uhdinterface(r2,'set_rx_freq',1.5e9)
uhdinterface('flush');

