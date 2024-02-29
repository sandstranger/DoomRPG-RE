# DoomRPG-RE - PlayStation Vita port

This is a PS Vita port of the [reverse engineered Doom RPG](https://github.com/Erick194/DoomRPG-RE) by [GEC]. All credits go to the GEC team, this project would not exist without their hard work.

 <img src="https://raw.githubusercontent.com/jakubito/DoomRPG-RE-vita/main/screenshot.png" width="480" />

## How to install

1. Search for "Doom RPG BREW" on [archive.org](https://archive.org/) to get the original mobile game assets file: `doomrpg.bar` (CRC32: d7cf11c5)
1. Next, you need `BarToZip.exe` converter tool (Windows only) included in the [PC release](https://github.com/Erick194/DoomRPG-RE/releases/latest)
1. Drag and drop `doomrpg.bar` onto `BarToZip.exe`. You should now have `DoomRPG.zip` file saved in the same folder
1. Download the [latest VPK release](https://github.com/jakubito/DoomRPG-RE-vita/releases/latest) and install it using VitaShell
1. Copy the newly generated `DoomRPG.zip` to `ux0:data/doomrpg/`
1. You can now start the game

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

All user data is stored in `ux0:data/doomrpg/` - these files are compatible with the PC release.

## Building instructions

This port uses SDL2, ZLIB, SoLoud and fluidsynth-lite

1. Install [Vita SDK](https://vitasdk.org/) and make sure `VITASDK` env variable is set
1. Install [fluidsynth-lite](https://github.com/vitasdk/packages/tree/master/fluidsynth-lite) using `vdpm`. Please refer to "Porting libraries" section on Vita SDK's website for further instructions
1. Run `build.sh` for a debug build, or add `-r` flag for a release build

## Bug reporting

If you find a bug, please send me an e-mail to dobes.jakub@gmail.com or open an issue here on github.

## License

GNU General Public License v3.0
