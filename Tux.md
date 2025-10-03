# Tux

Use [this](https://www.spriters-resource.com/arcade/streetfighter2/sheet/60224/) as a point of reference.

## Movement

* 3 sprites for an idle animation (first, second, third, second)
* 2 sprites for walking forwards
  * Tux leans forwards a bit
* 2 sprites for walking backwards
  * Tux leans backwards a bit
* 1 sprite in-between standing and crouching
* 1 sprite for crouching

## Blocking

* 1 sprite for blocking while standing
* 1 sprite for blocking while crouching

(No in-between)

## Jumping

* 1 sprite for being in midair

(Pre-jump frames will reuse the sprite between standing and crouching)

## Getting Hit

* 1 sprite for getting hit while standing
  * 1 sprite in between this and the first standing sprite
* 1 sprite for getting hit while crouching
  * 1 sprite in between this and the crouching sprite
* 1 sprite for getting hit in midair
* A few sprites for doing a midair backflip after getting hit
* 1 sprite laying down face up
* 2 or 3 sprites getting up

## Attacks

Assume Tux is facing right. Keep in mind that heavy attacks will play the sprites in the order of first, second, first, idle.

### Normal Attacks

* 1 sprite for standing light punch
  * Tux's left flipper quickly makes a jab
* 2 sprites for standing heavy punch
  * Tux's right flipper punches straight forward
* 1 sprite for standing light kick
  * Tux makes a small kick near the ground with his right foot
* 2 sprites for standing heavy kick
  * Tux makes a kick straight forward with his left foot
* 1 sprite for crouching light punch
  * Similar to standing light punch
* 2 sprites for crouching heavy punch
  * Tux punches upwards while remaining on the ground
* 1 sprite for crouching light kick
  * Tux moves his left foot a bit forward
* 2 sprites for crouching heavy kick
  * Tux sticks his right leg out and spins on the ground, staying upright
* 1 sprite for jumping light punch
  * Tux sticks his left flipper out a little bit
* 2 sprites for jumping heavy punch
  * Tux sticks his right flipper out noticeably further
* 1 sprite for jumping light kick
  * Tux sticks his left foot out a little bit
* 2 sprites for jumping heavy kick
  * Tux sticks his right foot out noticeably further
* 3 sprites for forward light kick
  * **Not a kick**, but rather a chop
* 1 sprite for whiffing a throw
* 3 sprites for throwing an opponent
  * First sprite should be holding an opponent, second should be preparing to throw and third should be throwing (don't worry about sprites for being thrown, we can reuse other sprites)

### Special Attacks

* 2 sprites for Slide
  * 1 for Tux leaning towards the ground about to slide on his belly, the other sliding
* 3 sprites for Snowball
  * First sprite should be Tux holding a snowball with his left arm, second should be shortly before throwing the snowball, third should be after throwing the snowball (no snowball in arm)
* 1 sprite for Leap
  * This sprite should look like a Mario jump, prejump frames will be reused

### Super Art: Double Leap

Just reuse the Leap sprite.
