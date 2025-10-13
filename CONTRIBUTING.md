# Contributing

## \*.ff files

\*.ff files are how FOSS Fight character data is stored. The header is `F0 55`, which represents "FOSS".

Each file will begin with floating point numbers representing movement stats, in this order:

1. Size
2. Walking Forward Speed
3. Walking Backward Speed
4. Forward Jump X-Velocity
5. Backward Jump X-Velocity
6. Initial Jump Velocity
7. Gravity

Then, the character sprite animations are described. Each animation begins with two bytes describing the type of animation it is.

| Number  | Data                     |
|:-------:|:-------------------------|
| `00 00` | Idle                     |
| `00 01` | Walking Forward          |
| `00 02` | Walking Backwards        |
| `00 03` | Crouch Transition        |
| `00 04` | Crouching                |
| `00 05` | Stand Blocking           |
| `00 06` | Crouch Blocking          |
| `00 07` | Pre-Jump                 |
| `00 08` | Jumping Forwards         |
| `00 09` | Jumping Neutral          |
| `00 0A` | Jumping Backwards        |
| `00 0B` | Stand Getting Hit        |
| `00 0C` | Crouch Getting Hit       |
| `00 0D` | Air Getting Hit          |
| `00 0E` | Air Reset                |
| `00 0F` | Knockdown                |
| `00 10` | Getting Up               |
| `00 11` | Victory Animation        |
| `00 12` | Time Over Loss Animation |
| `01 00` | Standing Light Punch     |
| `01 01` | Standing Heavy Punch     |
| `01 02` | Standing Light Kick      |
| `01 03` | Standing Heavy Kick      |
| `01 04` | Crouching Light Punch    |
| `01 05` | Crouching Heavy Punch    |
| `01 06` | Crouching Light Kick     |
| `01 07` | Crouching Heavy Kick     |
| `01 08` | Jumping Light Punch      |
| `01 09` | Jumping Heavy Punch      |
| `01 0A` | Jumping Light Kick       |
| `01 0B` | Jumping Heavy Kick       |
| `01 0C` | Forward Light Kick       |
| `01 80` | Forward Throw            |
| `01 81` | Backwards Throw          |
| `02 00` | Special 1                |
| `02 01` | Special 2                |
| `02 02` | Special 3                |
| `02 03` | Special 4                |
| `02 04` | Special 5                |
| `02 05` | Special 6                |
| `02 06` | Special 7                |
| `02 07` | Special 8                |
| `02 08` | Special 9                |
| `02 09` | Special 10               |
| `03 00` | Super                    |

For Gnu, these describe his Footsies mode sprites. If a pair of bytes begins with `10`, `11`, `12`, or `13`, these are his Gatling mode sprites.

Anything beginning with `04` describes miscellaneous assets such as projectiles.

After these two bytes, another two bytes indicate how many different sprites this animation consists of.

For each sprite, the first 4 pairs of bytes are the location of the asset on the sprite sheet, in order of leftmost pixel, uppermost pixel, width and height. Then, the boxes are described.

| Number  | Box Type                                |
|:-------:|:----------------------------------------|
| `00 00` | Null Terminator                         |
| `00 01` | Hurtbox                                 |
| `00 02` | Grab Box                                |
| `00 03` | Command Grab Box                        |
| `00 04` | Throwbox, Push Box and Ground Collision |
| `00 05` | Proximity Guard Box                     |
| `01 00` | Non-Cancelable Hitbox                   |
| `01 01` | Special-Cancelable Hitbox               |
| `01 02` | Super-Cancelable Hitbox                 |
| `01 03` | Special- and Super-Cancelable Hitbox    |

If a frame's length begins with `FF`, that means that it is copying another frame's data. The byte after `FF` determines which information to copy (most significant bit first). In order, the bits are:

1. Frame Length
2. Sprite Sheet Location
3. Hurtboxes
4. Grab Boxes
5. Command Grab Boxes
6. Throwboxes & Push Boxes
7. Proximity Guard Boxes
8. Hitboxes

A value of 1 means to copy this information, and 0 means to define it explicitly. The definitions are implicitly ordered.  

The first two bytes afterward represent the animation type to copy from, and the next two bytes are the frame index (starting from 0). 

If a box's coordinates are `FF FF`, that means that it covers the entire asset as one whole rectangle.

### Example

(Note to self) To format data:

1. Find `/(([0-9A-F]{2} ){2})/`, replace with `/\1 /`.
2. Find `/((([0-9A-F]{2} ){2} ){8})/`, replace with `/\1\n/`.
3. Find `/  $/` (two spaces before the `$`), replace with `//` (delete).

```hexdump

```

## To Do

* Fill in code box with Debuggy data
* Add horizontal walls