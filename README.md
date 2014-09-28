# writetoserial

A simple program that reads the contents of a file and writes it as binary data to a serial port.

## Usage
writetoserial [OPTIONS] INPUT_FILE SERIAL_DEVICE

### Options
-h

	Print usage information.

-b

	Set baud rate (default 9600).
	
## Examples

Write the contents of image.png to the serial port /dev/ttyACM0 with a baud rate of 38400.
```
writetoserial image.png -b 38400 /dev/ttyACM0
```
