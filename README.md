# International Doom for DOS

This is a sub-project of the main project [Russian Doom](https://github.com/Russian-Doom/russian-doom). 


## Russian Doom for DOS 1.8 (released: June 9, 2020)

**DOS**:
[russian-doom-dos-1.8.zip](https://github.com/Russian-Doom/russian-doom-dos/releases/download/1.8/russian-doom-dos-1.8.zip)  
Compilation from source code available, see [COMPILING.md](COMPILING.md).  
Source code available in [zip](https://github.com/Russian-Doom/russian-doom-dos/archive/refs/tags/1.8.zip)
and [tar.gz](https://github.com/Russian-Doom/russian-doom-dos/archive/refs/tags/1.8.tar.gz) formats


## Supported games

* Doom shareware
* Doom registered
* The Ultimate Doom
* Doom II: Hell on Earth
* Final Doom - TNT: Evilution
* Final Doom - The Plutonia Experiment
* SIGIL (five episodes version)

If you have multiple IWADs in your directory, you can use the `-iwad` parameter to specify which IWAD to use. 


## New command line parameters

`-vanilla`<br />
Enables "vanilla" mode, that will turn off almost all improvements like colored blood, shadowed text and other.
Note: it does affect increased engine limits and bug fixes.

`-mb`<br />
Define amount of memory for game to use. Minimum is 2 MB, maximum is 64 MB.

`-dm3`<br />
Enables newschool deathmatch (Deathmatch 3.0) mode.
Weapons will stay after picking up, but items and powerups will dissapear, and will respawn after 30 seconds.


## Gravis UltraSound

Gravis UltraSound sound synth requires a GUS patches to be installed.
Please follow this [instruction](https://github.com/JNechaevsky/inter-doom/blob/master/src_dos/ultrasnd/README.md)
to setup them in DOSBox.


## License

DOS version is based on [PCDoom](https://github.com/nukeykt/PCDoom-v2) by Alexey Khokholov
and [Doom Vanille](https://github.com/AXDOOMER/doom-vanille) by Alexandre-Xavier Labonté-Lamoureux,
with significant additions from [Crispy Doom](http://fabiangreffrath.github.io/crispy-doom) by Fabian Greffrath
and [Doom Retro](http://doomretro.com) by Brad Harding, and is distributed with their approval.

The project, along with all its translation materials and source code,
is available under the terms and conditions of
[GNU General Public License version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).

Copyright &copy; 2017&ndash;2022 [Julia Nechaevskaya](https://jnechaevsky.github.io/author.html)  
Copyright &copy; 2023 [Leonid Murin (Dasperal)](https://github.com/Dasperal)