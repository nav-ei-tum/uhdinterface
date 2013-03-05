/**
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
*/

#include "uhdinterface.hpp"
#include "rx_functions.hpp"
#include "tx_functions.hpp"
#include "uhd_functions.hpp"
#include "mexutil.hpp"
#include "storage.hpp"

#define _UHDINTERFACE_CPP_

extern "C" void mexFunction(int nlhs, mxArray *plhs[],	int nrhs, const mxArray *prhs[]) {

	// make sure we do not get cleared without our permission
	initMexFunction();

	// read out messages, that arrived during time in Matlab
	process_messages();

	// store the command
	std::string command;
	// store the argument
	const mxArray *arg = NULL;
	// store the uhd index
	int uhd;

	// at least command and uhd index given
	if(nrhs >= 2 && isUInt(prhs[0]) && isString(prhs[1])) {
		// store the uhd index
		uhd = getUInt(prhs[0]);
		if(!Storage::hasElement(uhd)) {
			mexErrMsgIdAndTxt( "MATLAB:uhdInterface:noUSRP", "The provided index is not associated with a valid USRP object");
		}

		// store the command string
		command = getString(prhs[1]);

		// optional argument given
		if(nrhs >= 3) {
			arg = prhs[2];
		}
		// no uhd index given
	} else if(nrhs >= 1 && isString(prhs[0])) {

		// store command
		command = getString(prhs[0]);

		// optional argument given
		if(nrhs >= 2) {
			arg = prhs[1];
		}

		// check if there is a default uhd index to fall back on
		if(Storage::getNumberOfElements() >= 1 && Storage::hasElement(0)) {
			// default index
			uhd = 0;
		} else {
			// default usrp object does not exists
			uhd = -1;
		}
		// wrong arguments given
	} else {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:noCommand", "Please specify a command");
	}

	// commands that don't access the uhd index
	if(command.compare("init") == 0) {					// ====== Init
		std::string devarg;

		// check if optional parameter is given
		if(arg == NULL) {
			// no optional parameter, default to empty string
			devarg = std::string();
		} else {
			// optional parameter given
			devarg = getString(arg);
		}

		// check if return value is requested
		if(nlhs >= 1) {
			plhs[0] = retInt(initUHD(devarg));
		}
		return;
	}
	if(command.compare("flush") == 0) {					// ====== Flush UHD output
		// do nothing, as input is checked above
		return;
	}

	if(uhd>= 0) { // uhd >= 0 in this scope
		// we have a valid, initialized uhd number
		if(Storage::getUHDStruct(uhd)->init) {
			// we can provide an additional argument
			if(arg != NULL) {
				if(command.compare("set_rx_freq") == 0) {	// ====== Set RX Frequency
					std::string ret = set_rx_freq(getDouble(arg), uhd);
					if(nlhs >= 1) {
						plhs[0] = retString(ret);
					}
					return;
				}
				if(command.compare("set_rx_freq_offset") == 0) {	// ====== Set RX Frequency
					set_rx_freq_offset(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_rx_rate") == 0) {	// ====== Set RX Rate
					set_rx_rate(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_rx_bw")   == 0) {	// ====== Set RX Bandwidth
					set_rx_bw(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_rx_gain") == 0) {	// ====== Set RX Gain
					set_rx_gain(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_rx_subdev") == 0) {	// ====== Set RX Subdevice
					set_rx_subdev(getString(arg), uhd);
					return;
				}
				if(command.compare("set_rx_antenna") == 0) {// ====== Set RX Antenna
					set_rx_antenna(getString(arg), uhd);
					return;
				}
				if(command.compare("receiven")    == 0) {	// ====== RX Receive N
					plhs[0] = retCVector(receiveN(getSize_t(arg), uhd));
					return;
				}
				if(command.compare("set_tx_freq") == 0) {	// ====== Set TX Frequency
					set_tx_freq(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_tx_rate") == 0) {	// ====== Set TX Rate
					set_tx_rate(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_tx_bw")   == 0) {	// ====== Set TX Bandwidth
					set_tx_bw(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_tx_gain") == 0) {	// ====== Set TX Gain
					set_tx_gain(getDouble(arg), uhd);
					return;
				}
				if(command.compare("set_tx_subdev") == 0) {	// ====== Set TX Subdevice
					set_tx_subdev(getString(arg), uhd);
					return;
				}
				if(command.compare("set_tx_antenna") == 0) {// ====== Set TX Antenna
					set_tx_antenna(getString(arg), uhd);
					return;
				}
				if(command.compare("set_tx_timespec") == 0) {// ====== Set TX Timespec
					set_tx_timespec(getDouble(arg), uhd);
					return;
				}
				if(command.compare("send")    == 0) {		// ====== TX Send N
					std::vector<cdouble> *buff = getCVector(arg);
					plhs[0] = retInt(send(buff, uhd));
					delete buff;
					return;
				}
			}

			// arg might be NULL here
			if(command.compare("get_rx_freq") == 0) {		// ====== Get RX Frequency
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_rx_freq(uhd));
				}
				return;
			}
			if(command.compare("get_rx_rate") == 0) {		// ====== Get RX Rate
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_rx_rate(uhd));
				}
				return;
			}
			if(command.compare("get_rx_bw")   == 0) {		// ====== Get RX Bandwidth
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_rx_bw(uhd));
				}
				return;
			}
			if(command.compare("get_rx_gain") == 0) {		// ====== Get RX Gain
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_rx_gain(uhd));
				}
				return;
			}
			if(command.compare("get_rx_subdev") == 0) {		// ====== Get RX Subdevice
				if(nlhs >= 1) {
					plhs[0] = retString(get_rx_subdev(uhd));
				}
				return;
			}
			if(command.compare("get_rx_antenna") == 0) {	// ====== Get RX Antenna
				if(nlhs >= 1) {
					plhs[0] = retString(get_rx_antenna(uhd));
				}
				return;
			}
			if(command.compare("get_rx_timespec") == 0) {	// ====== Get RX Timespec
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_rx_timespec(uhd));
				}
				return;
			}
			if(command.compare("rx_stream_start") == 0) {	// ====== RX Stream start
				rx_stream_start(uhd);
				return;
			}
			if(command.compare("rx_stream_stop")  == 0) {	// ====== RX Stream stop
				rx_stream_stop(uhd);
				return;
			}
			if(command.compare("get_tx_freq") == 0) {		// ====== Get TX Frequency
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_tx_freq(uhd));
				}
				return;
			}
			if(command.compare("get_tx_rate") == 0) {		// ====== Get TX Rate
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_tx_rate(uhd));
				}
				return;
			}
			if(command.compare("get_tx_bw")   == 0) {		// ====== Get TX Bandwidth
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_tx_bw(uhd));
				}
				return;
			}
			if(command.compare("get_tx_gain") == 0) {		// ====== Get TX Gain
				if(nlhs >= 1) {
					plhs[0] = retDouble(get_tx_gain(uhd));
				}
				return;
			}
			if(command.compare("get_tx_subdev") == 0) {		// ====== Get TX Subdevice
				if(nlhs >= 1) {
					plhs[0] = retString(get_tx_subdev(uhd));
				}
				return;
			}
			if(command.compare("get_tx_antenna") == 0) {	// ====== Get TX Antenna
				if(nlhs >= 1) {
					plhs[0] = retString(get_tx_antenna(uhd));
				}
				return;
			}
			if(command.compare("tx_stream_start") == 0) {	// ====== TX Stream start
				tx_stream_start(uhd);
				return;
			}
			if(command.compare("tx_stream_stop")  == 0) {	// ====== TX Stream stop
				tx_stream_stop(uhd);
				return;
			}
			if(command.compare("close") == 0) {				// ====== close
				closeUHD(uhd);
				return;
			}
		} // end usrp is initialized
	} // end uhd >= 0

	if(command.compare("test")    == 0) {		// ====== test 1
		if(arg != NULL) {
			Storage::setTest(getInt(arg));
		}
		if(nlhs >= 1) {
			plhs[0] = retInt(Storage::getTest());
		}
		return;
	}
	if(command.compare("closeAll")    == 0) {	// ====== close All
		closeMexFunction();
		return;
	}


	// nothing to do yet :(
	mexErrMsgIdAndTxt( "MATLAB:uhdInterface:noCommand", "Please specify a command");
	return;
}

/**
 * check if MEX file is locked and if not, lock it and register a exit handler
 */
void initMexFunction(void) {
	// if this file is already locked, there is nothing to do
	if(!mexIsLocked()) {
		// not locked yet, initialize

		// if this file has to exit, call the close function first
		mexAtExit(*closeMexFunction);

		// lock the MEX file
		mexLock();
	}
}

/**
 * Ensure the interface is cleaned before closing it
 */
void closeMexFunction(void) {
	// close all open USRP objects
	std::vector<int> indices = Storage::getIndices();
	size_t i;
	for(i=0;i<Storage::getNumberOfElements();i++) {
		closeUHD(indices[i]);
	}

	// unlock this file
	mexUnlock();
}
