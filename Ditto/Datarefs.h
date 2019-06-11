#pragma once
#include "cpptoml.h"
#include <vector>
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "flatbuffers/flexbuffers.h"

class dataref {
private:
	// Name, dataref, type, max range
	std::vector<std::tuple<std::string, XPLMDataRef, std::string, int>> dataref_list;
	std::vector<std::tuple<std::string, XPLMDataRef, std::string, int>> get_list();
	void initialize_list();
	int get_value_int(XPLMDataRef inDataref);
	float get_value_float(XPLMDataRef inDataref);
	double get_value_double(XPLMDataRef inDataref);
	std::vector<int> get_value_int_array(XPLMDataRef inDataref, int max_range);
	std::vector<float> get_value_float_array(XPLMDataRef inDataref, int max_range);
	std::vector<char> get_value_char_array(XPLMDataRef inDataref, int max_range);
	flexbuffers::Builder fbb;
	bool status;
public:
	void set_status(bool inStatus);
	bool get_status();
	std::vector<uint8_t> dataref::get_serialized_data();
	size_t dataref::get_serialized_size();
	void dataref::empty_list();
	void reset_builder();
	void init();
};
