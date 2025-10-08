# Contributing

## \*.ff files

\*.ff files are how FOSS Fight character data is stored. The header is `F0 55`, which represents "FOSS".

Each animation begins with two bytes describing the type of animation it is.

|  Number  | Data                  |
|:--------:|:----------------------|
| `0x0000` | Idle                  |
| `0x0001` | Walking Forward       |
| `0x0002` | Walking Backwards     |
| `0x0003` | Crouch Transition     |
| `0x0004` | Crouching             |
| `0x0005` | Stand Blocking        |
| `0x0006` | Stand Crouching       |
| `0x0007` | Pre-Jump              |
| `0x0008` | Jumping Forwards      |
| `0x0009` | Jumping Neutral       |
| `0x000A` | Jumping Backwards     |
| `0x000B` | Stand Getting Hit     |
| `0x000C` | Crouch Getting Hit    |
| `0x000D` | Air Getting Hit       |
| `0x000E` | Air Reset             |
| `0x000F` | Knockdown             |
| `0x0010` | Getting Up            |
| `0x0011` | Standing Light Punch  |
| `0x0012` | Standing Heavy Punch  |
| `0x0013` | Standing Light Kick   |
| `0x0014` | Standing Heavy Kick   |
| `0x0015` | Crouching Light Punch |
| `0x0016` | Crouching Heavy Punch |
| `0x0017` | Crouching Light Kick  |
| `0x0018` | Crouching Heavy Kick  |
| `0x0019` | Jumping Light Punch   |
| `0x001A` | Jumping Heavy Punch   |
| `0x001B` | Jumping Light Kick    |
| `0x001C` | Jumping Heavy Kick    |
| `0x001D` | Forward Light Kick    |
| `0x001E` | Throw Whiff           |
| `0x001F` | Forward Throw         |
| `0x0020` | Backwards Throw       |

Anything above `0x0020` is character-specific.

After these two bytes, another two bytes indicate how many frames this animation consists of. Then, the hitboxes, hurtboxes, etc. are detailed in order of left, top, right, bottom.

|  Number  | Box Type            |
|:--------:|:--------------------|
| `0x0000` | Hurtbox             |
| `0x0001` | Hitbox              |
| `0x0002` | Grab Box            |
| `0x0003` | Command Grab Box    |
| `0x0004` | Throwbox            |
| `0x0005` | Pushbox             |
| `0x0006` | Proximity Guard Box |

Example:

```hexdump
F0 55  44 65  62 75  67 67  79 00  00 00  00 04  00 00
00 01  00 00  00 00  00 32  00 32  00 01  00 00  00 02
00 00  00 03  00 00  00 04  00 01  00 00  00 28  00 32
00 32  00 05  00 01  00 00  00 28  00 32  00 32  00 06
00 00
```

`00 00` means that the following data is for the character's idle animation. The following `00 04` means that there are 4 frames. `00 00` afterwards means that a hurtbox is being described, and the `00 01` afterwards means that there is one hurtbox. `00 00 00 00 00 32 00 32` means that the part of the sprite sheet used for this hurtbox extends from (0, 0) to (50, 50). `00 01` begins to describe the frame's hitboxes, but `00 00` means there are none. The same applies for `00 02 00 00` and `00 03 00 00`, meaning no grab/command grab boxes. However, `00 04 00 01` means that there is one throwbox. Its coordinates on the sprite sheet are `00 00 00 28 00 32 00 32`, or from (0, 40) to (50, 50). The same applies for what comes after `00 05 00 01`, meaning there is a pushbox with the same coordinates. `00 06 00 00` means that there are no proximity guard boxes.

After all six types of boxes have been described for this frame, the next frame of the idle animation is detailed with the same format. After describing all four frames of the idle animation, the forward walk animation is described.
