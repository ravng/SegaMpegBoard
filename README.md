# SegaMpegBoard!
![board](https://user-images.githubusercontent.com/66999751/209871745-0dc552ad-267b-4ff6-9a17-deaa2952572c.png "Mpeg Replacement")

Drop in replacement board for Sega Model 3 Mpeg board.
Tested on Scud Race but can't see why it would not work in other aracdes with the same mpeg board.

It uses the cheap DFPlayer MP3 module for soundtrack files and attiny84 for the communication.

A jumper is used to select different soundtracks (folders). 
Folder 1-4 plays the same sound track for each race track every time, while folder 5 will play songs in random order. 
If no jumper installed it will use folder "01", see source for details. 


IMPORTANT NOTE!
The attiny might need a clock calibration to work. 
I supposed it would be smart to add an external clock, but I didn't.
Fell free to modify the design yourself or search the web for attiny clock calibration.
