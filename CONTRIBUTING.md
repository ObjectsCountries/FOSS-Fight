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

| Number  | Data                            |
|:-------:|:--------------------------------|
| `00 00` | Idle                            |
| `00 01` | Walking Forward                 |
| `00 02` | Walking Backward                |
| `00 03` | Crouch Transition               |
| `00 04` | Crouching                       |
| `00 05` | Stand Blocking                  |
| `00 06` | Crouch Blocking                 |
| `00 07` | Pre-Jump                        |
| `00 08` | Jumping Forwards                |
| `00 09` | Jumping Neutral                 |
| `00 0A` | Jumping Backwards               |
| `00 0B` | Stand Getting Hit               |
| `00 0C` | Crouch Getting Hit              |
| `00 0D` | Air Getting Hit                 |
| `00 0E` | Air Reset                       |
| `00 0F` | Knockdown                       |
| `00 10` | Getting Up                      |
| `00 11` | Victory Animation               |
| `00 12` | Time Over Loss Animation        |
| `01 00` | Standing Light Punch            |
| `01 01` | Standing Heavy Punch            |
| `01 02` | Standing Light Kick             |
| `01 03` | Standing Heavy Kick             |
| `01 04` | Forward Light Kick              |
| `01 10` | Crouching Light Punch           |
| `01 11` | Crouching Heavy Punch           |
| `01 12` | Crouching Light Kick            |
| `01 13` | Crouching Heavy Kick            |
| `01 20` | Jumping Light Punch             |
| `01 21` | Jumping Heavy Punch             |
| `01 22` | Jumping Light Kick              |
| `01 23` | Jumping Heavy Kick              |
| `01 XX` | Other Command Normals           |
| `01 80` | Forward Throw                   |
| `01 81` | Backwards Throw                 |
| `02 XX` | Special Move                    |
| `03 00` | Super                           |
| `F0 XX` | Character-Specific Meter Assets |
| `F1 XX` | Miscellaneous Assets            |
| `FF 00` | Character Selection Image       |
| `FF 01` | Post-Game Win Image             |
| `FF 02` | Post-Game Loss Image            |
| `FF FF` | Nothing                         |

For Gnu, these describe his Footsies mode sprites. If a pair of bytes begins with `10`, `11`, etc., these are his Gatling mode sprites.

After these two bytes, another two bytes indicate how many different sprites this animation consists of.

For each sprite, the first pair of bytes is how many frames (1/60 of a second) to display it for. Then, the next 4 pairs of bytes are the location of the asset on the sprite sheet, in order of leftmost pixel, uppermost pixel, width and height. Then, two pairs of bytes describe a `signed short` of a visual offset (horizontal and vertical, respectively). Then, the boxes are described.

| Number  | Box Type                                |
|:-------:|:----------------------------------------|
| `00 00` | Null Terminator                         |
| `00 01` | Hurtbox                                 |
| `00 02` | Command Grab Box                        |
| `00 03` | Throwbox, Push Box and Ground Collision |
| `00 04` | Proximity Guard Box                     |
| `01 XX` | Hitbox                                  |

The `XX` byte of a hitbox describes its properties. Each bit describes the following (MSB first):

1. Can be blocked high
2. Can be blocked low
3. Special-Cancelable
4. Super-Cancelable
5. `00` for mild knockback (1 sprite of reel-back), `01` for medium (2 sprites), `10` for heavy (3 sprites), and `11` for a knockdown
6. See above
7. Soft knockdown (0) or hard knockdown (1), ignored if no knockdown
8. Resets when hitting airborne opponent

After the coordinates of a hitbox, four numbers are detailed: the hit stun in frames (unsigned), the block stun in frames (unsigned), the pushback on hit in units (signed), and the pushback on block in units (signed). Note that the hit stun and pushback on hit are instead read as the x-velocity and y-velocity of the knockback if the hitbox knocks down.

If a sprite's length begins with `FF`, that means that it is copying another sprite's data. The byte after `FF` determines which information to copy (most significant bit first). In order, the bits are:

1. Sprite Length
2. Sprite Sheet Location
3. Offset
4. Hurtboxes
5. Command Grab Boxes
6. Throwboxes, Push Boxes and Ground Collision Boxes
7. Proximity Guard Boxes
8. Hitboxes

A value of 1 means to copy this information, and 0 means to define it explicitly. The definitions are implicitly ordered.  

The first two bytes afterward represent the animation type to copy from, and the next two bytes are the sprite index (starting from 0). 

**(TO BE IMPLEMENTED)** If a box's coordinates are `FF FF`, that means that it covers the entire asset as one whole rectangle.

### Example

```
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

A length of `FF BF` indicates that this frame copies from another frame. `BF` in binary is `10111111`, meaning that this sprite copies everything but the sprite sheet location. `00 00` indicates that it will take from the idle animation, and the next `00 00` indicates that it will take from the first sprite. The sprite sheet location is then explicitly described. The third frame takes the same action, but describes a different sprite sheet location.

Finally, `FF FF 00 00 00 01` means to make an exact copy of the second sprite of the idle animation, including the sprite sheet location.

After this, the walking forward animation is described (not shown).

There must be a gap of at least one frame between hitboxes for multi-hits/sour spots.

## Special Moves

Immediately before the hitboxes of a special move or Super, its input and buttons are described with one byte.

|  Bits  | Command Input                           |
|:------:|:----------------------------------------|
| `0000` | Quarter Circle Forward (236)            |
| `0001` | Quarter Circle Back (214)               |
| `0010` | Half Circle Forward (41236)             |
| `0011` | Half Circle Back (63214)                |
| `0100` | Full Circle (360)                       |
| `0101` | Horizontal Charge (\[4]6)               |
| `0110` | Vertical Charge (\[2]8)                 |
| `0111` | Dragon Punch (623)                      |
| `1000` | Double Quarter Circle Forward (236236)  |
| `1001` | Double Quarter Circle Back (214214)     |
| `1010` | Double Half Circle Forward (4123641236) |
| `1011` | Double Half Circle Back (6321463214)    |
| `1100` | Double Full Circle (720)                |
| `1101` | Double Horizontal Charge (\[4]646)      |
| `1110` | Double Diagonal Charge (\[1]319)        |
| `1111` | Half Circle Back, Then Forward (632146) |

| Bits  | Command Input     |
|:-----:|:------------------|
| `000` | Light Punch (LP)  |
| `001` | Heavy Punch (HP)  |
| `010` | Either Punch (P)  |
| `011` | Both Punches (PP) |
| `100` | Light Kick (LK)   |
| `101` | Heavy Kick (HK)   |
| `110` | Either Kick (K)   |
| `111` | Both Kicks (KK)   |

The last bit is unused as of now.