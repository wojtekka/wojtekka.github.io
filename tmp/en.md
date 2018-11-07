/tmp - tiny/tani mp3 player ([🇵🇱](pl.html) [🇬🇧](en.html))
=============================================================

Project goals
-------------

To build small, simple and as cheap as possible portable MP3 player.
Why buy something when you can do it yourself? Why assemble
[someone else's](http://www.yampp.com/) project when you can have
your own? By the way, you can learn many useful things. The word _tani_ in
the name means _cheap_ in Polish.</p>

![Picture of /tmp](foto1.jpg)

What's new?
-----------

* 2005-03-03: Information about interferences and the workaround.
* 2005-01-23: Added implementation details and pictures of the project.
* 2005-01-23: Updated firmware has been put on the site.
* 2005-01-15: The project has been finished. The device has been assembled, it works and at last I have my own MP3 player.
* 2004-08-06: First English version.

Hardware
--------

* MCU: [AT90S2313](http://www.atmel.com/dyn/products/product_card.asp?part_id=1993). It costs about $3 in Poland, it's small, works fine at 3V (even the non-_L_ version), easy to program with gcc port. Its 15 I/O pins are enough for memory card, MP3 decoder and some buttons. It will handle 128 kbps at 6 MHz.
* MP3 decoder: [VS1001K](http://www.vlsi.fi/vs1001/vs1001.htm). The most expensive part of the project, but it seems that this is the most popular and the most available decoder chip. Besides, it doesn't require external D/A nor headphone amplifier.
* Flash card: [MMC](http://www.mmca.org/). It can be handled with only 4 I/O pins.

Software and docs
-----------------

* avr-gcc.
* MMC flash card docs from [SanDisk](http://www.sandisk.com/) and [Hitachi](http://www.hitachi.com/).
* VS1001K docs.
* In crisis situations I've looked at [YAMPP-7](http://www.yampp.com/) source code.

Cons
----

* No LCD. Most of the time, the player will be in my pocket or backpack, so I won't use it anyway. Walkmans don't show current time and no one complains. I don't need ID3 because I know what I have uploaded to the card.
* No PC connectivity. MCU's flash memory is too small to fit USB and MMC write support and another chip (USB inteface) and a socket would drastically increase player size. Currently the cheapest MMC card readers cost about $10, so it isn't an issue. Besides, if someone has a digital camera with MMC/SD cards, he already has a MMC reader (like I do.)
* No fast-forward or rewind. Like in some CD-ROM drives, there are only two buttons: ⏹ stopping the playback and going back to the beginning of the playlist and ⏵/⏭ starting playback or skipping to the next track.

Pros
----

* It's very simple. The PCB is about 2"x1". Most of the parts can be bought in any electronics store. I bought a MP3 decoder in [Jelu Handelsbolag](http://www.jelu.se/) in Sweden.
* It supports FAT12 and FAT16 filesystems not some own like some DIY MP3 players. That's why you can upload files using any MMC reader under any operating system.
* Its software is available under the terms of GPL.

Implementation details
----------------------

* MCU is run at 6 MHz because it's the lowest frequency at which 128 kbps files are played correctly. If you want to play higher quality files at the cost of higher power consumption, you should use faster crystal.
* After power-up the MCU goes to idle mode so it consumes minimal amount of energy. Play button is connected to external interrupt pin and pressing it wakes up the MCU and starts the playback. When playing, the forward button is polled by software. Stop button is connected to reset input so it doesn't have to be checked by software.
* Because of troubles with getting 12.288 MHz crystal, 12 MHz has been used.  The difference in playback speed is about 2% to it's barely noticable.
* <span style="color: red;">Ground and power wires on the first revision of the board are are too thin and it causes audible interferences. The workaround it to connect MCU ground with battery pad with a thick wire as seen on the prototype image.</span>

Schematic
---------

![Schematic](tmp.sch.png)

Board
-----

![Board](tmp.brd.png)

Pictures of prototype
---------------------

![Picture 1](foto2.jpg) ![Picture 2](foto3.jpg)

Firmware
--------

Available at [project page](https://github.com/wojtekka/tmp/releases/)

