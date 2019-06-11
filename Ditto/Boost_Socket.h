#pragma once
#include <boost/asio.hpp>
#include "XPLMUtilities.h"

using boost::asio::ip::udp;

class boost_socket
{
public:
	explicit boost_socket(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
		socket_.open(udp::v4());
	}

	// Initalize all the endpoints to send the data to. Then map then identification string
	// with an endpoint in a map.
	// The structure of the tuple is: [IP address] - [port number] - [ID string]
	size_t init_endpoint(std::vector<std::tuple<std::string, unsigned int, std::string>> in_vector)
	{
		for (auto &v : in_vector)
		{
			auto temp = udp::endpoint(boost::asio::ip::address::from_string(std::get<0>(v)).to_v4(), std::get<1>(v));
			endpoints_.insert(std::make_pair(std::get<2>(v), temp));
		}
		return endpoints_.size();
	}

	// Send the data to the endpoint that matches the referenced ID string
	size_t send_data(char * send_buf, const int size, const std::string& endpoint)
	{
		try
		{
			const auto retval = socket_.send_to(boost::asio::buffer(send_buf, size), endpoints_.find(endpoint)->second);
			return retval;
		}
		catch (boost::system::system_error &e)
		{
			XPLMDebugString(("Send data to " + endpoint + " failed with error: ").c_str());
			XPLMDebugString(e.what());
			XPLMDebugString("\n");
			return 0;
		}
	}
private:
	udp::socket socket_;
	std::map<std::string, udp::endpoint> endpoints_;
};