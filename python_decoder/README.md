# Python Serial Decoder

The python serial decoder does what is described in the main REAMDE.

## Getting started

To get started install python onto your system along with pip. There is a single package that is used that is not included in the default python installation and that is the mqtt package. This is the link to that package https://pypi.org/project/paho-mqtt/.

## Usage

This program has the following command line usage.

```bash
usage: decoder.py [-h] [--file Save to file] [--serialport SerialPort]
                  [--verbose] [--test] [--mqtt] [--host HOST] [--port PORT]
                  [--baud BAUD]

optional arguments:
  -h, --help            show this help message and exit
  --file Save to file, -f Save to file
                        Serial port
  --serialport SerialPort, -sp SerialPort
                        Serial port
  --verbose, -v         Verbose will print everything that is happening
  --test, -t            Enable dummy data to be transmitted.
  --mqtt, -m            To use MQTT
  --host HOST           MQTT host name, default localhost
  --port PORT           MQTT port name, default 1883
  --baud BAUD, -b BAUD  MQTT port name, default 9600
```

Somethings to watch out for is that the default port is 1883, if you are using an unmodified version of the ReactUI app make sure to specify the port as 8883. The dummy data also needs to be updated so that it gives the data you want.

An example of this using dummy data is the following.

```bash
python decoder.py --port 8883 --host localhost --mqtt --test
```

Another example that listens to incoming serial data and transmits to mqtt broker

```bash
python decoder.py --serialport COM19 --port 8883 --host localhost --mqtt
```

## Break down

A state machine is used to interpret the incoming serial data. Review the artifact on the custom protocol to review how it is structured. The basics is that each packet of received data contains an id, length of data in bytes and then the actual data. Once a single packet is received it is publish to the broker using the following topic "baja/sensors/id" with id as the incoming packet id. To listen to a specific sensor the id of that sensor needs to be know, the id that is in the topic is the hex value of that id including the 0x. For example if an rpm sensor has an id of a4, then if you want to get any data from that sensor you would subscribe to the topic "baja/sensors/0xa4". This structure was chose as it minimizes the amount of changes needed to support new sensors. In fact no changes should be made as it is as generic as possible. The Serial Decoder does no processing of the data, it only processes the incoming serial data and pipes it into the mqtt broker.
