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
#include "tx_functions.hpp"
#include "storage.hpp"

void set_tx_freq(double freq, int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_tx_freq(freq);
}

void set_tx_rate(double rate, int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_tx_rate(rate);
}

void set_tx_bw  (double bw  , int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_tx_bandwidth(bw);
}

void set_tx_gain(double gain, int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_tx_gain(gain);
}

void set_tx_subdev(std::string subdev, int uhd) {
	uhd::usrp::subdev_spec_t spec(subdev);
	Storage::getUHDStruct(uhd)->usrp->set_tx_subdev_spec(spec);
}

void set_tx_antenna(std::string antenna, int uhd) {
	Storage::getUHDStruct(uhd)->usrp->set_tx_antenna(antenna);
}

double get_tx_freq(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_tx_freq();
}

double get_tx_rate(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_tx_rate();
}

double get_tx_bw  (int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_tx_bandwidth();
}

double get_tx_gain(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_tx_gain();
}

std::string get_tx_subdev(int uhd) {
	return Storage::getUHDStruct(uhd)->usrp->get_tx_subdev_name();
}

std::string get_tx_antenna(int uhd) {
	return Storage::getUHDStruct(uhd)->usrp->get_tx_antenna();
}

void set_tx_timespec(double timespec, int uhd) {
	Storage::getUHDStruct(uhd)->tx_time_spec = timespec;
	Storage::getUHDStruct(uhd)->tx_time_spec_init = true;
}

void tx_stream_start(int uhd) {
	// check if tx_streamer already exists
	if(!Storage::getUHDStruct(uhd)->tx_stream) {
		// create tx_streamer
		uhd::stream_args_t stream_args("fc64");
		uhd::tx_streamer::sptr tx_stream_local = Storage::getUHDStruct(uhd)->usrp->get_tx_stream(stream_args);
		if(!tx_stream_local) {
			mexErrMsgIdAndTxt( "MATLAB:uhdInterface:cantCreateTXStreamer", "creating TX streamer failed" );
			return;
		}

		// store tx_streamer
		Storage::getUHDStruct(uhd)->tx_stream = tx_stream_local;
	}

	// initialize timepsec if secessary
	if(!Storage::getUHDStruct(uhd)->time_init) {
		Storage::getUHDStruct(uhd)->time_init = true;
		Storage::getUHDStruct(uhd)->usrp->set_time_now(uhd::time_spec_t(0.0));
	}

	// create metadata
	uhd::tx_metadata_t md;
	md.start_of_burst = true;
	md.end_of_burst   = false;
	// is there a new time spec
	if(Storage::getUHDStruct(uhd)->tx_time_spec_init) {
		// if so, set it
		md.has_time_spec  = true;
		md.time_spec = uhd::time_spec_t(Storage::getUHDStruct(uhd)->tx_time_spec);

		// and disable the currently used time spec
		Storage::getUHDStruct(uhd)->tx_time_spec_init = false;
	} else {
		// no time spec available
		md.has_time_spec  = false;
	}

	// send metadata
	Storage::getUHDStruct(uhd)->tx_stream->send("", 0, md);
}

void tx_stream_stop (int uhd){
	// create end of burst metadata
	uhd::tx_metadata_t md;
	md.start_of_burst = false;
	md.end_of_burst   = true;
	md.has_time_spec  = false;

	// send metadata
	Storage::getUHDStruct(uhd)->tx_stream->send("", 0, md);
}

size_t send(std::vector<cdouble> *buff, int uhd){
	// TX Metadata
	uhd::tx_metadata_t md;
	md.start_of_burst = false;
	md.end_of_burst   = false;

	// is there a new time spec
	if(Storage::getUHDStruct(uhd)->tx_time_spec_init) {
		// if so, set it
		md.has_time_spec  = true;
		md.time_spec = uhd::time_spec_t(Storage::getUHDStruct(uhd)->tx_time_spec);

		// and disable the currently used time spec
		Storage::getUHDStruct(uhd)->tx_time_spec_init = false;
	} else {
		// no time spec available
		md.has_time_spec  = false;
	}
	size_t num_tx_samps = 0;

	if(!Storage::getUHDStruct(uhd)->tx_stream) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:cantFindTXStreamer", "TX stream is not initialized yet" );
		return 0;
	}

	num_tx_samps = Storage::getUHDStruct(uhd)->tx_stream->send(&(buff->front()), buff->size(), md, TX_TIMEOUT);

	if(TX_DEBUG) {
		printf("send (%d/%d) samples\n",num_tx_samps,buff->size());
	}

	return num_tx_samps;
}
