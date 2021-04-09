# Serial Decoder

This is a decoder that listens to the incoming transmissions from the radio module a the base station and then retransmits them on MQTT or on the serial port so that we can see the incoming data. Written in several different languages that run on various platforms to be as extensible as possible. The serial decoder generally using a state machines to decode the incoming serial data and parses it into packages that contain all the data. The data can then be piped into the mqtt broker. The serial decoder should also support using dummy data, this helps developing the display code without needing to be connected to the vehicle. Since this generically listens to serial data, you can by pass the radio module and listen to the serial data from the teensy directly. This would only work if you print the same data that is transmitted to the radio module to the serial port instead. Either way, the serial decoder is very important as it is the point that translates the data from the mirocontroller and pipes it into a broker.

## Break down

A state machine is used to interpret the incoming serial data. Review the artifact on the custom protocol to review how it is structured. The basics is that each packet of received data contains an id, length of data in bytes and then the actual data. Once a single packet is received it is publish to the broker using the following topic "baja/sensors/id" with id as the incoming packet id. To listen to a specific sensor the id of that sensor needs to be know, the id that is in the topic is the hex value of that id including the 0x. For example if an rpm sensor has an id of a4, then if you want to get any data from that sensor you would subscribe to the topic "baja/sensors/0xa4". This structure was chose as it minimizes the amount of changes needed to support new sensors. In fact no changes should be made as it is as generic as possible. The Serial Decoder does no processing of the data, it only processes the incoming serial data and pipes it into the mqtt broker.

There is a lot of room for improvement however. Not all possible data is sensor data. Currently only sensor data is transmitted, but in the future commands and other data can be transmitted as well. A simple solution would be to block out id's so that id's that fall within a specific block are sensor data, and ids that fall within another block are command data. Adding this functionality would require additional processing in the serial decoder. The topics would also need to be updated, an example would be adding commands. The updated topic would be "baja/command/id". Another interesting idea would be adding a video feed, that would also have its own id and the topic might be something like "baja/video/id". So, there could be multiple video feeds. The custom protocol also has a version flag, so the protocol can be changed to support different structures while maintaining backwards compatibility.

## c_serial_decoder

This is the c version of the decoder. I would not recommend using this one, use the python one instead.

## python_decoder

This is the python version, you can view a detailed description on usage in the python_decoder folder.
