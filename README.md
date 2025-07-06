&nbsp; <br>

<div align="center">
   <a href='https://cot.as/tjenester' target="__blank">
      <picture>
         <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Company-of-Things/.github/3d85f205b06ba0b85365c6a1c69f55d22b5ba8c2/deploii/logos/deploii_light.svg" />
         <img height="80" alt="Deploii" src='https://raw.githubusercontent.com/Company-of-Things/.github/3d85f205b06ba0b85365c6a1c69f55d22b5ba8c2/deploii/logos/deploii_dark.svg'><br>
      </picture>
   </a>

  <p>Educational IoT platform</p>

  ![Release](https://img.shields.io/github/v/release/Company-of-Things/deploii-library?sort=date&display_name=tag&color=blue)
![Release Date](https://img.shields.io/github/release-date/Company-of-Things/deploii-library?sort=date&display_name=tag&color=green)
![Downloads](https://img.shields.io/github/downloads/Company-of-Things/deploii-library/latest/total?sort=date&label=downloads&color=green)
  
</div>
&nbsp; <br>

An Arduino library for connecting your microcontroller to [Deploii](https://www.deploii.no), an educational IoT platform developed by Company of Things.

# Getting started 
The first thing you want to do is to use macros to specify your medium and protocols, import the library and create an instance of the Deploii class. Here you will need to provide your board ID from Deploii. A list of all currently available mediums and protocols can be found [here](#currently-supported-mcus-and-protocols).
```c++
#define DEPLOII_MEDIUM DEPLOII_MEDIUM_WIFI
#define DEPLOII_PROTOCOL DEPLOII_PROTOCOL_WEBSOCKETS

#include <deploii.h>

Deploii oi("Board ID");
```
In the setup function of the Arduino sketch, you need to call the connect function to connect to Deploii. The parameters of the connect function will vary based on your selected medium and protocol. For connection with Wifi and WebSockets this looks like:
```c++
void setup() {
 oi.connect("WiFi SSID", "WiFi Password");
}
```
Now you can send data using the send function and providing a data stream ID and a variable containing your data. The send function works on all datatypes and arrays. 
```c++
oi.send("Data stream ID", data);
```
If you want to do repeated tasks such as sending data at a set interval, you can use the interval function. You call the interval function in setup to register an interval. The function takes an interval specified in milliseconds and a function to be executed at that interval. 
```c++
oi.interval(1000, myFunction);
```
In order to receive data from the Deploii control panel you need to register a callback with the receive function in setup. This callback function takes in the control panel module ID and its corresponding data, both as a String, and is called every time the MCU receives data. 
```c++
oi.receive(myCallback);
```
For protocols that maintain a persistent connection, such as the WebSocket protocol or when using intervals, it is required that you call the Deploii loop function inside of the arduino void loop. You also cannot use delays or other blocking code when using these protocols as they might break the connection.
```c++
void loop(){
  oi.loop();
}
```
By default, the library has a debug level set to INFO. the available debug levels are NONE, INFO and VERBOSE. Debug messages are printed to Serial, therefore Serial.begin() will be called from the debugger for all levels above NONE. Changing the debug level can be done with the following macro, which needs to happen before the inclusion of the deploii library.
```c++
#define DEPLOII_DEBUG DEPLOII_DEBUG_VERBOSE
```

For more examples of how to use the library, please see [examples](https://github.com/Company-of-Things/deploii-library/tree/dev/examples). 

# Currently supported MCUs and protocols

|              | WiFi/WebSockets  | WiFi/HTTP |
|--------------|------------------|-----------|
| ESP32        |        ✅       |    ✅     |          
| Arduino WiFi |        ❌       |    ❌     |
