# ESP32 TV LED strip — effects implementation brief

## Purpose and hardware

Implement selected AlexGyver-inspired ambient effects **inside the existing ESP32 LED-controller project**. The TV installation is approximately **120 individually addressable LEDs in one linear sequence** (1 × ~120), **not** a 16 × 16 matrix. Exact count and physical direction must come from the existing project configuration. Do not replace the firmware, LED driver, state synchronization, or browser UI with GyverLamp's.

This is a shortlist of usable effects, **not** a request to implement everything at once. No source code is included here; inspect the linked source and the current project before implementation.

## A. Directly suitable for a 1D strip

These concepts already appear in AlexGyver's **strip-oriented GyverLight**, or in the original GyverLamp code without requiring meaningful vertical geometry. Port the rendering *idea* into our existing architecture, not its hardware/UI setup.

| Effect | Source | Required behavior / relevant controls |
|---|---|---|
| Wandering light (`Блуждающий огонёк`) | GyverLight | One bright point travels back/forth or wanders along LED indices; speed, color, glow/tail size. |
| Fireflies (`Светлячки`) | GyverLight | Several independent small lights appear/move/fade; population, speed, lifetime, colors. |
| Confetti (`Конфетти`) | GyverLight; original GyverLamp `sparklesRoutine()` | Random pixels light up in different colors and fade; spawn rate, decay. Original matrix implementation uses random `x,y`: replace with one LED index. |
| Moving rainbow (`Радуга`) | GyverLight; original GyverLamp `rainbowHorizontal()` | Spatial hue gradient that changes with time; speed, spatial scale. Use the **horizontal**, not vertical, matrix variant. |
| Color change (`Смена цвета`) | GyverLight; original GyverLamp `colorsRoutine()` | Whole strip changes color smoothly; speed, optional palette. |
| Static color (`Цвет`) | Original GyverLamp `colorRoutine()` | Whole strip one selected color; brightness/color. Only add as an effect if not already present in our controller. |

## B. Suitable, but **rework the 2D effect into a 1D algorithm**

Names below are listed in the **59-effect community GyverLamp firmware**; that list is not proof that the original implementation works unchanged on a strip. A Codex agent must inspect the relevant community source, or implement the specified 1D interpretation. Do not emulate an artificial 2D matrix just to flatten its output.

| Effect / original name | 1D implementation target |
|---|---|
| Comet (`Комета`), white/single-color variants | Moving bright head with fading trail; color, speed, tail length, direction. |
| Two/three comets (`Две кометы`, `Три кометы`) | Multiple independently moving heads/trails; allow opposite directions. |
| Pulsating comet (`Пульсирующая комета`) | Comet whose intensity/size oscillates as it moves. |
| Pulse (`Пульс`), including rainbow/white variants | Whole-strip brightness pulse; optional changing hue/palette, period, min/max brightness. |
| Waves (`Волны`), sinusoid (`Синусоид`) | Spatially varying color/brightness wave traveling along LED indices; wavelength, speed, palette. |
| Plasma (`Плазма`) | Combine time-varying 1D waves or smooth noise, map result through palette; speed, spatial scale. |
| Clouds (`Облака`), lava (`Лава`), ocean (`Океан`), forest (`Лес`) | Slowly evolving 1D noise fields; distinguish by palette, scale, and motion. Avoid making four copies of an identical effect with only a different display name unless preset-based design is deliberate. |
| Metaballs (`Метаболз`) | Several moving 1D influence centers; overlapping fields merge smoothly; count, radius, speed, palette. |
| Paintball (`Пейнтбол`) | Random colored patches appear and blend/fade along the strip; patch size, spawn rate, decay. |

## C. FIRE EFFECTS — separate implementation group

**Use AlexGyver/FireFX first, rather than trying to collapse the matrix fire from GyverLamp.** FireFX explicitly identifies these five as effects for a **1D addressable strip**:

| Exact FireFX name | Source author's description / intended behavior |
|---|---|
| `fireLine` | Each LED is an independent fire source (author's own algorithm). |
| `fireLinear` | Fire moves/propagates linearly along the strip. |
| `fireLineNoise` | Each LED is an independent fire source, using Perlin noise. |
| `fireLinePerlin` | Smooth fire driven by Perlin noise across the strip. |
| `fireLineZones` | Fire divided into a fixed number of zones. |

Implement them as **five separately selectable variants if requested**, or use one shared fire effect with clearly named algorithm modes. Preserve the visual differences. Useful configurable dimensions: animation speed, flame intensity/brightness, noise/spatial scale or zone count where applicable, and fire palette (classic red/orange/yellow; optional other color themes). Do not imply that GyverLamp's 2D `fireRoutine()` is a ready-to-port 1D fire effect.

## Implementation constraints for Codex

- Read the actual current project first; follow its existing effect interface, render loop, LED buffer, brightness/current limiting, and UI/state conventions.
- Treat the LEDs as **one ordered array**; bounds-check positions at 0 and `LED_COUNT - 1`. For moving particles choose explicit reflect, wrap, or despawn behavior and make it consistent.
- Avoid blocking delays; use elapsed time/frame delta for stable animation speed. Keep effect state independent and reset it appropriately on mode changes.
- Prefer inexpensive per-pixel calculations and reusable palette/noise/fade helpers. At ~120 LEDs there is no need for a 2D buffer or physical simulation.
- Keep effect speed and brightness adjustable. Add other parameters only when they materially control the visual result. Respect the project's existing power and brightness limits.
- Do not transplant GyverLamp's Wi-Fi, buttons, alarms, matrix-coordinate utilities, app protocols, or firmware configuration.
- Before copying upstream code, check each repository's license and preserve required notices.

## Primary references — inspect these when implementing

1. **1D general effects: GyverLight** — https://github.com/AlexGyver/GyverLight (README enumerates wandering light, fireflies, color change, rainbow, confetti, flame; firmware under `firmware/`).
2. **1D fire algorithms: FireFX** — https://github.com/AlexGyver/FireFX (README explicitly enumerates the five 1D functions above; code under `firmware/`).
3. **Original GyverLamp code** — https://github.com/AlexGyver/GyverLamp/blob/master/firmware/GyverLamp_v1.5.5/effects.ino (`sparklesRoutine`, `rainbowHorizontal`, `colorsRoutine`, `colorRoutine` are relevant; its fire and other effects use a matrix).
4. **Community 59-effect names and firmware link** — https://alexgyver.ru/gyverlamp/ (section `Прошивка 59 эффектов в 1` → community firmware post). Community-firmware effects in section B are **design candidates requiring 1D adaptations**, not verified drop-in functions from the original repo.
