# [DigoleSerial](http://www.digole.com/index.php?productID=551) with support for SoftwareSerial.

I've been using the Digole serial LCD adapter together with Softwareserial for a while now, so I thought I'd better make my changes public.

#Usage
```
#define _Digole_SoftSerial_UART_
#include <DigoleSerial.h>

....


SoftwareSerial mySerial(10, 11); // RX, TX (only TX will be used)
DigoleSerialDisp mydisp(&mySerial, 9600); 
```

#License
I could not find any license notes on the code from digole, but my changes are public domain
