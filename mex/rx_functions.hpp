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

#ifndef _RX_FUNCTIONS_HPP_
#define _RX_FUNCTIONS_HPP_

/**
 * Set RX frequency in Hertz. Returning the tune results
 */
std::string set_rx_freq(double freq, int uhd);

/**
 * Set the RX rate in samples per second
 */
void set_rx_rate(double rate, int uhd);

/**
 * Set the bandwidth of the RX channel filter in Hertz
 */
void set_rx_bw  (double bw  , int uhd);

/**
 * Set RX gain in the range of 0 to 30 dB
 */
void set_rx_gain(double gain, int uhd);

/**
 * Set the RX subdevice specification
 */
void set_rx_subdev (std::string subdev, int uhd);

/**
 * Set the RX antenna
 */
void set_rx_antenna(std::string antenna, int uhd);

/**
 * Set the tune request frequency offset
 */
void set_rx_freq_offset(double freqofst, int uhd);

/**
 * Return the RX frequency in Hertz
 */
double get_rx_freq(int uhd);

/**
 * Return the RX rate in samples per second
 */
double get_rx_rate(int uhd);

/**
 * Return the bandwidth of the RX channel filter in Hertz
 */
double get_rx_bw  (int uhd);

/**
 * Return the RX gain in the range from 0 to 30 dB
 */
double get_rx_gain(int uhd);

/**
 * Return the RX subdevice pretty print string
 */
std::string get_rx_subdev(int uhd);

/**
 * Return the RX antenna
 */
std::string get_rx_antenna(int uhd);

/**
 * Return the timepsec of the last received sample packet
 */
double get_rx_timespec(int uhd);

/**
 * Start the reception into the USRPs internal buffer
 */
void rx_stream_start(int uhd);

/**
 * Stop the reception
 */
void rx_stream_stop (int uhd);

/**
 * Try to receive the specified amount of samples and return them.
 * If the timespec of the usrp has not been set yet, it is initialized to zero.
 * If either RX_RECV_NUM_ATTEMPTS or RX_TIMEOUT seconds have passed before less samples are returned
 */
std::vector<cdouble> receiveN(size_t, int uhd);

#endif /*_RX_FUNCTIONS_HPP_*/
