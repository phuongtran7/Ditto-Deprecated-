#include "Datarefs.h"

std::vector<std::tuple<std::string, XPLMDataRef, std::string, int>> dataref::get_list()
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
		auto dataref_type = std::get<2>(dataref);
		auto dataref_max_range = std::get<3>(dataref);
		auto dataref_name = std::get<0>(dataref);
		if (dataref_max_range == 1) {
			if (dataref_type == "int") {
				fbb.Int(dataref_name.c_str(), dataref::get_value_int(std::get<1>(dataref)));
			}
			else if (dataref_type == "float") {
				fbb.Float(dataref_name.c_str(), dataref::get_value_float(std::get<1>(dataref)));
			}
			else if (dataref_type == "double") {
				fbb.Double(dataref_name.c_str(), dataref::get_value_double(std::get<1>(dataref)));
			}
		}
		else {
			if (dataref_type == "int") {
				//auto temp_vector = dataref::get_value_int_array(std::get<1>(dataref), dataref_max_range);
				//fbb.Vector(std::get<0>(dataref), temp_vector);
			}
			else if (dataref_type == "float") {
				//auto temp_vector = dataref::get_value_float_array(std::get<1>(dataref), dataref_max_range);
				//fbb.Vector(std::get<0>(dataref), temp_vector);
			}
			else if (dataref_type == "char") {
				//auto temp_vector = dataref::get_value_char_array(std::get<1>(dataref), dataref_max_range);
				//fbb.Vector(std::get<0>(dataref), temp_vector);
			}
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
		dataref_list.push_back(std::make_tuple(table->get_as<std::string>("name").value_or(""), new_dataref, table->get_as<std::string>("type").value_or(""), table->get_as<int>("max_range").value_or(1)));
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

std::vector<int> dataref::get_value_int_array(XPLMDataRef inDataref, int max_range) {
	std::vector<int> return_val;

	for (int i = 0; i < max_range; ++i) {
		int temp;
		XPLMGetDatavi(inDataref, &temp, 0, i);
		return_val.push_back(temp);
	}

	return return_val;
}
std::vector<float> dataref::get_value_float_array(XPLMDataRef inDataref, int max_range) {
	std::vector<float> return_val;

	for (int i = 0; i < max_range; ++i) {
		float temp;
		XPLMGetDatavf(inDataref, &temp, 0, i);
		return_val.push_back(temp);
	}

	return return_val;
}
std::vector<char> dataref::get_value_char_array(XPLMDataRef inDataref, int max_range) {
	std::vector<char> return_val;

	for (int i = 0; i < max_range; ++i) {
		float temp;
		XPLMGetDatab(inDataref, &temp, 0, i);
		return_val.push_back(temp);
	}

	return return_val;
}

void dataref::init() {
	initialize_list();
	set_status(true);
}