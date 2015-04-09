# Penguin Chip 8 Emulator

![Penguin](https://dl.dropboxusercontent.com/u/60223805/penguin.png)

Penguin is a very simple Chip 8 emulator/interpreter programmed in the C language.
The goal with penguin is to support almost any platform smoothly.

## Planed Features

* Support for all 35 opcodes
* No platform-specific/graphics code except in ```main.c``` (Should be runnable on almost every platform)

### Current Version

* Supports almost every opcode perfectly

![Penguin Showcase](https://dl.dropboxusercontent.com/u/60223805/showcase.png)

#### Bugs/Unimplemented features

**bug** Creates an infinite loop on some games (PONG, PONG2) (probably due to error in opcode(s)) **FIXED!**

*unimplemented feature* No sound

##### Purpose

I made this emulator for learning/educational purposes only. I wanted to get more familiar with emulation and the C language. 

Software is provided as is.

####### Documentations/Help Used

* http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4

*Thanks a ton to you guys!*
