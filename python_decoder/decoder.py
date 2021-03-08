import argparse

verbose = False
use_mqtt = False
host = "localhost"
port = "1883"


def parse_args():
    print("parsing args")
    parser = argparse.ArgumentParser()
    parser.add_argument("serialport", metavar="SerialPort",
                        type=str, help="Serial port")
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Verbose will print everything that is happening")
    parser.add_argument(
        "--mqtt", "-m", action="store_true", help="To use MQTT")
    parser.add_argument("--host", type=str,
                        required=False, help="MQTT host name, default localhost")
    parser.add_argument("--port", type=str,
                        required=False, help="MQTT port name, default 1883")
    args = parser.parse_args()
    print(args)


if __name__ == "__main__":
    print("Starting the serial decorder")
    parse_args()
