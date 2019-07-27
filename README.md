# Ditto
<h4 align="center">An X-Plane plugin that can add/remove/swap datarefs and endpoints on-the-fly.</h4>

Ditto is an X-Plane plugin that allows the user to pause the simulator in a specific scenario to add/remove or swap the datarefs that Ditto is sending the values out; along with the endpoint's IP address/port number. Ditto will allow faster iteration in plugin developments and language independent on the receiver side thanks to Flatbuffers.

Ditto uses <a href="https://google.github.io/flatbuffers/flexbuffers.html">Flexbuffers</a>, <a href="https://github.com/skystrife/cpptoml">cpptoml</a>, <a href="https://developer.x-plane.com/sdk/">X-Plane SDK</a> and <a href="http://boost.org/libs/asio">Boost Asio</a>.

## Installation
### Windows
If you don't want to compile the plugin by yourself, you can head over the <a href="https://github.com/phuongtran7/Ditto/releases">releases</a> tab a get a pre-compiled version.

1. Install Flatbuffers, X-Plane SDK, cpptoml and Boost Asio with Microsoft's <a href="https://github.com/Microsoft/vcpkg">vcpkg</a>.
    * `vcpkg install flatbuffers`
    * `vcpkg install x-plane`
    * `vcpkg install boost-asio`
    * `vcpkg install cpptoml`
2. Clone the project: `git clone https://github.com/phuongtran7/Ditto`.
3. Build the project.

## Usage
### Start up
1. Copy the compiled Ditto into plugin folder in X-Plane.
2. Copy `Datarefs.toml` file into Ditto folder. For example, `path_to_aircraft/plugins/Ditto/`.
3. Define all the endpoints that the data should be sent to.
4. Define all the datarefs that the plugin should send the value out.
5. Start X-Plane.

### Modifying datarefs/endpoints
1. Disable Ditto by using Plugin Manager in X-Plane.
2. (Optional) Pause X-Plane.
3. Modify `Datarefs.toml`.
4. Re-enable Ditto and unpause X-Plane if necessary.

## Limiations
1. Currently, if Ditto needs to be installed in multiple aircrafts, it has to be copied into each aircraft's plugin folder along with the `Datarefs.toml` file. This is currently the only way for Ditto to have different endpoints and set of datarefs for each aircraft.

2. Even though Ditto supports multiple endpoints, there is currently no way to specify which endpoint should receive which values. All the endpoints will be receiving the same data that is output by Ditto.

## Related Projects
1. <a href="https://github.com/phuongtran7/Test_Receive_Flexbuffers">Test_Receive_Flexbuffers</a>: A test project to demonstrate receiving data from Ditto.