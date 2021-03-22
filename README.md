# doorbell_clientserver - SMART home without raspberry and arduino ;)
Hear your doorbell (with wagner hammer) on every computer (no raspberry or arduino needed!)

Client is written for Windows 7, but should work with other versions too. Server is written for (MX) linux.

You have no raspberry or arduino but need to digitalize your door-bell with wagner hammer? No problem, just grab the electronics of an old mouse with usb-port (ps/2 might work as well), solder 2 wires on your mouse-button (I choose the right button) and connect that to a relay.

My doorbell has about 12 V AC (alternative current) and I build 2 little boxes with

1) Relay, rectifier and a variable resistor in parallel to the coil to reduce the spikes your relay will get if the coil inside it shuts off. You have to play a little bit with the right resistance of the variable resistor to let the relay work properly after this.
2) Old electronics board from a computer mouse (usb or ps/2) and solder 2 wires on the right button (you can also remove the button, but not ressesary).

If you
This is not needed: I also soldered 2 wires on the left button and 2 wires on the middle button (usually the middle-click from the wheel, works only if your mouse has a wheel and/or middle-button).

## Do this at your own risk and only if you know what you're doing, I'm not responsible for any damages that could happen, doorbell electronics could vary and voltage could vary, measure it first and measure the current your relay gets. 
Then I opened the front cover of the doorbell where the wagner hammer sits and connect my (full-wave) rectifier in parallel with the coil from the wagner hammer.

To be continued...


client_portable.
