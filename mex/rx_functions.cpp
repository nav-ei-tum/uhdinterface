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
#include "storage.hpp"

std::string set_rx_freq(double freq, int uhd){
	uhd::tune_request_t tune_req(freq,Storage::getUHDStruct(uhd)->rx_freq_offset);
	uhd::tune_result_t tune_res = Storage::getUHDStruct(uhd)->usrp->set_rx_freq(tune_req);

	if(RX_DEBUG) {
		printf("rx_tune_result: %s",tune_res.to_pp_string().c_str());
	}

	return tune_res.to_pp_string();
}

void set_rx_freq_offset(double freqofst, int uhd) {
	Storage::getUHDStruct(uhd)->rx_freq_offset = freqofst;
}

void set_rx_rate(double rate, int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_rx_rate(rate);
}

void set_rx_bw  (double bw  , int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_rx_bandwidth(bw);
}

void set_rx_gain(double gain, int uhd){
	Storage::getUHDStruct(uhd)->usrp->set_rx_gain(gain);
}

void set_rx_subdev(std::string subdev, int uhd) {
	uhd::usrp::subdev_spec_t spec(subdev);
	Storage::getUHDStruct(uhd)->usrp->set_rx_subdev_spec(spec);
}

void set_rx_antenna(std::string antenna, int uhd) {
	Storage::getUHDStruct(uhd)->usrp->set_rx_antenna(antenna);
}

double get_rx_freq(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_rx_freq();
}

double get_rx_rate(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_rx_rate();
}

double get_rx_bw  (int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_rx_bandwidth();
}

double get_rx_gain(int uhd){
	return Storage::getUHDStruct(uhd)->usrp->get_rx_gain();
}

double get_rx_timespec(int uhd) {
	return Storage::getUHDStruct(uhd)->rx_time_spec;
}

std::string get_rx_subdev(int uhd) {
	return Storage::getUHDStruct(uhd)->usrp->get_rx_subdev_name();
}

std::string get_rx_antenna(int uhd) {
	return Storage::getUHDStruct(uhd)->usrp->get_rx_antenna();
}

void rx_stream_start(int uhd) {
	// check if rx_streamer already exists
	if(!Storage::getUHDStruct(uhd)->rx_stream) {
		// create rx_streamer
		uhd::stream_args_t stream_args("fc64");
		uhd::rx_streamer::sptr rx_stream_local = Storage::getUHDStruct(uhd)->usrp->get_rx_stream(stream_args);
		if(!rx_stream_local) {
			mexErrMsgIdAndTxt( "MATLAB:uhdInterface:cantCreateRXStreamer", "creating RX streamer failed" );
			return;
		}
		// store the streamer
		Storage::getUHDStruct(uhd)->rx_stream = rx_stream_local;

		// check if stored correctly
		if(!Storage::getUHDStruct(uhd)->rx_stream) {
			mexErrMsgIdAndTxt( "MATLAB:uhdInterface:cantCreateRXStreamer", "storing RX streamer failed" );
			return;
		}
	} // rx_streamer does now exists

	Storage::getUHDStruct(uhd)->usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
}

void rx_stream_stop (int uhd){
	Storage::getUHDStruct(uhd)->usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);
}

std::vector<cdouble> receiveN(size_t n, int uhd){
	// RX Metadata
	uhd::rx_metadata_t md;
	// Storage for received samples
	std::vector<cdouble> buff(n);

	size_t i, num_rx_samps = 0;

	// check if rx_streamer exists
	if(!Storage::getUHDStruct(uhd)->rx_stream) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:cantFindRXStreamer", "RX stream is not initialized yet" );
		return std::vector<cdouble>();
	}

	// initialize timepsec if secessary
	if(!Storage::getUHDStruct(uhd)->time_init) {
		Storage::getUHDStruct(uhd)->time_init = true;
		Storage::getUHDStruct(uhd)->usrp->set_time_now(uhd::time_spec_t(0.0));
	}

	// Try to receive
	for(i=0;i<RX_RECV_NUM_ATTEMPTS;i++) {
		num_rx_samps += Storage::getUHDStruct(uhd)->rx_stream->recv(&buff.at(num_rx_samps), buff.size()-num_rx_samps, md);

		// update the rx timespec if necessary
		if(md.has_time_spec) {
			Storage::getUHDStruct(uhd)->rx_time_spec = md.time_spec.get_real_secs();
		}

		if(RX_DEBUG) {
			printf("received (%d/%d) samples so far\n",num_rx_samps,n);
		}

		// Check if we received n samples in total
		if(num_rx_samps >= n) {
			if(RX_DEBUG) {
				printf("reception of %d samples complete\n", num_rx_samps);
			}
			break;
		}
		// We are not done yet, print debug output if this was the last attempt
		if(RX_DEBUG && i==RX_RECV_NUM_ATTEMPTS-1) {
			printf("Did only receive %d of %d samples\n",num_rx_samps,n);
			break;
		}
	}

	// check if there has been an error
	if(md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
		printf("rx_metadata has error code: %d\n",md.error_code);
	}

	return buff;
}
