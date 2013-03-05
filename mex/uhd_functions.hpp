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
#include <string>
#include <uhd/utils/msg.hpp>
#include <boost/thread/mutex.hpp>

#ifndef UHD_FUNCTIONS_HPP_
#define UHD_FUNCTIONS_HPP_

/**
 * Buffer for synchronous messages from the UHD
 */
static std::string messages_status("");
static std::string messages_warning("");
static std::string messages_error("");
static std::string messages_fastpath("");

/**
 * Mutex to ensure that either the UHD or the Matlab Thread is accessing the message buffers,
 * but not both at the same time.
 */
static boost::mutex messages_mutex;


/**
 * Initializes a USRP object and creates the according uhdstruct.
 * Returns the uhd index or -1 if the initialization failed.
 */
int initUHD(std::string device);

/**
 * Sets the device's time spec by calling set_time_now(time)
 */
void setTime(int uhd, double time);

/**
 * closes the UHD structure with the uhd index and if no uhd object is left, unlocks the mexFunction
 */
void closeUHD(size_t);

/**
 * get's called by the UHD lib. Stores the parametrized message in a thread safe variable
 */
void message_handler(uhd::msg::type_t type, const std::string &msg);

/**
 * processes and prints out every message that has benn stored by the message_handler function in a thread safe way
 */
void process_messages(void);

#endif /* UHD_FUNCTIONS_HPP_ */
