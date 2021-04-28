# doorbell_clientserver
## SMART home without raspberry or arduino
Hear your doorbell sound on every computer (no raspberry or arduino needed!)

Shortest explanation: Use an old mouse electronics board, relay and bridge-rectifier to digitalize your doorbell.

![Sample output](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/Sample_Output.jpg)
![Electronics](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/Electronics.jpg)

More images with explanation of the electronics see folder "sample_pictures".

Client is written for Windows 7, but should work with other versions too. Server is written for (MX) linux.

You have no raspberry or arduino but need to digitalize your door-bell? No problem, just grab the electronics of an old mouse with usb-port (ps/2 might work as well), solder 2 wires on your mouse-button (I choose the right button) and connect that to a relay.

My doorbell has about 12 V AC (alternative current) and I build 2 little plastic boxes with

1) Relay, rectifier and a variable resistor in parallel to the coil to reduce the spikes your relay will get if the coil inside it shuts off. You have to play a little bit with the right resistance of the variable resistor to let the relay work properly. Somewhere on the net there's a formula, how big this resistor should be.
2) Old electronics board from a computer mouse (usb or ps/2) and solder 2 wires on the right button (you can also remove the button, but not ressesary).

This is not needed: I also soldered 2 wires on the left button and 2 wires on the middle button (usually the middle-click from the wheel, works only if your mouse has a wheel and/or middle-button). Reason was to have some additional self-made "GPIO"-pins (or "GPO"-pins because only reading and NOT writing) for future projects to get more signals for the mouse electronics board.

## Do this at your own risk and only if you know what you're doing, I'm not responsible for any damages that could happen, doorbell electronics could vary and voltage could vary, measure it first and measure the current your relay gets. 
Then I opened the front cover of the doorbell where the hammer sits and connect my (full-wave) rectifier in parallel with the coil from the hammer.

## TCP-Client
Used compiler for windows 7 (could work on other windows platforms too): [Download compiler](http://win-builds.org/doku.php/download_and_installation_from_windows). My used compiler-version was 1.5.0

client.c: Windows client (TCP). Compiled under windows with the following options:

**gcc client.c -o client.exe -lws2_32 -lmswsock -ladvapi32 -lwinmm**

## TCP-Server
Used compiler for (MX) linux: gcc

bell_server.c: Linux server (TCP). Compiled with

**gcc bell_server.c -o bell_server**

mouse_shared.c: For server. Watches mouse state. Compiled with

**gcc mouse_shared.c -o mouse_shared**

Has NOT to be startet manually, will be startet automatically.

## Installation
Server: Just compile everything and start "./bell_server" with the optional argument -fakebell if you want to test your bell. That command runs fake bells every 30 seconds and sends it to all connected clients. You can also use "fakebell -1" to produce fake bells every second.

I'm unsure bubt I think I used a 24 V relay, because 12 V AC rectified is more than 12 V DC and I think I grabbed a 24 V relay. You can also test the relay with a 9 V battery in good condition and a simple button in series.

Client: Compile client.c and run "client.exe" with the ip-adress of your server, e.g. "client.exe 192.168.1.33"

Please be aware that the right mouse-button will be used on your server. My server runs headless most of the time and the users are logged off, so using that right mouse button isn't a big deal. If you use your workstation where you're logged in all the time this could have silly side effect (e.g. when someone press your doorbell he could make a right click on your computer). Get a clue now, why I use the right mouse button instead of the left? ;)

## Bell sound
You can replace "Klingel.wav" with your own sound. It has to be in the WAV-Format.

## More pictures of the electronics
**You can view big images [here](https://github.com/jk-85/doorbell_clientserver/tree/main/sample_pictures)**

![1](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Complete.JPG)
![2](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification_.JPG)
![3](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification.JPG)
![4](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification2.JPG)
![5](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification3.JPG)
![6](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification4.JPG)
![7](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/small/Doorbell_Modification5.JPG)

## Circuit
![1](https://github.com/jk-85/doorbell_clientserver/blob/main/circuit/circuit.png)

You have to play a little bit with the resistor (that is called "variable resistor" in the sample pictures), I don't know the exact value, there's a formula on the internet what's a good resistor value to reduce the spike the relay coil (and AC bell coil?) produces when they shut off. 
If you want to edit it, circuit was made with https://www.circuit-diagram.org/editor and a svg and cddx file can be found in [this](https://github.com/jk-85/doorbell_clientserver/blob/main/circuit) folder.

## Adding client to Windows (7) autostart

Goto Start menue, input "shell:startup" and paste a link to client.cmd. Edit your client.cmd that it fits your server IP or server name.

You can start your client.cmd minimized with this option:

![1](https://github.com/jk-85/doorbell_clientserver/blob/main/sample_pictures/startup.jpg)

## Updates
- Added a file that will be created after the first pressed doorbell that's called _doorbell_history.txt_. There you can find the exact date and time the doorbell was pressed. Be aware that your date and time on your machine has to be accurate to get proper results.
- Added the option -fakebell 1 (the 1 is new) to hit a fake bell every second.
- 03/25/2021: Huge CPU leak fixed
- 04/27/2021: Bugfixes, mainly in client

If you have a question, you can e-mail me at tour2002 [AT] arcor [dot] de
