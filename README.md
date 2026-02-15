# 🏎️ 2D Racing Car Game

A fast-paced, top-down 2D racing game developed in **C++** using `<graphics.h>` (or your preferred graphics library). Race through challenging tracks, avoid obstacles, compete against AI or friends, and aim to cross the finish line first!
![image alt](https://github.com/Kushmma/2D-Racing-Car-Game/blob/master/Screenshot%202026-02-15%20081830.png?raw=true)

## 📋 Table of Contents
- [Features](#-features)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Controls](#-controls)
- [Score System](#-score-system)
- [Screenshots](#-screenshots)
- [Contact](#-contact)
  
## 🎮 Features

* **Smooth 2D Movement** – Realistic car movement with acceleration, braking, and turning mechanics.
* **Game Modes**:
  * **Single Player** – Beat AI opponents on different tracks.
  * **Multiplayer** – Race against a friend locally.
* **Dynamic AI Opponents** – Computer-controlled cars with basic obstacle avoidance and lane-following.
* **Varied Tracks** – Multiple circuits with unique layouts and challenges.
* **Obstacles & Hazards** – Avoid spikes, walls, and moving objects.
* **Lap & Finish Line Tracking** – Keep track of completed laps and race winners.
* **Score & Time Recording** – Records high scores and fastest laps using file I/O.
* **Keyboard Controls** – Simple arrow key navigation for a smooth experience.
---

## 📁 Project Files

- `Makefile.win` - Windows makefile for compilation
- `game.cpp` - Core game logic and rendering
- `game.h` - Game class declarations
- `main.cpp` - Main game loop and initialization
- `score.cpp` - Score management functions
- `score.h` - Score class declarations
- `.gitignore` - Git ignore file
- `Screenshot 2026-02-15 081830.png` - Game screenshot

## 🎮 Controls System

### Keyboard Controls

| Action | Primary Key | Alternative Key |
|--------|-------------|-----------------|
| **Move Left** | `←` (Left Arrow) | `A` |
| **Move Right** | `→` (Right Arrow) | `D` |
| **Accelerate** | `↑` (Up Arrow) | `W` |
| **Brake/Reverse** | `↓` (Down Arrow) | `S` |

## 💾 Score System


### Score System Overview

The game tracks wins and scores for both **Single Player** (Player vs Bot) and **Multiplayer** (Player 1 vs Player 2) modes. All match results are saved persistently using file I/O.

### Score Structures

#### Single Player Stats (Player vs Bot)
| Field | Description |
|-------|-------------|
| Player Wins | Number of matches won by player |
| Bot Wins | Number of matches won by bot |
| Matches Played | Total number of matches |


#### Multiplayer Stats (Player 1 vs Player 2)
| Field | Description |
|-------|-------------|
| Player 1 Score | Total score of player 1 |
| Player 2 Score | Total score of player 2 |
| Player 1 Wins | Matches won by player 1 |
| Player 2 Wins | Matches won by player 2 |
| Matches Played | Total matches between them |


## 📸 Screenshots

<div align="center">

| Main Menu | Gameplay |
|:---:|:---:|
| <img width="400" src="https://github.com/user-attachments/assets/67b2a19d-62ba-4f95-a792-f35b438d908c" /> | <img width="400" src="https://github.com/user-attachments/assets/600a791a-7a2f-410d-992e-45052d174e2c" /> |

| Track View | Score Screen |
|:---:|:---:|
| <img width="400" src="https://github.com/user-attachments/assets/e5d71ff2-b05d-4d75-b1e9-0a1ea74973cd" /> | <img width="400" src="https://github.com/user-attachments/assets/3cee2d65-7b37-4345-8364-5960f478c717" /> |

</div>

  
## 🚀 Getting Started

### Prerequisites

* **C++ Compiler** – Supports C++11 or higher (GCC, Clang, MSVC).
* **Graphics Library** – `<graphics.h>` (or modern alternatives like WinBGIm).
* **IDE** – Dev C++, Code::Blocks, Turbo C++, or any C++ compatible IDE.

## 📞 Contact

| | |
|---|---|
| **Name** | Kushma Shrestha |
| **Email** | arushsthaii@gmail.com |
| **GitHub** | [@yourusername](https://github.com/Kushmma) |
| **Instagram** | [@yourhandle](https://instagram.com/kushmma) |
