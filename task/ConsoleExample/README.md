
# ConsoleExample

ConsoleExample is a simple C/C++ example application on how to use FSSDK.

## Setting up

Before building the application, verify the settings in ConsoleExample.h.

Change the IP address EXAMPLE_IP_ADDR according to your LAN settings. Or, you
can also set it to NULL if your PC is set to obtain IP address automatically.

## Building in Windows

Open ConsoleExample in VisualStudio. Compile ConsoleExample.

## Building in Linux

Run the following commands in command line:

	mkdir build
	cd build
	cmake ..
	make

## Configurating Logging

File vevolog.conf contains logging configuration.

See https://github.com/zuhd-org/easyloggingpp for configuration details.

## Running

1. Turn on the sensor.
2. Run the application.
  - Use parameter -h for help for other parameters.

Keys:
1 = quit
2 = start 3D profiles
3 = start raw image
4 = start line interface
5 = start batch run
6 = stop

