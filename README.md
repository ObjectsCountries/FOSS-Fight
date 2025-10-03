# FOSS Fight

FOSS Fight is a 4-button fighting game à la *Super Street Fighter II Turbo*, with a roster of various FOSS mascots.

## Notes

* 4-button fighter
* No proximity normals or command normals (yet, except for 6LK)
* Charge time is 40 frames
* Throw is cl.4HP or cl.6HP
* Sweeps are universally cancelable
* 2HP is an anti-air
* 6LK is a universal overhead, +2 on hit and -1 on block
* No EX moves, only supers
* No canceling specials into supers

## Roster

### Tux

Mascot of Linux. **Archetype**: Shoto

* Slide (236K, air OK): Tux slides along the ground, or if done in the air, dives then slides.
* Snowball (214P): Throws a snowball at the opponent, with the light version being slower but having more range than the heavy version.
* Leap (623P): Uppercut with invincibility on the light version. Great for anti-airs and stopping pressure.
* **Super Art**: Double Leap (236236P): Two leaps of great height, fully invincible on startup.

### Evil Tux

Not a mascot, just an evil version of Tux. **Archetype**: Evil Shoto

**NOTES**: Evil Tux will not be on the select screen. To select him, pick Tux and hold the button that selected him for 1 second.

* Snowball (214P): Both travel the same speed and distance (the average of Tux's snowballs), but the heavy fireball hits twice and has more startup.
* Leap & Crush (623P): Similar to Tux's leap, but comes down with a hit as well.
* Axe Dive (41236P): Short hop into a quick dive that hits low when landing.
* **Super Art**: Raging Penguin (LP LP 6 LK HP): Evil Tux grabs the opponent and overwhelms them with a flurry of attacks. Not invincible and cannot catch opponents about to jump, but cannot be blocked.

### Konqi

Mascot of KDE. **Archetype**: Item Toss

**NOTES**: Up to 2 items can be on screen at once (with Triple Item Toss as an exception). If an item is on screen, the same item cannot be summoned.

* Item Toss (236P, air OK): Throws a random item from one of KDE's many apps.
  * Okular: Bounces along the ground. If fighting against Hyprchan, mirrors tile layout horizontally once it hits her; simply hits the opponent otherwise.
  * Dolphin: Jumps in one big arc. Heals whoever touches it.
  * Konsole: Starts from the top, and comes down diagonally. Inverts controls of whoever touches it.
  * Kdenlive: Moves horizontally, and hits multiple times (though it is possible to block between hits).
  * KDE Connect: Acts as a temporary puppet to Konqi, allowing him to attack from afar. KDE Connect cannot jump, and only has one attack that hits mid.
* Hug (360P): Hugs the opponent and leaves them stunned, allowing for a quick follow-up. Cannot be blocked.
* Dragon Dragon Punch (623P): An uppercut that's invincible on startup and is a great anti-air.
* **Super Art**: Triple Item Toss (236236P, air OK): Throws 3 items in quick succession.

### Hyprchan

Mascot of Hyprland. **Archetype**: Mixup

**NOTES**: Hyprchan can have up to 5 tiles present at a time. It is possible to drift left and right during her descent from a tile. p.X means an input during Latch.

* Split/Mend Horizontal (\[4]6P): Creates a horizontal split across the center of the screen. Doing this again mends the split.
* Split/Mend Vertical (\[2]8P): Creates a vertical split across the center of the screen. Doing this again mends the split.
* Latch (j.2LK): Latch onto the center of the tile that Hyprchan is over.
* Release (p.LP): Detach from a tile and jump in the inputted direction. p.5LP will result in a slight leap.
* Release & Attack (p.HP): Detach from a tile and jump in the inputted direction while attacking. p.5HP will result in a slight leap.
* Release & Mend (p.LK): Detach from a tile, mend it, and jump in the inputted direction. p.5LK will result in a slight leap, and will mend horizontally by default. Diagonal inputs will mend vertically if possible.
* Release & Split (p.HK): Detach from a tile, split it, and jump in the inputted direction. p.5HK will result in a slight leap, and will split horizontally by default. Diagonal inputs will split vertically if possible.
* **Super Art**: Hypr Reflector (236236P): Sets up an Aegis-like super of two tiles, one on top of the other. The tiles will flash 3 times to indicate attack level; for example, the top tile flashing, then the bottom, then the top indicates that the super will attack high, then low, then high.

### Wilber

Mascot of GIMP. **Archetype**: Zoner

* Square Boom (\[4]6P): Releases a spinning square projectile. 
* Flash Flip (\[2]8K): Wilber flips in an arc. Invincible on startup, and is great for anti-airing.
* Stroke (623P): Grabs the opponent and paints them, making them receive slightly more damage.
* **Super Art**: Squarenado (\[4]646P): Tornado that sucks the opponent into the center of the screen. Covers opponent in paint if it hits.

### Godette (and GDBot)

Mascots of Godot. **Archetype**: Puppet

**NOTES**: GDBot cannot leave the ground outside of Leap. By default, Godette and GDBot move together, with GDBot's speed being noticeably slower. To move Godette without moving GDBot, hold down any attack button. To move GDBot without moving Godette, input down-forward or down-back.

* Run (236P, air OK): GDBot dashes towards the opponent, automatically stopping when extremely close.
  * Stop (236P\~LP): Stops GDBot in its tracks.
  * Leap (236P\~HP): Leaps at the opponent. Hits high.
  * Slide (236P\~LK): Slides on the ground. Hits low.
  * Grab (236P\~HK): Grabs the opponent. Cannot be blocked.
* Godot Rekka (214X\~214X\~214X): A 3-part rekka that Godette does where punches hit high and kicks hit low. The first hit always hits mid, and the last hit will knock down if it is 214K. Run can be activated in between hits for further mixups.
* Godoken (236K): GDBot shoots out a laser beam, which flashes in front of it. The heavy version hits farther, but has more startup.
* **Super Art**: Independence (236236K): GDBot acts on its own for a few seconds, mainly trying to mix up the opponent.

### Gnu

Mascot of the GNU project (the G is not silent). **Archetype**: Stance

**NOTES**: Gnu is a stance character, with two major modes as his stances, and minor modes that give him various buffs. Only two of the minor modes are available at any given time.

#### Major Modes

* **Footsies Mode**: Gnu is slow, but his buttons have considerable range and deal moderate damage. Footsies Mode moves are represented with f.X.
* **Gatling Mode**: Gnu is noticeably faster, and while his buttons are short and somewhat weak, they can be canceled into each other. The order of canceling is 5LP > 2LP > 5LK > 2LK > 5HP > 2HP > 6LK > 5HK > 2HK. Gatling mode moves are represented with g.X.

#### Minor Modes

* **Alpha Mode**: 25% of the damage taken from the next combo appears as gray health, recovering gradually over time.
* **Beta Mode**: Gnu can hold up to slow down his descent from a jump.
* **Gamma Mode**: Gnu gains the ability to walk through the opponent.
* **Delta Mode**: Gnu gains the ability to cancel into Stance Change, GNU/Cycle and Many fighting game players run a modified version of Gnu every match.

#### Moves

* Stance Change (g.5PP into Footsies Mode, f.5KK into Gatling Mode): Changes the stance Gnu is in. Cannot be canceled into, but can be done before a round starts.

* GNU/Flip (g.\[2]8K): A jump that has multiple followups.
  * Flip/Chop (g.\[2]8K\~P): An overhead chop that must be blocked high.
  * Flip/Dive (g.\[2]8K\~K): A divekick that can be blocked either way. Can be done incredibly close to the ground to immediately cancel the divekick.
  * Flip/Slide (g.\[2]8K\~no input): A slide on the ground that must be blocked low.
* GNU/Tackle (g.\[4]6K): A shoulder tackle that juggles airborne opponents.
* GNU/Cycle (g.\[2]8P): Cycles the available minor modes. Cannot be canceled into. g.\[2]8LP cycles the first minor mode, and g.\[2]8HP cycles the second.
* **Super Art**: GNU/Slash (g.\[4]646P): Slashes through the opponent. Crosses up (and Gnu flashes) if the button used for the super is held.

* I'd just like to interject your pressure for a moment (f.623P): An invincible uppercut.
* What you're referring to as my oki is in fact GNU/Oki (f.214P): A floating fireball that can be steered left and right by inputting the respective directions. Hits 3 times.
* Many fighting game players run a modified version of Gnu every match (f.22P): Toggles a minor mode. Cannot be canceled into. f.22LP applies the first minor mode, and f.22HP applies the second.
* **Super Art**: All the so-called Gnu mains are really GNU/Mains (f.236236P): A large floating fireball that can be steered in any direction. Hits 5 times.


### Nolok

The main villain of Super Tux and Super Tux Kart. **Archetype**: Grappler

* Protect (22P): Creates a force field that protects from strikes, but not throws. Pulls opponent in on strikes, but Nolok cannot move while in this position. On-block advantage/disadvantage does not change for the attacker.
* Give Me a Hug (63214P): Runs towards the opponent to grab them.
* Driving Pilespinner (360P, air OK): Immediately grabs the opponent.
* **Super Art**: Obligatory Command Grab Super (720P): Grabs the opponent and piledrives them.

### Suzanne

Mascot of Blender, AKA the "Blender Monkey". **Archetype**: Setplay

* Summon Object (236X): Suzanne will summon the object she has available in a given position depending on the button used. Once on-screen, it can be attacked by Suzanne to move at a certain angle and velocity. Once a button has been used to place an object, that button cannot be used again until its corresponding object either despawns or hits the opponent (a shrugging animation will play when attempting this). The objects only differ in appearance, and all have the same hitbox.
* Withdraw Object (214X): Withdraws the object summoned with the given button, or plays a shrugging animation if no object with that button is present.
* Summon All Objects (41236K): Summons 4 objects at once, though this takes longer than summoning a single object.
* **Super Art**: Teapot (236236P): Pours tea on the ground, making it slippery for both Suzanne and the opponent for 5 seconds.

### Ferris

Mascot of the Rust programming language. **Archetype**: Install

**NOTES**: Only one of these moves can be active at a time. Ferris has a character-specific cooldown meter that controls when these moves can be used, as well as for how long. The meter starts off empty, but slowly fills over time. Inputting the command to activate a move while it's active will make it stop early, and preserve any remaining meter. Hitting or throwing Ferris will stop the move early as well, and empty the meter.

* &borrow\_checker (236P): Allows Ferris to leave their hurtbox where they're standing, and attack without being attacked.
* macro\_rules! (236K): Records inputs until cooldown meter runs out or until 236K is inputted again. Inputting 236K will make a ghost of Ferris spawn in front of them and repeat the inputs. Summoning the ghost will not affect the cooldown meter.
* #\[derive(Copy, Clone)] (214K): Creates a mirror of Ferris that copies their moves 20 frames after.
* **Super Art**: Blazingly Fast (236236P): Ferris' moves gain enhanced properties temporarily: &borrow\_checker makes Ferris' original hurtbox disappear entirely, macro\_rules! will make the ghost execute the inputs twice, and #\[derive(Copy, Clone)]'s delay is reduced to 15 frames.

### Debuggy

Debug character for FOSS Fight. **Archetype**: Joke

**NOTES**: Debuggy has a character-specific meter that starts off empty, fills up over time, and controls Debuggy's ability to use special moves. To play as Debuggy, compile the game from source with `-DDEBUGGY=1` as a flag.

* fireball (236P): Throws a black sphere that hits once. Consumes 15% of the meter.
* square kick or whatever i forgot what it was called (\[2]8K): A Flash Kick-like special that anti-airs and is invincible on startup. Consumes 35% of the meter.
* let's go gambling (236K): Throws a random shape. Like Suzanne, these shapes can be controlled by hitting them. Consumes 10% of the meter.
* **Super Art**: this is a super cool super art (236236P): Throws a bunch of shapes that bounce around.
