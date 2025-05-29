# Coup SFML Game

## Overview

This is a C++ implementation of the popular bluffing card game *Coup*, using SFML for the graphical interface. Two to six players take on hidden roles and perform actions such as Gather, Tax, Coup, etc., trying to eliminate opponents until only one remains.

## Directory Structure

```
/Ex3
├── Makefile
├── README.md        <-- this file
├── main.cpp         <-- entry point, launches HomeScreen
├── HomeScreen.hpp   
├── HomeScreen.cpp   <-- main menu & player‑count UI
├── Game.hpp
├── Game.cpp         <-- game logic & SFML main window
├── Player.hpp
├── Player.cpp       <-- base Player class
├── Baron.hpp/.cpp   
├── Governor.hpp/.cpp
├── Spy.hpp/.cpp     
├── General.hpp/.cpp
├── Judge.hpp/.cpp  
├── Merchant.hpp/.cpp
├── tests/
│   └── test.cpp     <-- doctest unit tests
└── Font/
    └── PIXEARG_.ttf  <-- game font
```

## Building

1. Install SFML (graphics, window, system).
2. From the project root:

   ```bash
   make            # builds `main` and `test`
   ./main          # run the game
   ./test          # run unit tests (requires doctest)
   make clean      # remove binaries and object files
   ```

## Gameplay Instructions

1. Run `main` to open the Home Screen.
2. Click **Start Game**.
3. A dialog appears: choose **number of players** (2–6) and click **Start**.
4. For each player, enter a **name** and click **Accept**/Press Enter.

   * Or: name your player exactly `Baron`, `Governor`, `Spy`, `General`, `Judge`, or `Merchant` to force that role.
5. After naming all players, the main game window opens:

   * **Right‑click** the “View coins” button to toggle revealing all coins (debug).
   * **Left‑click** it (spies only) to view everyone’s coins while held.
   * **Action buttons (2×3)** perform: Gather, Tax, Bribe, Arrest, Sanction, Coup.
   * **End Turn** advances to the next alive player.
   * **Spy** sees an extra **Block Arrest** button.
   * **Baron** sees an extra **Invest** button (pay 3 → gain 6).
   * **Turn Info** button toggles display of current player name, role, and coins.
6. The last non‑spectator wins; a pop‑up shows the winner’s name and **Go Back to Menu**.

## Hidden & Debug Features
* **Enter and Esc on some windows will press the corresponding accept/yes button and Esc will close the window 
 
* **Name override**: naming exactly `Baron`, `Governor`, `Spy`, `General`, `Judge`, or `Merchant` bypasses random assignment.
* **Right‑click** on **View coins** toggles coin reveal for all (debug).

## Code Architecture

### Player Class (Player.hpp/.cpp)

* **Purpose**: encapsulates common data (name, ID, role, coins, flags) and actions:

  * `gather()`, `tax()`, `bribe()`, `arrest()`, `sanction()`, `coup()`, `endTurnhelp()`.
* **Rule of Three**:

  * Custom copy constructor, copy assignment, and destructor (defaulted) ensure proper copying of name, role, coins.
* **Inheritance**: six subclasses override or extend behavior:

  * `Baron` (invest), `Governor` (tax override), `Spy`, `General` (onArrest bonus), `Judge`, `Merchant` (start‑turn bonus).

### Game Class (Game.hpp/.cpp)

* **launchGame()**: builds players, prompts for names, assigns roles, then calls `runningGame()`.
* **assignRole()**: checks name override first; otherwise picks a random role and replaces the `Player*` with the appropriate subclass.
* **runningGame()**: main SFML loop:

  * Initializes UI elements (info boxes, buttons).
  * Tracks `currentPlayerIndex`, `endTurn` flags, and per-turn bonuses.
  * Handles button clicks:

    * **Gather/Tax** with governor block prompt.
    * **Bribe** with judge block prompt.
    * **Arrest** with victim selection.
    * **Sanction**, **Coup** with general block prompt and victory check.
    * **Baron Invest**, **Spy Block Arrest**, **View coins**, **Turn Info**, **End Turn**.
* **Utility dialogs**:

  * `chooseVictim()`, `governorBlockPrompt()`, `blockCoupPrompt()`, `blockBribePrompt()`, `showWinnerWindow()`.

## Roles & Uniqueness

* **Baron**: pays 3 to invest, gains 6 (once per turn).
* **Governor**: collects 3 on Tax; can block others’ Tax.
* **Spy**: may view all coins; can block an upcoming Arrest for any player.
* **General**: recovers 1 coin when arrested; can block one Coup by paying 5.
* **Judge**: can block one Bribe by paying nothing (briber still spends 4).
* **Merchant**: at the start of turn, if ≥3 coins and not yet triggered, gains +1; otherwise acts normally.

## Makefile

* **Targets**:

  * `all` (default): builds `main` and `test`.
  * `main`: links all `.o` into the playable executable.
  * `test`: links core `.o` with `test.o` to produce a test executable, then runs it.
  * `clean`: removes object files and executables.
* **Variables**:

  * `CXXFLAGS` includes `-std=c++17 -Wall -Wextra -pedantic -g -I.`.
  * `LDFLAGS` links SFML libs.

## Testing (`test.cpp`)

* Uses **doctest** to verify:

  * Base `Player` actions (gather, tax, bribe).
  * Subclass behaviors (`Baron::invest`, `Governor::tax`, `General::onArrest`).
  * `Merchant` start‑turn bonus via passive flag.
  * `assignRole()` honors name‑overrides and random fallback variety.

//mishaaskarov@gmail.com
