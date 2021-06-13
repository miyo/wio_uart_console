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
- put a line string at the cursor
- read 5-way switch
- graphics commands of TFT\_eSPI
  - draw{Line,Rect,RoundRect,Circle,Triangle,Ellipse,String}
  - fill{Rect,RoundRect,Circle,Triangle,Ellipse,String}
- set forground/background color

## Examples

### draw graphics

```
import serial
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.2)

ser.write(b"DL80,100,200,240\n")
ser.write(b"FR50,80,100,80\n")
ser.write(b"DC120,100,50\n")

ser.close()
```

### change screen color

```
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.2)

ser.write(b"SB000F\n")
ser.write(b"C\n") # clear
time.sleep(1)

ser.write(b"SBF81F\n")
ser.write(b"C\n") # clear
time.sleep(1)

ser.write(b"SB7BEF\n")
ser.write(b"C\n") # clear
time.sleep(1)

ser.write(b"SBF800\n")
ser.write(b"C\n") # clear
time.sleep(1)

ser.write(b"SB0000\n")
ser.write(b"C\n") # clear
time.sleep(1)

ser.close()
```

### draw string

```
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.2)

ser.write(b"DS10,Hello World,fefe\n")
ser.write(b"SFF800\n")
ser.write(b"DS11,Hello World,fefe\n")
ser.write(b"SFFFFF\n")

ser.close()
```
