import serial

# Replace '/dev/pts/Y' with the actual path of your serial port
ser = serial.Serial('COM4', baudrate=9600, timeout=1)

try:
    while True:
        # Read data
        received_data = ser.readline()
        print("Received:", received_data.decode('utf-8'))
except UnicodeDecodeError:
    print("Error decoding data. Check if the data is valid UTF-8.")
except KeyboardInterrupt:
    # Handle Ctrl+C to gracefully exit the loop
    pass
finally:
    ser.close()
