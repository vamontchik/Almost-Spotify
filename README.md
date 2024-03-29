# Almost Spotify

A simple GUI built on top of a music player, much like Spotify or
Windows Groove on your desktop.

### Build

This application requires the openFrameworks base and the ofxDatGui add-on,
which is the add-on from which all the GUI elements come from. Failure to do so
will prevent the project from building and running.

### Usage

Usage is intuitive: The GUI presents play/pause buttons for playing and pausing
the song, buttons and a scroll view to select which song you want to play, and
some sliders to adjust volume and where in the song you want to be at.
You can also go back and forth between songs by pressing the right/left buttons.
A shuffle mode toggle button exists as well, which modifies the behavior
of updating to the next song and how the left/right buttons behave.

Keyboard shortcuts:
- Pressing the spacebar will pause/unpause the song.
- Pressing ESC will exit the program.

# Notes

Below are things you must know in order for this program to function smoothly!

### Framework Changes

In ofxDatGuiScrollView.h, it is necessary to adjust the multiplier
on line 256, containing:

```C++
float sy = e.scrollY * 2;
```

The multiplier, set at '2', is much too small, and scrolling becomes abysmally slow.
Unfortunately, there is no simple or nice way to get at `e.scrollY`, since the
implementation details are hidden with the add-on itself. Therefore, it is simpler
to just change the source file as you see fit. My local project has it set to '35',
though you may want to adjust that number to your preference.

### Song Folder

Songs must be stored in a specific `songs` folder, which can be found at
`bin/data/`. If the program has been run once, the folder will have been automatically
created . If not, simply make the folder with the name `songs` at that
location and populate it with whatever songs you need! Most formats
are acceptable, though openFrameworks specifies that mp3 and wav files
are probably the best.

### Slider Values

While tempting, it is best not to select and type in the values in the text
fields associated with sliders. While doing so is totally safe with the volume
slider, there is a huge (and unnecessary) spike in memory usage as a result.
Also, due to the nature of how the song position slider's value is being constantly
updated, it is possible to crash the program! If one is to type in a value into
the song position slider's text field, not only will the values you type in not be visible
and a simple moving cursor will be the only indicator of you even doing anything,
but you will also **crash the program if you type in too many values**.
