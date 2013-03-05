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

#ifndef _TX_FUNCTIONS_HPP_
#define _TX_FUNCTIONS_HPP_

/**
 * Set TX frequency in Hertz
 */
void set_tx_freq(double freq, int uhd);

/**
 * Set TX rate in samples per second
 */
void set_tx_rate(double rate, int uhd);

/**
 * Set bandwidth of TX channel filter in Hertz
 */
void set_tx_bw  (double bw  , int uhd);

/**
 * Set TX gain in the range of 0 to 30 dB
 */
void set_tx_gain(double gain, int uhd);

/**
 * Set TX subdevice specification
 */
void set_tx_subdev (std::string subdev, int uhd);

/**
 * Set TX antenna
 */
void set_tx_antenna(std::string antenna, int uhd);

/**
 * Return TX frequency in Hertz
 */
double get_tx_freq(int uhd);

/**
 * Return TX rate in samples per second
 */
double get_tx_rate(int uhd);

/**
 * Return bandwidth of TX channel filter
 */
double get_tx_bw  (int uhd);

/**
 * Return TX gain in the range from 0 to 30 dB
 */
double get_tx_gain(int uhd);

/**
 * Return the TX subdevice
 */
std::string get_tx_subdev(int uhd);

/**
 * Return the TX antenna
 */
std::string get_tx_antenna(int uhd);

/**
 * Sets the time when the next buffer should be send
 */
void set_tx_timespec(double timespec, int uhd);

/**
 * Starts TX streaming with a delay of TX_DEFAULT_DELAY to fill the tx buffer
 */
void tx_stream_start(int uhd);

/**
 * sends out an end of burst packet to stop transmitting
 */
void tx_stream_stop (int uhd);

/**
 * Tries to transmitt the samples given in buff.
 * If it takes longer than TX_TIMEOUT, transmission is aborted.
 * Returns the number of transmitted samples
 */
size_t send(std::vector<cdouble>* buff, int uhd);

#endif /*_TX_FUNCTIONS_HPP_*/
