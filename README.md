# Deivox

**Deivox is a three-dimensional falling sand simulator.** Although pretty simple for now, the idea is to turn it into a full falling sand framework where you can freely add new blocks, and maybe eventually grow it into a sort of 3D WorldBox.

I really like WorldBox, PowderToy and similar falling sand sandbox games, but I always wondered if it could step out from the second dimension and become a 3D game. Turns out that making a 3D falling sand game comes with a lot more hurdles than you would expect,
including some that I *still* don't know how to do (like interaction with the world: How do you have a cursor in a 3D grid????). <br> However it's already a pretty fun demo and I think it has the potential to become a really fun sandbox framework :D


### Features
Features for now:
- Different blocks
- Falling sand physics
- 3D cursor

But these features are coming in the near future:
- JSON-based tile definition
- Other tile types (water, maybe even smoke?)

<br>

## How to build
If you have G++ set up on your computer, than building the project should be as simple as just typing in `make`. If you use Windows, you should use the `Makefile` while on Linux you should use `Makefile.linux`.

<br>

## How to use

On Windows:
-  Download the newest release
-  Unzip
-  Run `deivox.exe`.

On Linux: **Linux build coming as soon as I have time to upload and properly test it!**
- Download the newest Linux release 
- Unzip
- Run `deivox`.
