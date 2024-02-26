# DoomRPG-RE - PlayStation Vita port (WIP)

![Screenshot](https://raw.githubusercontent.com/jakubito/DoomRPG-RE-vita/main/screenshot.png)

This is a PS Vita port of the reverse engineered [Doom RPG by [GEC]](https://github.com/Erick194/DoomRPG-RE). All credits go to the original team, this project would not exist without their hard work.

## How to install

1. The original mobile phone game assets are required to run this port. Search for "Doom RPG BREW" on archive.org, the file you need is `doomrpg.bar` (CRC32: d7cf11c5)
1. Next, you need `BarToZip.exe` converter tool (Windows only) included in [the PC release](https://github.com/Erick194/DoomRPG-RE/releases/latest)
1. Drag and drop `doomrpg.bar` onto `BarToZip.exe`. You should now have `DoomRPG.zip` file saved in the same folder
1. Download [the latest Vita VPK release](https://github.com/jakubito/DoomRPG-RE-vita/releases/latest) and install it using VitaShell
1. Copy the newly generated `DoomRPG.zip` to `ux0:data/doomrpg/`
1. You can now run the game

## Default controls

| Action           | Button      |
| ---------------  | ----------- |
| Move Forward     | D-pad up    |
| Move Backward    | D-pad down  |
| Move Left        | L           |
| Move Right       | R           |
| Turn Left        | D-pad left  |
| Turn Right       | D-pad right |
| Attack/Talk/Use  | X           |
| Next Weapon      | Triangle    |
| Prev Weapon      | Circle      |
| Pass Turn        | Square      |
| Automap          | Select      |
| Menu Open/Back   | Start       |

## Save and config data

All user data is stored in `ux0:data/doomrpg/`. These files are compatible with the PC release.

## Building instructions

This port uses SDL2, ZLIB, SoLoud and fluidsynth-lite

1. Install [Vita SDK](https://vitasdk.org/)
1. Install [fluidsynth-lite](https://github.com/vitasdk/packages/tree/master/fluidsynth-lite) using `vdpm`. Please refer to "Porting libraries" section on Vita SDK's website for further instructions
1. Run `build.sh` for a debug build or add `-r` flag for a release build. The output VPK is saved as `build/DoomRPG_vita.vpk`

## Bug reporting

If you find a bug, please send me an e-mail to dobes.jakub@gmail.com or open an issue here on github.

## License

GPL v3
