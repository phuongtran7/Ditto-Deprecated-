# Ditto
<h4 align="center">An X-Plane plugin that can add/remove/swap datarefs on-the-fly.</h4>

Ditto is an X-Plane plugin that allows the user to pause the simulator in a specific scenario to add/remove or swap the datarefs that Ditto is sending the values out. Ditto will allow faster iteration in plugin developments and multi-language on the receiver side.

Ditto uses <a href="https://google.github.io/flatbuffers/flexbuffers.html">Flexbuffers</a>, <a href="https://github.com/skystrife/cpptoml">cpptoml</a> and <a href="https://developer.x-plane.com/sdk/">X-Plane SDK</a>.

## Installation
### Windows
If you don't want to compile the program by yourself, you can head over the <a href="https://github.com/phuongtran7/Ditto/releases">releases</a> tab a get a pre-compiled version.

1. Install Flatbuffers, X-Plane SDK, cpptoml and Boost Asio with Microsoft's <a href="https://github.com/Microsoft/vcpkg">vcpkg</a>.
    * `vcpkg install flatbuffers`
    * `vcpkg install x-plane`
    * `vcpkg install boost-asio`
    * `vcpkg install cpptoml`
2. Clone the project: `git clone https://github.com/phuongtran7/Ditto`.
3. Build the project.

## Usage
### Start up
1. Copy the compiled plugin into plugin folder in X-Plane.
2. Copy `Datarefs.toml` file into X-Plane root folder. 
3. Define all the datarefs that the plugin should send the value out.
4. Start X-Plane.

### Modifying datarefs
1. Disable Ditto by using Plugin Manager in X-Plane.
2. (Optional) Pause X-Plane.
3. Modify `Datarefs.toml`.
4. Re-enable Ditto and unpause X-Plane if necessary.

## Related Projects
1. <a href="https://github.com/phuongtran7/Test_Receive_Flexbuffers">Test_Receive_Flexbuffers</a>: A test project to demonstrate receiving data from Ditto.