#include "Datarefs.h"

boost_socket new_socket;
dataref new_data;

static float	listenCallback(
	float                inElapsedSinceLastCall,
	float                inElapsedTimeSinceLastFlightLoop,
	int                  inCounter,
	void* inRefcon);

std::string get_plugin_path()
{
	const auto my_id = XPLMFindPluginBySignature("phuong.x-plane.ditto");;
	char buffer[1024]{};
	XPLMGetPluginInfo(my_id, nullptr, buffer, nullptr, nullptr);
	auto return_string = std::string(buffer);
	auto pos = return_string.find(R"(64\win.xpl)");
	if (pos != std::string::npos)
	{
		// XPLMGetPluginInfo return absolute path to win.xpl file so it need to be trimmed off the string
		return_string.erase(pos, return_string.length()); // Trim "64\win.xpl"

	}
	return return_string;
}

PLUGIN_API int XPluginStart(
	char* outName,
	char* outSig,
	char* outDesc)
{
	strcpy_s(outName, 13, "Ditto");
	strcpy_s(outSig, 21, "phuong.x-plane.ditto");
	strcpy_s(outDesc, 45, "A dataref extractor that can grow or shrink.");

	XPLMDebugString("Starting Ditto.\n");

	//Register to get callback every frame
	XPLMRegisterFlightLoopCallback(listenCallback, -1.0, nullptr);

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	new_data.empty_list();
	XPLMUnregisterFlightLoopCallback(listenCallback, nullptr);
	new_socket.shutdown();
	XPLMDebugString("Stopping Ditto.\n");
}

PLUGIN_API void XPluginDisable(void) {
	new_data.empty_list();
	new_socket.disable_socket();
	XPLMUnregisterFlightLoopCallback(listenCallback, nullptr);
	XPLMDebugString("Disabling Ditto.\n");
}

PLUGIN_API int XPluginEnable(void) {
	if (!new_data.get_status()) {

		const auto path = get_plugin_path();

		new_data.set_plugin_path(path);
		new_socket.set_plugin_path(path);

		new_data.init();
		if (!new_socket.enable_socket())
		{
			XPLMDebugString("Cannot init endpoints.\n");
			return 0; // Disable the plugin as it cannot init endpoint.
		}

		XPLMRegisterFlightLoopCallback(listenCallback, -1.0, nullptr);
	}
	XPLMDebugString("Enabling Ditto.\n");
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) { }

float listenCallback(float inElapsedSinceLastCall,
	float inElapsedTimeSinceLastFlightLoop,
	int inCounter,
	void* inRefcon)
{
	const auto out_data = new_data.get_serialized_data();
	const auto size = new_data.get_serialized_size();

	auto verifier = flatbuffers::Verifier(out_data, size);
	if (Ditto::VerifyDataBuffer(verifier)) {
		new_socket.send_data(reinterpret_cast<char*>(&out_data[0]), size);
	}
	else {
		XPLMDebugString("Flatbuffers verifier failed.\n");
	}

	new_data.reset_builder();

	return -1.0;
}