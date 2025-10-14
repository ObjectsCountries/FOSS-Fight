# Contributing

## \*.ff files

\*.ff files are how FOSS Fight character data is stored. The header is `F0 55`, which represents "FOSS".

The first two bytes detailed describe the number of different palettes. Then, the number of different colors is detailed. Afterward, groups of 3 bytes serve to represent each color. The next same-amount of groups of 3 bytes are the alternative color. More colors will be added as development continues.

Then, floating point numbers are detailed that represent stats, in this order:

1. Size
2. Walking Forward Speed
3. Walking Backward Speed
4. Forward Jump X-Velocity
5. Backward Jump X-Velocity
6. Initial Jump Velocity
7. Gravity

Then, the character sprite animations are described. Each animation begins with two bytes describing the type of animation it is.

| Number  | Data                      |
|:-------:|:--------------------------|
| `00 00` | Idle                      |
| `00 01` | Walking Forward           |
| `00 02` | Walking Backwards         |
| `00 03` | Crouch Transition         |
| `00 04` | Crouching                 |
| `00 05` | Stand Blocking            |
| `00 06` | Crouch Blocking           |
| `00 07` | Pre-Jump                  |
| `00 08` | Jumping Forwards          |
| `00 09` | Jumping Neutral           |
| `00 0A` | Jumping Backwards         |
| `00 0B` | Stand Getting Hit         |
| `00 0C` | Crouch Getting Hit        |
| `00 0D` | Air Getting Hit           |
| `00 0E` | Air Reset                 |
| `00 0F` | Knockdown                 |
| `00 10` | Getting Up                |
| `00 11` | Victory Animation         |
| `00 12` | Time Over Loss Animation  |
| `01 00` | Standing Light Punch      |
| `01 01` | Standing Heavy Punch      |
| `01 02` | Standing Light Kick       |
| `01 03` | Standing Heavy Kick       |
| `01 10` | Crouching Light Punch     |
| `01 11` | Crouching Heavy Punch     |
| `01 12` | Crouching Light Kick      |
| `01 13` | Crouching Heavy Kick      |
| `01 20` | Jumping Light Punch       |
| `01 21` | Jumping Heavy Punch       |
| `01 22` | Jumping Light Kick        |
| `01 23` | Jumping Heavy Kick        |
| `01 04` | Forward Light Kick        |
| `01 80` | Forward Throw             |
| `01 81` | Backwards Throw           |
| `02 00` | Special 1                 |
| `02 01` | Special 2                 |
| `02 02` | Special 3                 |
| `02 03` | Special 4                 |
| `02 04` | Special 5                 |
| `02 05` | Special 6                 |
| `02 06` | Special 7                 |
| `02 07` | Special 8                 |
| `02 08` | Special 9                 |
| `02 09` | Special 10                |
| `03 00` | Super                     |
| `04 00` | Misc. Asset 1             |
| `04 01` | Misc. Asset 2             |
| `04 02` | Misc. Asset 3             |
| `04 03` | Misc. Asset 4             |
| `04 04` | Misc. Asset 5             |
| `04 05` | Misc. Asset 6             |
| `04 06` | Misc. Asset 7             |
| `04 07` | Misc. Asset 8             |
| `04 08` | Misc. Asset 9             |
| `04 09` | Misc. Asset 10            |
| `05 00` | Character Meter Asset 1   |
| `05 01` | Character Meter Asset 2   |
| `05 02` | Character Meter Asset 3   |
| `05 03` | Character Meter Asset 4   |
| `05 04` | Character Meter Asset 5   |
| `05 05` | Character Meter Asset 6   |
| `05 06` | Character Meter Asset 7   |
| `05 07` | Character Meter Asset 8   |
| `05 08` | Character Meter Asset 9   |
| `05 09` | Character Meter Asset 10  |
| `FF 00` | Character Selection Image |
| `FF 01` | Post-Game Win Image       |
| `FF 02` | Post-Game Loss Image      |

For Gnu, these describe his Footsies mode sprites. If a pair of bytes begins with `10`, `11`, etc., these are his Gatling mode sprites.

After these two bytes, another two bytes indicate how many different sprites this animation consists of.

For each sprite, the first pair of bytes is how many frames (1/60 of a second) to display it for. Then, the next 4 pairs of bytes are the location of the asset on the sprite sheet, in order of leftmost pixel, uppermost pixel, width and height. Then, two pairs of bytes describe a `signed short` of a visual offset (horizontal and vertical, respectively). Then, the boxes are described.

| Number  | Box Type                                |
|:-------:|:----------------------------------------|
| `00 00` | Null Terminator                         |
| `00 01` | Hurtbox                                 |
| `00 02` | Grab and Command Grab Box               |
| `00 03` | Throwbox, Push Box and Ground Collision |
| `00 04` | Proximity Guard Box                     |
| `01 00` | Non-Cancelable Hitbox                   |
| `01 01` | Special-Cancelable Hitbox               |
| `01 02` | Super-Cancelable Hitbox                 |
| `01 03` | Special- and Super-Cancelable Hitbox    |

If a sprite's length begins with `FF`, that means that it is copying another sprite's data. The byte after `FF` determines which information to copy (most significant bit first). In order, the bits are:

1. Sprite Length
2. Sprite Sheet Location
3. Offset
4. Hurtboxes
5. Grab and Command Grab Boxes
6. Throwboxes, Push Boxes and Ground Collision Boxes
7. Proximity Guard Boxes
8. Hitboxes

A value of 1 means to copy this information, and 0 means to define it explicitly. The definitions are implicitly ordered.  

The first two bytes afterward represent the animation type to copy from, and the next two bytes are the sprite index (starting from 0). 

If a box's coordinates are `FF FF`, that means that it covers the entire asset as one whole rectangle.

### Example

(Note to self) To format data:

1. Find `/(([0-9A-F]{2} ){16})/`, replace with `/\1\n/`.
2. Find `/ $/`, replace with `//` (delete).

```hexdump
F0 55 00 03 00 01 00 00 00 FF 00 00 00 00 FF 40
80 00 00 40 80 00 00 C0 40 00 00 41 00 00 00 C1
00 00 00 C1 A0 00 00 3F 80 00 00 00 00 00 04 00
05 00 09 00 08 00 17 00 2A 00 00 00 00 00 01 00
01 00 00 00 00 00 1A 00 2A 00 03 00 01 00 00 00
20 00 1A 00 0A 00 00 FF BF 00 00 00 00 00 2D 00
09 00 19 00 29 FF BF 00 00 00 00 00 55 00 09 00
1A 00 2A FF FF 00 00 00 01
```

The file begins with the header of `F0 55`.

Then, it describes 3 color palettes, each with one color. The colors of #000000 for the first, #FF0000 for the second and #0000FF for the third are used.

The next 7 groups of 4 bytes represent floating point numbers of the stats described near the top of this document, ending after `3F 80 00 00`.

Then, animation `00 00` (idle) is described, consisting of 4 sprites. The first sprite has a length of 5 frames. The next 4 pairs of bytes describe its location and dimensions on the sprite sheet. The `00 00 00 00` afterward indicates that it has 0 horizontal offset, and 0 vertical offset.

`00 01` begins to describe the sprite's hurtboxes, with the `00 01` afterward indicating that there is one hurtbox. This hurtbox's coordinates are detailed, then the same is done for the throw/push/ground collision box. The `00 00` afterward indicates the end of box description.

`FF BF` indicates that this frame copies from another frame. `BF` in binary is `10111111`, meaning that this sprite copies everything but the sprite sheet location. `00 00` indicates that it will take from the idle animation, and the next `00 00` indicates that it will take from the first sprite. The sprite sheet location is then explicitly described. The third frame takes the same action, but describes a different sprite sheet location.

Finally, `FF FF 00 00 00 01` means to make an exact copy of the second sprite of the idle animation, including the sprite sheet location.

## To Do

* Add horizontal walls