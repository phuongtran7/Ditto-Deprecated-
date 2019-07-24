#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class boost_socket
{
public:
	explicit boost_socket(boost::asio::io_context& io_context);

	void shutdown();

	size_t init_endpoint(std::vector<std::tuple<std::string, unsigned int, std::string>> in_vector);

	size_t send_data(char* send_buf, const int size, const std::string& endpoint);
private:
	udp::socket socket_;
	std::map<std::string, udp::endpoint> endpoints_;
};