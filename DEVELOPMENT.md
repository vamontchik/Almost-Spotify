# April 20, 2018

By now, most of the GUI elements have been put into place, and a nice
encapsulation of the ofSoundPlayer class that openFrameworks provides
has been made in order to ease the usage of such a class. The GUI consists
of a scrolling view, a few text panels, and a few sliders are in place to
allow the basic functionality of what a "Music Player" would look like.
Features relating to the songs are still not implemented, and may not be due
to limitations of openFrameworks.

Mostly, issues revolving around this project have not been due to its
complexity, but due to the failure of openFrameworks to provide
simple, consistent, or even reachable values that greatly impact the
user's experience.

For example, it is not possible to stop certain GUI elements from updating
due to an automatic (and internal) call to their update() method. Why this is the
case is beyond me, but it prevents one from deleting GUI elements and replacing
them with a modified copy because some of these said GUI elements do not have
a clear() method to remove their contents. Similar issues crop up all the time,
and very "hacky" solution have to be made in order to allow things to appear
to work in a nice manner.
