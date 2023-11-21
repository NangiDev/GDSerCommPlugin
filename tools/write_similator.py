import serial
import time

# Replace '/dev/pts/X' with the actual path of your serial port
ser = serial.Serial('/dev/pts/2', baudrate=9600, timeout=1)

try:
    while True:
        # Send data
        data_to_send = b'Hello, world!'
        ser.write(data_to_send)
        print("Sent:", data_to_send.decode('utf-8'))

        # Add a delay if needed
        time.sleep(1)

except KeyboardInterrupt:
    # Handle Ctrl+C to gracefully exit the loop
    pass
finally:
    ser.close()
