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

#include "mex.h"
#include <uhd/usrp/multi_usrp.hpp>
#include <string>

#ifdef _UHDINTERFACE_CPP_
#define EXTERN
#else
#define EXTERN extern
#endif

#ifndef _UHDINTERFACE_HPP_
#define _UHDINTERFACE_HPP_

/**
 * Make sure the output gets to the Matlab console
 */
#define printf mexPrintf

/**
 * Maximum number of receive attempts before reception with less than N samples is stopped
 */
#define RX_RECV_NUM_ATTEMPTS 20

/**
 * Print out general debug information
 */
#define DEBUG false

/**
 * Whether to print out reception related debug information or not
 */
#define RX_DEBUG DEBUG

/**
 * Whether to print out transmission related debug information or not
 */
#define TX_DEBUG DEBUG

/**
 * Timeout in seconds to wait before reception is aborted
 */
#define RX_TIMEOUT 0.1

/**
 * Timeout in seconds to wait before sending is aborted
 */
#define TX_TIMEOUT 0.01

/**
 * Default number of seconds between starting tx stream and transmission in seconds
 */
#define TX_DEFAULT_DELAY 0.01

/**
 * convenience definition for complex doubles
 */
typedef std::complex<double> cdouble;

/**
 * Store all USRP related information that needs to be persistent
 */
struct uhdstruct {
	// Pointer to the USRP object
	uhd::usrp::multi_usrp::sptr usrp;
	// Pointer to the reception streamer
	uhd::rx_streamer::sptr rx_stream;
	// Pointer to the transmission streamer
	uhd::tx_streamer::sptr tx_stream;

	// timepsec of the next transmitted sample
	double tx_time_spec;
	// timespec of the last received packacge of samples
	double rx_time_spec;

	// Frequency offset of the A/D converters that needs to be compensated by the DSP later on
	double rx_freq_offset;

	// Is the value stored in tx_time_spec still valid
	bool tx_time_spec_init;

	// Has this USRP already been initialized
	bool init;

	// Has the timing of this USRP already been set, otherwhise it will be set to zero at start of reception or transmission
	bool time_init;

	uhdstruct() {
		usrp = uhd::usrp::multi_usrp::sptr(); // NULL
		init = false;

		rx_stream = uhd::rx_streamer::sptr(); // NULL
		tx_stream = uhd::tx_streamer::sptr(); // NULL
		time_init = false;

		tx_time_spec = TX_DEFAULT_DELAY;
		rx_time_spec = 0;
		rx_freq_offset = 0;
		tx_time_spec_init = false;
	}

	uhdstruct(uhd::usrp::multi_usrp::sptr usrp_sptr) {
		usrp = usrp_sptr;
		init = true;

		rx_stream = uhd::rx_streamer::sptr(); // NULL
		tx_stream = uhd::tx_streamer::sptr(); // NULL
		time_init = false;

		tx_time_spec = TX_DEFAULT_DELAY;
		rx_time_spec = 0;
		rx_freq_offset = 0;
		tx_time_spec_init = true;
	}
};



void initMexFunction(void);
void closeMexFunction(void);

#endif /*_UHDINTERFACE_HPP_*/
