#include "Boost_Socket.h"

boost_socket::boost_socket(boost::asio::io_context& io_context)
	: socket_(io_context)
{
	socket_.open(udp::v4());
}

void boost_socket::shutdown()
{
	socket_.shutdown(udp::socket::shutdown_both);
	socket_.close();
}

// Initalize all the endpoints to send the data to. Then map then identification string
// with an endpoint in a map.
// The structure of the tuple is: [IP address] - [port number] - [ID string]
size_t boost_socket::init_endpoints()
{
	auto in_vector = get_endpoints();
	for (auto& v : in_vector)
	{
		auto temp = udp::endpoint(boost::asio::ip::address::from_string(std::get<0>(v)).to_v4(), std::get<1>(v));
		endpoints_.insert(std::make_pair(std::get<2>(v), temp));
	}
	return endpoints_.size();
}

void boost_socket::reset_endpoints()
{
	endpoints_.clear();
}

// Send the data to the endpoint that matches the referenced ID string
size_t boost_socket::send_data(char* send_buf, const int size, const std::string& endpoint)
{
	try
	{
		const auto retval = socket_.send_to(boost::asio::buffer(send_buf, size), endpoints_.find(endpoint)->second);
		return retval;
	}
	catch (boost::system::system_error& e)
	{
		XPLMDebugString(("Send data to " + endpoint + " failed with error: ").c_str());
		XPLMDebugString(e.what());
		XPLMDebugString("\n");
		return 0;
	}
}

// Send the data to all endpoints
void boost_socket::send_data(char* send_buf, const int size)
{
	for (const auto& endpoint : endpoints_)
	{
		try
		{
			const auto retval = socket_.send_to(boost::asio::buffer(send_buf, size), endpoint.second);
		}
		catch (boost::system::system_error& e)
		{
			XPLMDebugString(("Send data to " + endpoint.first + " failed with error: ").c_str());
			XPLMDebugString(e.what());
			XPLMDebugString("\n");
		}
	}
}

std::vector<std::tuple<std::string, unsigned int, std::string>> boost_socket::get_endpoints()
{
	const auto input_file = cpptoml::parse_file("Datarefs.toml");

	// Create a list of all the Endpoints table in the toml file
	const auto endpoint_list = input_file->get_table_array("Endpoints");

	std::vector<std::tuple<std::string, unsigned int, std::string>> addresses;

	// Loop through all the tables
	for (const auto& endpoint : *endpoint_list)
	{
		addresses.emplace_back(
			endpoint->get_as<std::string>("address").value_or("127.0.0.1"),
			endpoint->get_as<int>("port").value_or(6996),
			endpoint->get_as<std::string>("name").value_or("Pylon")
		);
	}
	return addresses;
}