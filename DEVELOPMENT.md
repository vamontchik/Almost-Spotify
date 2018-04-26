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

# April 26th, 2018

Internally, the code has been run/tested thoroughly and has undergone fine
tuning. Most of the changes of the last week have been made to revamp
how the internals of the system work.

The GUI now also presents a few additional elements, such as the
right and left buttons to go between songs. Also, each song now
displays some key statistics about it, such as its length, where it
is in the song in a readable hh:mm:ss or mm:ss format, and its size
with the correct prefix attached to the word "byte".
