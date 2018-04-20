# Poor Man's Spotify

A simple GUI built on top of a music player, much like Spotify or
Windows Groove on your desktop.

### Build

This application require the openFrameworks base and the 'ofxDatGui' addon,
which is the addon from which all the gui elements come from. Failure to do so
will prevent the project from building and running.

### Usage

Usage is intuitive: The GUI presents play/pause buttons for playing and pausing
the song, buttons to select which song you want to play, some sliders to adjust
volume and where in the song you want to be at.

Pressing space will pause/unpause the song, if selecting the pause button is
not as convenient

# Notes

Below are things you must know in order for this program to function smoothly!

### Framework Changes

In ofxDatGuiScrollView.h, it is necessary to adjust the multiplier
on line 256, containing:

```C++
float sy = e.scrollY * 2;
```

The multiplier, set at '2', is much too small, and scrolling becomes abysmally slow.
Unfortunately, there is no simple or nice way to get at this e.scrollY value,
since the implementation details of how it is created and then sent are handled
internally by the ofxDatGui addon. Therefore, it is simpler to just change the source
file as you see fit. My local project has it set to '35', though you may want
to adjust that number to your preference.

### Song Folder

Songs must be stored in a specific `songs` folder, which can be found at
`bin/data/`. If the program has been run once, the folder will have been automatically
created and empty. If not, simply make the folder with the name `songs` at that
location and populate it with whatever songs you need! Most formats
are acceptable, though openFrameworks make a note that mp3 and wav files
are probably the best.

### Slider Values

While tempting, it is best not to select and type in the values in the text
fields associated with sliders. While doing so is totally safe with the volume
slider, there is a huge (and unnecessary) spike in memory usage as a result.
Also, due to the nature of how the position slider's value is being constantly
updated, it is possible to crash the program if you type in *too many decimals
into updating field*. Not only will the values you type in not be visible,
and a simple moving cursor will be the only indicator of you even doing anything,
the potential to crash the program is apparent. If you really need to type
in a value for the position slider, pause the song, then type it in.
