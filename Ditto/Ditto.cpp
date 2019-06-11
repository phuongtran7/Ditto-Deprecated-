// Used to disable warning when compiling with C++17.
// More info: https://github.com/boostorg/asio/issues/109
#define  _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

// More info about ulti.h error
// https://github.com/google/flatbuffers/issues/5237
// Solution: define NOMINMAX in preprocessor

#include "Boost_Socket.h"
#include "XPLMDataAccess.h"
#include "XPLMProcessing.h"
#include "Datarefs.h"

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

boost::asio::io_context io_context;
boost_socket new_socket(io_context);
dataref new_data;

static float	listenCallback(
	float                inElapsedSinceLastCall,
	float                inElapsedTimeSinceLastFlightLoop,
	int                  inCounter,
	void *               inRefcon);

PLUGIN_API int XPluginStart(
	char *		outName,
	char *		outSig,
	char *		outDesc)
{
	strcpy_s(outName, 13, "Ditto");
	strcpy_s(outSig, 21, "phuong.x-plane.ditto");
	strcpy_s(outDesc, 45, "A dataref extractor that can grow or shrink.");

	std::vector<std::tuple<std::string, unsigned int, std::string>> addresses;
	addresses.emplace_back("127.0.0.1", 6996, "Pylon");

	io_context.run();

	if (!new_socket.init_endpoint(addresses))
	{
		XPLMDebugString("Cannot init endpoints.\n");
		return 0;
	}

	new_data.init();

	//Register to get callback every frame
	XPLMRegisterFlightLoopCallback(listenCallback, -1.0, nullptr);

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	/* Unregister the callback */
	XPLMUnregisterFlightLoopCallback(listenCallback, nullptr);
}

PLUGIN_API void XPluginDisable(void) {
	new_data.empty_list();
}
PLUGIN_API int  XPluginEnable(void)  {
	if (!new_data.get_status()) {
		new_data.init();
	}
	return 1;
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }

float listenCallback(float inElapsedSinceLastCall,
	float inElapsedTimeSinceLastFlightLoop,
	int inCounter,
	void* inRefcon)
{

	auto out_data = new_data.get_serialized_data();
	auto size = new_data.get_serialized_size();
	new_socket.send_data(reinterpret_cast<char*>(&out_data[0]), size, "Pylon");

	new_data.reset_builder();

	return -1.0;
}