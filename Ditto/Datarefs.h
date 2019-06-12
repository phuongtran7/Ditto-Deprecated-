#pragma once
#include "cpptoml.h"
#include <vector>
#include <optional>
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "flatbuffers/flexbuffers.h"

class dataref {
private:
	struct dataref_info {
		std::string name{};
		XPLMDataRef dataref{};
		std::string type{};
		std::optional<int> start_index{};
		std::optional<int> num_value{}; // Number of values in the array to get starts at start_index
	};

	std::vector<dataref_info> dataref_list;
	std::vector<dataref_info> get_list();
	void initialize_list();
	int get_value_int(XPLMDataRef inDataref);
	float get_value_float(XPLMDataRef inDataref);
	double get_value_double(XPLMDataRef inDataref);
	std::vector<int> get_value_int_array(XPLMDataRef inDataref, int start_index, int end_index);
	std::vector<float> get_value_float_array(XPLMDataRef inDataref, int start_index, int end_index);
	std::vector<char> get_value_char_array(XPLMDataRef inDataref, int start_index, int end_index);
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
