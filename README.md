# Shadow Empire

Shadow Empire is a console-based turn-based strategy management game built in C++17 for an Object-Oriented Programming course. The player manages a fictional underground organization by balancing profit, territory expansion, crew management, and Heat risk.

## Features

- New game and organization setup
- Buy goods
- Hire Dealers and Enforcers
- Assign and move Dealers between territories
- Sell goods through Dealers
- Pending Deals with delayed payment
- Next Week processing
- Territory expansion based on Power requirements
- Heat system
- Police events
- Crackdown event
- Save / Load system
- Win / Lose conditions

## OOP Concepts Demonstrated

- Encapsulation through private `Organization` fields and controlled public methods
- Inheritance with `CrewMember` as a base class for `Dealer` and `Enforcer`
- Polymorphism with `GameEvent` as a base class for concrete police events
- Abstraction through abstract base classes
- Separation of concerns between `Game`, `Organization`, `SaveManager`, models, events, and utility constants
- Safe ownership using `std::unique_ptr<CrewMember>` for polymorphic crew storage

## Project Structure

```text
include/
  core/       Organization class
  game/       Game menu and gameplay flow
  models/     CrewMember, Dealer, Enforcer, Territory, PendingDeal
  systems/    SaveManager and GameEvent hierarchy
  utils/      Constants

src/
  core/
  game/
  models/
  systems/
  utils/

data/         Save file location
docs/         Architecture document and diagrams
```

## Build Instructions

The project uses a `Makefile`.

For Linux, macOS, or Git Bash:

```bash
make
make run
make clean
```

For Windows with MinGW:

```bash
mingw32-make
mingw32-make run
```

On Windows, the project can also be built and run using:

```bat
start.bat
```

## How to Play

The main gameplay loop is based on weekly decisions:

1. Buy goods.
2. Hire a Dealer.
3. Assign the Dealer to an unlocked territory.
4. Sell goods through the Dealer.
5. Advance to the next week.
6. Receive payment when Pending Deals complete.
7. Hire Enforcers to increase Power.
8. Expand into new territories.
9. Manage Heat to reduce the risk of police pressure and Crackdowns.

## Win / Lose Conditions

Win condition:

- Money >= 30000
- Power >= 40
- Unlocked territories >= 3

Lose condition:

- Money < 0

## Save / Load

The save file is stored at:

```text
data/save.txt
```

`SaveManager` saves and loads the complete game state, including resources, crew, territories, pending deals, and `nextCrewId`.

Because crew members are stored polymorphically as `std::unique_ptr<CrewMember>`, the save file stores a type tag for each crew member:

- `Dealer`
- `Enforcer`

This allows the correct subclass objects to be reconstructed when loading.

## Checkpoint 2 Status

Implemented:

- More than 4 planned functionalities are working
- Class hierarchy from checkpoint 1 is implemented
- Project compiles and runs
- Git history contains at least 5 commits

Design changes:

- No major design changes from checkpoint 1.
- Minor implementation differences were made to keep the MVP simpler, more stable, and easier to test from the console.

## Notes

This is an MVP console version of the game.

The following systems are not included:

- GUI
- Broker system
- Loyalty system
- Rival events
- Market events
- Passive territory income
- Combat system
