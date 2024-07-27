# Game Specifications

## Overview

We're creating a bullet hell, Vampire Survivors-like game. It is a time survival game with minimalistic gameplay and roguelike elements. The game will support network multiplayer for 1 to 4 players.

## Gameplay

### Controls

- The player moves their fighter using the WASD keys.

### Waves of Enemies

- Each enemy has specific behaviors, powers, and stats.

### Player Characters

Players can choose from a set of characters. Initially, there will be two characters: Leo and Pow, each with their own stats and skills.

#### Character Details

Each character has:

- A passive default attack that counts as an item and can be upgraded.
- A set of statuses: Armor, Move Speed, Ability Haste, Health Regeneration, Max Health, Projectile Count, Critical Chance, Area Size, and Level.
- A skill that unlocks at level 3.
- An ultimate ability that unlocks at level 6.
- Five slots for stats to upgrade their base stats, which can be increased to seven slots with the shop.
- Five slots for items, one of which is always the character's default attack.

#### Leo (Tank Melee Character)

**Abilities:**

- **Passive:**

  - Gains bonuses to Area Size, Armor, Max Health, and Health Regeneration based on her Armor and Health stats.

- **Base Skill:**

  - **Cooldown:** 17 seconds
  - **Effect Radius:** 300 / 450 units
  - Leo raises her defenses, gaining 200% Armor and a shield equal to 40% Max Health for 3 seconds. The shield explodes after the duration, dealing damage and refreshing itself for 2 seconds.
  - **Explosion Damage:** 200 (+ 5 per level)
  - Burns enemies every 0.5 seconds while the shield is active.
  - **Burn Damage per Tick:** 20 (+ 3 per level)

- **Ultimate:**

  - **Cooldown:** 90 seconds
  - **Target Range:** 2000 units
  - **Effect Radius:** 1000 units
  - Leo calls down a solar flare, dealing damage and stunning enemies for 2 seconds.
  - **Flare Damage:** 205 (+ 5 per level)
  - Creates a scorched zone for 12 seconds, dealing damage every 0.25 seconds.
  - **Zone Damage per Tick:** 9 (+ 1 per level)

- **Signature Weapon:**
  - **Cooldown:** 3 / 2.75 / 2.5 / 2.25 / 2 seconds
  - **Range:** 550 units
  - **Angle:** 38°
  - Leo projects a light beam, dealing damage in a cone.
  - **Beam Damage:** 50 / 75 / 100 / 125 / 150 (+ 100% Armor)
  - **Scales With:** Ability Haste, Area Size, Armor, Critical Chance, Damage

#### Pow (Marksman Character)

**Abilities:**

- **Passive:**

  - Upon killing 70 non-elite enemies or 1 elite, gains bonuses for 8 seconds:
    - +75 Ability Haste
    - +(3.5% × Total Times Excited) Move Speed, decaying over the duration.

- **Base Skill:**

  - **Cooldown:** 13 seconds
  - **Range:** 2200 units
  - **Effect Radius:** 300 units
  - **Width:** 60 units
  - Pow sends out 9 rockets in a cone, each exploding on contact.
  - **Damage per Rocket:** 50 (+ 5 per level)

- **Ultimate:**

  - **Cooldown:** 70 seconds
  - **Range:** 2000 units
  - **Effect Radius:** 600 units
  - **Width:** 280 units
  - Pow unleashes a massive rocket that explodes, dealing damage based on the target's missing health.
  - **Base Damage:** 500 (+ 10 per level)
  - **Maximum Damage:** 1500 (+ 30 per level)

- **Signature Weapon:**
  - **Cooldown:** 2.5 seconds
  - **Range:** 1500 units
  - **Width:** 90 units
  - Pow fires clusters of bullets, each damaging the first enemy hit.
  - **Cluster Count:** 3 / 4 / 5 / 6 / 7
  - **Total Damage:** 60 / 80 / 100 / 120 / 140
  - **Scales With:** Ability Haste, Critical Chance, Damage, Projectile Count

### Stats and Items

Stats and items are collected in a roguelike card manner through level-ups. Each level-up allows the player to choose between 3 randomly selected cards for stats and items. Both items and stats can be upgraded up to 5 times.

### Upgrades

#### Objectives and Rewards

- Some objectives reward players with upgrades, granting extra stats from the beginning of the level.
- Upgrades can be purchased from the main lobby menu using Gold, a currency earned during gameplay.
- Gold is earned regardless of victory or defeat.
- The entire team earns the same amount of Gold any teammate picks up.
- Players can earn Gold by:
  - Killing elite and mini-boss monsters.
  - Killing monsters with the augment of dropping gold at every X monster kills.

#### List of Upgrades

| Upgrade          | Increment | Max Level | Cost |
| ---------------- | --------- | --------- | ---- |
| Damage           | +10%      | 8         |      |
| Armor            | +5        | 5         |      |
| Max Health       | +10%      | 5         |      |
| Health Regen     | +3        | 5         |      |
| Move Speed       | +5%       | 5         |      |
| Pickup Radius    | +25%      | 3         |      |
| Area Size        | +5%       | 4         |      |
| Duration         | +5%       | 4         |      |
| Critical Chance  | +5%       | 4         |      |
| Ability Haste    | +5        | 5         |      |
| EXP              | +5%       | 5         |      |
| Projectile Count | +1        | 2         |      |
| Gold Mod         | +15%      | 3         |      |

## Implementation Specifications

The game will be written in C++ using raylib and raygui for the engine and interface, and SanderMertens/flecs to manage ECS. The codebase should be well organized with separate logic concerns.

### Directory Structure

```
includes/
  swarm/
    ... (headers organized within folders representing common data)
lib/
  ... (sources organized within folders representing common data)
src/
  main.cpp (main application)
  // other binaries
```
