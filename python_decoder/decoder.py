import argparse
import serial
import time
import struct
import paho.mqtt.client as mqtt

verbose = False
use_mqtt = False
host = "localhost"
port = "1883"
serialPort = ""
use_dummy = False
baud = 9600
args = None
ser = None
file = None
frame = {}

state = "START"

# Change this depending on the protocol you want to use. Sending data to one MQTT port will not send it to all the ports.
# So if you are using the ReactUI web app make sure that you have websockets enables here, or it will not connect to the broker!
# client = mqtt.Client()
client = mqtt.Client(transport="websockets")
__dummy__data__a4 = 0.0

# The callback for when the client receives a CONNACK response from the server.


def on_connect(client, userdata, flags, rc):
    pass
    # print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # client.subscribe("$SYS/#")

    # The callback for when a PUBLISH message is received from the server.


def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))


client.on_connect = on_connect
client.on_message = on_message


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", "-f", metavar="Save to file",
                        type=str, help="Serial port")
    parser.add_argument("--serialport", "-sp", metavar="SerialPort",
                        type=str, help="Serial port")
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Verbose will print everything that is happening")
    parser.add_argument("--test", "-t", action="store_true",
                        help="Enable dummy data to be transmitted.")
    parser.add_argument(
        "--mqtt", "-m", action="store_true", help="To use MQTT")
    parser.add_argument("--host", type=str,
                        required=False, help="MQTT host name, default localhost", default="localhost")
    parser.add_argument("--port", type=str,
                        required=False, help="MQTT port name, default 1883", default=1883)
    parser.add_argument("--baud", "-b", type=int,
                        required=False, help="MQTT port name, default 9600", default=9600)
    global args, use_dummy, use_mqtt, verbose, serialPort, host, port, baud, file
    args = parser.parse_args()
    use_mqtt = args.mqtt
    verbose = args.verbose
    serialPort = args.serialport
    host = args.host
    port = args.port
    use_dummy = args.test
    baud = args.baud
    file = args.file


def connect_mqtt():
    client.connect(host, int(port))
    client.loop_start()


def close():
    global client, ser
    if use_mqtt:
        client.loop_stop()
    if serialPort is not None:
        ser.close()


def __dummy__data():
    global __dummy__data__a4
    if use_mqtt:
        __dummy__data__a4 += 0.1
        ba = bytearray(struct.pack("f", __dummy__data__a4))
        client.publish("baja/sensors/0xa4", ba)
        time.sleep(0.1)
        client.publish("baja/sensors/0xa3", ba)
        time.sleep(0.1)
        client.publish("baja/sensors/0xa2", ba)
        time.sleep(0.1)
        client.publish("baja/sensors/0xa1", ba)
        time.sleep(0.1)


def connect_serial():
    if verbose:
        print("Connecting to serial port")
    global ser
    ser = serial.Serial(serialPort, baud)


def proccess_serial():
    global frame, count, state
    b = ser.read()
    if state == "START":
        if b == b'\x00':
            state = "FLAG"

    elif state == "FLAG":
        frame["flag"] = int.from_bytes(b, byteorder='big', signed=False)
        state = "ID"

    elif state == "ID":
        frame["id"] = int.from_bytes(b, byteorder='big', signed=False)
        state = "DATA_LEN"

    elif state == "DATA_LEN":
        frame["len"] = int.from_bytes(b, byteorder='big', signed=False)
        count = 0
        frame["data"] = []
        state = "DATA"

    elif state == "DATA":
        if count >= frame["len"] - 1:
            state = "END"
        frame["data"].append(b[0])
        count = count + 1

    elif state == "END":
        if b == b'\xff':
            state = "START"
            __handle__frame()


def __handle__frame():
    if verbose:
        print(hex(frame["id"]), end=" ")
        print(hex(frame["flag"]), end=" ")
        print(hex(frame["len"]), end=" ")
        for i in frame["data"]:
            print(hex(i), end=" ")
        print()
    # if f is not None:
    #     f.write(hex(frame["id"]), end=" ")
    #     f.write(hex(frame["flag"]), end=" ")
    #     f.write(hex(frame["len"]), end=" ")
    #     for i in frame["data"]:
    #         f.write(hex(i), end=" ")
    #     f.write()

    if use_mqtt:
        client.publish("baja/sensors/" +
                       hex(frame["id"]), payload=bytearray(frame["data"]))


if __name__ == "__main__":
    parse_args()
    if file is not None:
        f = open(file, "w+")
    if use_mqtt:
        connect_mqtt()
    if use_dummy:
        print("Using dummy data")
        while(True):
            __dummy__data()
    if serialPort is not None:
        connect_serial()
        while(True):
            proccess_serial()
    close()
