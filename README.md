# Monster Defense

An old game project of mine from 2011-2012. Was intended to be a kind of
action-y shoot-em-up / bullet-hell / arena type game where the idea was
to survive as long as you could against an onslaught of monsters in different
arena-style levels with different types of hazards and other traps and a
variety of enemies.

Like many of my game projects, I never finished it. I only ever used
placeholder artwork, never did any audio work whatsoever and only "designed"
a single level (purely for testing purposes, of course). The gameplay is
rather un-refined and dull after about the first minute of playing (at least,
that is my feeling playing this for the first time in about 6 years at the
time of writing this). Of course, in my defense, the game was never finished.
Perhaps if I'd kept working on it, it would've developed into something a
little bit more fun. Meh.

![Title Screen](https://github.com/gered/MonsterDefense/raw/master/screenshots/titlescreen.png "Title Screen")
![In-game](https://github.com/gered/MonsterDefense/raw/master/screenshots/ingame.png "In-game")

As mentioned above, all artwork was placeholder. All character models were
borrowed from Egoboo (you can find the [original assets here](https://github.com/egoboo/egoboo-assets)).
Actually the fact that I left this project with placeholder character models
from Egoboo is precisely what stopped me from uploading this code previously.
But I just noticed today on a whim that those assets are released under a GPL3
license ... so, hooray, I feel OK sharing all this now.

I think the original MD2 model files were converted to my custom format via
my [Md2ToMesh](https://github.com/gered/Md2ToMesh) tool, but it's possible I was using some customized/tweaked
version of it and have now forgotten.

The UI is implemented via [GWEN](https://github.com/garrynewman/GWEN). The
UI skin is the default GWEN skin (at least at the time... I think the default
skin for it is a bit different now maybe).

Building this ... Back in 2011/2012 I know I somewhat regularly tested
builds on Windows 7, Gentoo Linux and Mac OS X. I primarily developed this
using Mac OS with Xcode. It depends on libSDL 1.2.x. If you have an old
version of the now defunct Marmalade SDK (previously Airplay SDK), then
you can even build a mobile version that will work on Android and iOS
(well, an old version of iOS... the Android build still works on my Google
Pixel albeit after clicking past a warning about the CPU being unsupported).

I was able to build this today on Mac OS 10.13.6 after having installed
the SDL.Framework (for libSDL 1.2.15) under /Library/Frameworks building
via Makefile with LLVM installed via the Xcode Command Line Tools package.
You can generate a Makefile for this by installing [Premake 4.4-beta5](https://premake.github.io/)
and then running the included `generate_makefile` script.

Somewhat similarly, I was able to also build this on Gentoo Linux today
as well once libSDL 1.2.15 and premake 4.4-beta5 were installed, by using a
Makefile generated via the same `generate_makefile` script.

I assume this would still build fine on Windows using VC++ 2010. Probably
also could build on MinGW.

Because of the age of the included version of EASTL, I had to tweak 
`lib/eastl/include/EABase/eabase.h` a bit (in a rather poor way mind you)
by commenting out a couple typedefs to resolve compile errors. Though,
now that I think about it, EASTL was not a requirement and I did include a
method to switch between it and the normal C++ STL (see `lib/stl/include/stlconfig/switchstl.h`).
Meh.

There's plenty of bad code in here, I'm not going to spend any time here
dissecting it. We've all written klugdey crap before. :-)

This code is using my [C/C++ framework I'd previously opensourced](https://github.com/gered/MyGameFramework).
I think that code is slightly more up to date then this, but I could be wrong.
This definitely does have some stuff that that other framework repository
doesn't have anyway.

Enjoy!
