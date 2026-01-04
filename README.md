# The Adventures of CurryMan– Gameplay Systems Prototype (C++)

This repository contains a **gameplay prototype built in Unreal Engine 5 using C++**, focused on testing player control, combat flow, and systemic interaction.

The goal of this prototype was to validate whether a specific style of **player freedom and combat expressiveness** could be reproduced consistently in Unreal Engine, independent of engine-specific quirks or tooling.

This is an **exploratory prototype**, not a production-ready project.

---

## Intent & Scope

This prototype was built to answer a very specific question:

> Can the same sense of player-driven flow, responsiveness, and expressive combat be achieved in Unreal Engine using clean C++ systems?

To explore that, the code emphasizes:
- Explicit gameplay logic over engine abstractions
- Timing-driven combat systems
- Animation-aware hit detection
- State locking and release (movement, combat, supers)
- Minimal automation or framework-level assistance

The focus is on **behavioral outcomes**, not feature coverage.

---

## Important Notes

- This repository intentionally does **not** include:
  - A runnable Unreal project
  - `.uproject` files
  - Assets, animations, UI layouts, or proprietary data

- The prototype exists as a **code-only artifact** to keep the focus on system design and reasoning.
- These files are not meant to be compiled as-is.

---

## Design Approach

- Systems are written **explicitly** and locally
- No Gameplay Ability System (GAS)
- No Blueprint-driven gameplay logic
- No Enhanced Input abstraction
- No premature generalization

Each system exists to answer a gameplay question, not to serve as a reusable framework.

---

## Systems Overview

### Core Gameplay
- `PlayerController_Prototype`  
  Player movement, rotation, gravity handling, and state locking

- `CombatController_Prototype`  
  Input buffering, combo sequencing, timing windows

- `HealthSystem_Prototype`  
  Damage handling, hit reactions, KO flow, death signaling

- `HitDetector_Prototype`  
  Animation-driven hit windows and damage dispatch

### AI
- `NPCChase_Prototype`  
  Distance-based pursuit and combat handoff

- `NPCCombat_Prototype`  
  Attack timing, facing logic, animation synchronization

### Super / Meter Systems
- `SuperController_Prototype`  
  Super activation, temporary damage scaling, state locking

- `SuperMeterSystem_Prototype`  
  Hit-based meter accumulation and gating

- `SuperMeterUI_Prototype`  
  Minimal UI representation of meter fill

### Camera & Feedback
- `SideScrollerCamera_Prototype`  
  Smoothed follow camera with positional bounds

- `CameraShake_Prototype`  
  Lightweight procedural camera shake

- `DynamicFocus_Prototype`  
  Runtime depth-of-field control via post-process volume

### UI
- `UI_HealthBar_Prototype`  
  Event-driven health display with smoothing and color feedback

### Audio
- `AudioManager_Prototype`  
  Centralized SFX and music playback

### Game Flow & Environment
- `SplashScreenController_Prototype`  
  Timed splash flow and level transition

- `GameResetManager_Prototype`  
  Win/lose detection and delayed reset

- `LightFlicker_Prototype`  
  Procedural light flicker for environmental ambience

---

## What This Prototype Is (and Isn’t)

**This is**:
- A focused Unreal Engine 5 gameplay experiment
- A validation of system-driven player freedom
- A demonstration of C++ gameplay architecture

**This is not**:
- A finished game
- A general-purpose framework
- A showcase of Unreal’s full feature set

---

## Notes for Reviewers

- Systems are intentionally direct and readable
- Some logic is verbose by design to keep behavior explicit
- The prototype prioritizes control feel and timing over abstraction

---

## Author

Gameplay Programmer / Technical Artist  
(Code sample – gameplay systems exploration)
