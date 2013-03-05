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

#include "uhd_functions.hpp"
#include "storage.hpp"

int initUHD(std::string device = std::string()) {
	// register message handler anyway so errors can be printed out
	uhd::msg::register_handler(&message_handler);

	return Storage::addUHDStruct(device);
}

void setTime(int uhd, double time) {
	Storage::getUHDStruct(uhd)->usrp->set_time_now(uhd::time_spec_t(time));
}

void closeUHD(size_t uhdi){
	Storage::removeUHDStruct(uhdi);
	if(Storage::getNumberOfElements() == 0) {
		closeMexFunction();
	}
}

void message_handler(uhd::msg::type_t type, const std::string &msg){

	// ensure messages are not read out during this run
	boost::mutex::scoped_lock lock(messages_mutex);

	if(type == 's') {
		messages_status.append(std::string("UHD status: "));
		messages_status.append(msg);
		messages_status.append(std::string("\n"));
	} else if(type == 'w') {
		messages_warning.append(std::string("UHD warning: "));
		messages_warning.append(msg);
		messages_warning.append(std::string("\n"));
	} else if(type == 'e') {
		messages_error.append(std::string("UHD error: "));
		messages_error.append(msg);
		messages_error.append(std::string("\n"));
	} else if(type == 'f') {
		// over/underflow messages
		messages_fastpath.append(msg);
	} else {
		messages_error.append("UHDInterface error: unknown message type. Message: ");
		messages_error.append(msg);
	}
}

void process_messages(void) {

	// ensure messages are not stored during this run
	boost::mutex::scoped_lock lock(messages_mutex);

	if(!messages_status.empty()) {
		mexPrintf(messages_status.c_str());
		messages_status = std::string("");
	}
	if(!messages_warning.empty()) {
		mexPrintf(messages_warning.c_str());
		messages_warning = std::string("");
	}
	if(!messages_error.empty()) {
		mexPrintf(messages_error.c_str());
		messages_error = std::string("");
	}
	if(!messages_fastpath.empty()) {
		mexPrintf("UHD: %s\n", messages_fastpath.c_str());
		messages_fastpath = std::string("");
	}
}
