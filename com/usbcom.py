
import serial
serial_port = serial.Serial( port="/dev/ttyACM0", baudrate=9600, timeout=1 )
while True:
    serial_port.write("s123")
    print hex(ord(serial_port.read()))
