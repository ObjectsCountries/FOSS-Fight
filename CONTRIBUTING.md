# Contributing

## \*.ff files

\*.ff files are how FOSS Fight character data is stored. The header is `F0 55`, which represents "FOSS".

Each animation begins with two bytes describing the type of animation it is.

| Number  | Data                  |
|:-------:|:----------------------|
| `00 00` | Idle                  |
| `00 01` | Walking Forward       |
| `00 02` | Walking Backwards     |
| `00 03` | Crouch Transition     |
| `00 04` | Crouching             |
| `00 05` | Stand Blocking        |
| `00 06` | Stand Crouching       |
| `00 07` | Pre-Jump              |
| `00 08` | Jumping Forwards      |
| `00 09` | Jumping Neutral       |
| `00 0A` | Jumping Backwards     |
| `00 0B` | Stand Getting Hit     |
| `00 0C` | Crouch Getting Hit    |
| `00 0D` | Air Getting Hit       |
| `00 0E` | Air Reset             |
| `00 0F` | Knockdown             |
| `00 10` | Getting Up            |
| `00 11` | Standing Light Punch  |
| `00 12` | Standing Heavy Punch  |
| `00 13` | Standing Light Kick   |
| `00 14` | Standing Heavy Kick   |
| `00 15` | Crouching Light Punch |
| `00 16` | Crouching Heavy Punch |
| `00 17` | Crouching Light Kick  |
| `00 18` | Crouching Heavy Kick  |
| `00 19` | Jumping Light Punch   |
| `00 1A` | Jumping Heavy Punch   |
| `00 1B` | Jumping Light Kick    |
| `00 1C` | Jumping Heavy Kick    |
| `00 1D` | Forward Light Kick    |
| `00 1E` | Throw Whiff           |
| `00 1F` | Forward Throw         |
| `00 20` | Backwards Throw       |

Anything greater than `00 20` is character-specific (such as specials).

After these two bytes, another two bytes indicate how many frames this animation consists of. Then, the hitboxes, hurtboxes, etc. are detailed in order of left, top, width, and height.

| Number  | Box Type            |
|:-------:|:--------------------|
| `00 00` | Hurtbox             |
| `00 01` | Hitbox              |
| `00 02` | Grab Box            |
| `00 03` | Command Grab Box    |
| `00 04` | Throwbox/Push Box   |
| `00 05` | Proximity Guard Box |

Each defined hurtbox will have a pair of bytes defining its cancellability.

| Number  | Cancellability      |
|:-------:|:--------------------|
| `00 00` | Not Cancelable      |
| `00 01` | Specials Only       |
| `00 02` | Supers Only         |
| `00 03` | Specials and Supers |

If a box's coordinates are with `FF FF` and it is not part of a copied frame, that means that it covers the entire asset as one whole rectangle.

If a frame has a length of `FF FF`, that means that it is copying another frame's data. The first two bytes after `FF FF` represent the animation type to copy from, and the next two bytes are the frame index (starting from 0). Then, `00 00` to copy the duration, or any other number to define manually. After the frame count, `00 01` to copy the location on the sprite sheet, or `00 00` to begin manual definition. Then, `00 01` to copy all boxes, and `00 00` to define manually. If a manually-defined box type has `FF FF` as its number of boxes, that means it is to be identical to that of the frame being copied.

### Example

(Note to self) To format data:

1. Find `/(([0-9A-F]{2} ){2})/`, replace with `/\1 /`.
2. Find `/((([0-9A-F]{2} ){2} ){8})/`, replace with `/\1\n/`.
3. Find `/  $/` (two spaces before the `$`), replace with `//` (delete).

TODO: Fill in with Debuggy data, update explanations

```hexdump

```

## To Do

* Add jumping data for Debuggy next time
* Rework copying to use bits
* Add horizontal walls