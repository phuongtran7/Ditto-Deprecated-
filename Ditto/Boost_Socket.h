#pragma once
#include <boost/asio.hpp>
#include <cpptoml.h>
#include <XPLMUtilities.h>

using boost::asio::ip::udp;

class boost_socket
{
public:
	explicit boost_socket(boost::asio::io_context& io_context);
	void shutdown();
	void disable_socket();
	bool enable_socket();
	size_t send_data(char* send_buf, size_t size, const std::string& endpoint);
	void send_data(char* send_buf, size_t size);
	void set_plugin_path(const std::string& path);
private:
	size_t init_endpoints();
	std::string plugin_path_{};
	std::map<std::string, udp::endpoint> endpoints_;
	udp::socket socket_;
	std::vector<std::tuple<std::string, unsigned int, std::string>> get_endpoints();
};