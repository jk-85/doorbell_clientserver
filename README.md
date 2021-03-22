# doorbell_clientserver
## SMART home without raspberry or arduino
Hear your doorbell sound (with wagner hammer) on every computer (no raspberry or arduino needed!)

![Electronics](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/Electronics.jpg)

More images with explanation of the electronics see folder "sample_pictures".

Client is written for Windows 7, but should work with other versions too. Server is written for (MX) linux.

You have no raspberry or arduino but need to digitalize your door-bell with wagner hammer? No problem, just grab the electronics of an old mouse with usb-port (ps/2 might work as well), solder 2 wires on your mouse-button (I choose the right button) and connect that to a relay.

My doorbell has about 12 V AC (alternative current) and I build 2 plastic little boxes with

1) Relay, rectifier and a variable resistor in parallel to the coil to reduce the spikes your relay will get if the coil inside it shuts off. You have to play a little bit with the right resistance of the variable resistor to let the relay work properly. Somewhere on the net there's a formula, how big this resistor should be.
2) Old electronics board from a computer mouse (usb or ps/2) and solder 2 wires on the right button (you can also remove the button, but not ressesary).

This is not needed: I also soldered 2 wires on the left button and 2 wires on the middle button (usually the middle-click from the wheel, works only if your mouse has a wheel and/or middle-button). Reason was to have some additional self-made "GPIO"-pins (or "GPO"-pins because only reading and NOT writing) for future projects to get more signals for the mouse electronics board.

## Do this at your own risk and only if you know what you're doing, I'm not responsible for any damages that could happen, doorbell electronics could vary and voltage could vary, measure it first and measure the current your relay gets. 
Then I opened the front cover of the doorbell where the wagner hammer sits and connect my (full-wave) rectifier in parallel with the coil from the wagner hammer.

## TCP-Client
Used compiler for windows 7 (could work on other windows platforms too): [Download compiler](http://win-builds.org/doku.php/download_and_installation_from_windows). My used compiler-version was 1.5.0

client.c: Windows client (TCP). Compiled under windows with the following options: "gcc -o client.exe client.c -lws2_32 -lmswsock -ladvapi32 -lwinmm"

## TCP-Server
Used compiler for (MX) linux: gcc

bell_server.c: Linux server (TCP). Compiled with "gcc bell_server.c -o bell_server"

mouse_shared.c: For server. Watches mouse state. Compiled with "gcc mouse_shared.c -o mouse_shared". Has NOT to be startet manually, will be startet automatically.

## Installation
Server: Just compile everything and start "./bell_server" with the optional argument -fakebell if you want to test your bell. That command runs fake bells every 30 seconds and sends it to all connected clients.

I'm unsure bubt I think I used a 24 V relay, because 12 V AC rectified is more than 12 V DC and I think I grabbed a 24 V relay. You can also test the relay with a 9 V battery in good condition and a simple button in series.

Client: Compile client.c and run "client.exe" with the ip-adress of your server, e.g. "client.exe 192.168.1.50"

To be continued...
