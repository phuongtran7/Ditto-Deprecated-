#include "Datarefs.h"

std::vector<dataref::dataref_info> dataref::get_list()
{
	return dataref_list;
}

void dataref::reset_builder() {
	fbb.Clear();
}

// Remove all the dataref in the dataref list
void dataref::empty_list() {
	dataref_list.clear();
	reset_builder();
	set_status(false);
}

std::vector<uint8_t> dataref::get_serialized_data() {

	auto map_start = fbb.StartMap();

	for (auto& dataref : dataref::dataref_list) {

		// If start and end index does not present that means the dataref is single value dataref
		if (!dataref.start_index.has_value() && !dataref.end_index.has_value()) {
			if (dataref.type == "int") {
				fbb.Int(dataref.name.c_str(), dataref::get_value_int(dataref.dataref));
			}
			else if (dataref.type == "float") {
				fbb.Float(dataref.name.c_str(), dataref::get_value_float(dataref.dataref));
			}
			else if (dataref.type == "double") {
				fbb.Double(dataref.name.c_str(), dataref::get_value_double(dataref.dataref));
			}
		}
		else {
			auto vector_start = fbb.StartVector(dataref.name.c_str());
			if (dataref.type == "int") {
				for (auto int_num : dataref::get_value_int_array(dataref.dataref, dataref.start_index.value(), dataref.end_index.value())) {
					fbb.Int(int_num);
				}
			}
			else if (dataref.type == "float") {
				for (auto float_num : dataref::get_value_float_array(dataref.dataref, dataref.start_index.value(), dataref.end_index.value())) {
					XPLMDebugString(("Float dataref: " + dataref.name + " " + std::to_string(float_num) + "\n").c_str());
					fbb.Float(float_num);
				}
			}
			else if (dataref.type == "char") {
				auto str = dataref::get_value_char_array(dataref.dataref, dataref.start_index.value(), dataref.end_index.value());
				fbb.String(std::string(str.begin(), str.end()));
			}
			fbb.EndVector(vector_start, true, true);
		}
	}

	fbb.EndMap(map_start);

	fbb.Finish();

	return fbb.GetBuffer();
}

bool dataref::get_status() {
	return dataref::status;
}

void dataref::set_status(bool inStatus) {
	dataref::status = inStatus;
}

size_t dataref::get_serialized_size() {
	return fbb.GetSize();
}

void dataref::initialize_list()
{

	auto input_file = cpptoml::parse_file("Datarefs.toml");

	// Create a list of all the data table in the toml file
	auto tarr = input_file->get_table_array("Data");

	// Loop through all the tables
	for (const auto& table : *tarr)
	{
		// Construct a new struct to store each dataref data
		XPLMDataRef new_dataref;
		new_dataref = XPLMFindDataRef(table->get_as<std::string>("string").value_or("").c_str());

		dataref::dataref_info temp_dataref_info{
			table->get_as<std::string>("name").value_or(""),
			new_dataref,
			table->get_as<std::string>("type").value_or(""),
			table->get_as<int>("start_index").value_or(std::nullopt),
			table->get_as<int>("end_index").value_or(std::nullopt),
		};
		dataref_list.push_back(temp_dataref_info);
	}
}

int dataref::get_value_int(XPLMDataRef inDataref)
{
	return XPLMGetDatai(inDataref);
}

float dataref::get_value_float(XPLMDataRef inDataref) {
	return XPLMGetDataf(inDataref);
}

double dataref::get_value_double(XPLMDataRef inDataref) {
	return XPLMGetDatad(inDataref);
}

std::vector<int> dataref::get_value_int_array(XPLMDataRef inDataref, int start_index, int end_index) {
	std::vector<int> return_val;
	for (start_index; start_index <= end_index; ++start_index) {
		int temp;
		XPLMGetDatavi(inDataref, &temp, start_index, 1);
		return_val.push_back(temp);
	}
	return return_val;
}
std::vector<float> dataref::get_value_float_array(XPLMDataRef inDataref, int start_index, int end_index) {
	std::vector<float> return_val;

	for (start_index; start_index <= end_index; ++start_index) {
		float temp;
		XPLMGetDatavf(inDataref, &temp, start_index, 1);
		return_val.push_back(temp);
		XPLMDebugString(("Got float: " + std::to_string(temp) + "\n").c_str());
	}

	return return_val;
}
std::vector<char> dataref::get_value_char_array(XPLMDataRef inDataref, int start_index, int end_index) {
	std::vector<char> return_val;

	for (start_index; start_index <= end_index; ++start_index) {
		float temp;
		XPLMGetDatab(inDataref, &temp, start_index, 1);
		return_val.push_back(temp);
	}

	return return_val;
}

void dataref::init() {
	initialize_list();
	set_status(true);
}