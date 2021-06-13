# wio_uart_console

A simple console(28x15) on WIO-Terminal controlled via USB-UART.

```
import serial
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.2)

ser.write(b"C\n") # 'C': clear screen

ser.write(b"Whello\n") # 'W': write message

for i in range(20):
	s = str(i)
	d = "Wval("+s+")\n"
	ser.write(d.encode('utf-8')) # 'W': write message

ser.close()
```

## Compile Environment

- Seeed SAMD Boards by Seeed Studio version 1.8.1
- Arduino 1.8.13

## Support

- clear screen
- write a line string
- read 5-way switch
