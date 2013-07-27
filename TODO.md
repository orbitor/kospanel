
Extended Window Manager Support
================================

fspanel had rudimentary EWMH support, but it is not implemented properly.
In particular, the window type and properties, and strut need to be
set so the window manager understands that this is a panel (toolbar)
and no longer covers it up.

Xinerama Support
=================

linux.die.net/man/3/xineramaisactive

Instead of having just one panel which covers all of the screens, have
a separate panel per screen and only show the windows in the panel
which are actually on that screen?

How would that work with a pager?

It would probably be pretty simple to have a separate gui representation
for each panel and pass 'tasks' between the panels as the windows are
moved around on the screen. (The task passing might work something like
how panel changes are handled.)

How about drag-n-drop tasks from one panel to another? Probably alot of
work for not much benefit.

The take away from here is that it would be nice to have the panel
*not* span monitors, and however it has to work, to draw separate panels
on each monitor and have only the tasks on that monitor drawing on that
panel.


Task Grouping
===============

When the panel becomes crowded, group similar tasks together into 1 task
button on the panel. But, what happens when the user selects the button?
For everything I dislike about Windows, in Windows 7 Microsoft did a nifty
thing in drawing thumbnails of the grouped windows. OSX does a decent
job of it, too, by putting the title text in a menu.

Probably a good first step would be to test grouping (as in, always on),
and come up with a menu with title text when the task button is clicked.
The thumbnail-hover would really be some work and drag in alot of extra
libraries to do the imaging. Definitely would have to be a compile
time option.

Anti-aliased Text and UTF-8 Support
====================================

This is the first easiest thing to do. Just replace the X text drawing and
use the Xft drawing. This should fix the font rendering completely.

