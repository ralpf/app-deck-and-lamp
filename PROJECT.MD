# Project Context

Read AGENTS.MD first for collaboration rules and permission to edit files.
Use current code as the source of truth for implementation details.

## Purpose and Architecture

ESP32 firmware controls a lamp LED ring and a deck LED strip.
src/main.cpp is the reference for orchestration and visual style.

- C-style organization: explicit global state, direct execution flow, and free
  functions, including functions defined in feature headers.
- Settings and runtime state are grouped by domain. The main loop coordinates
  services and output updates, with supporting behavior in feature modules.
- C++ features such as templates, scoped enums, and small objects are welcome
  when they improve clarity. Keep abstractions proportional to the problem.
- Readability and maintenance come before brevity. Reuse across ESP32 projects
  matters, but similar code alone does not justify a new abstraction.
- Preserve visible data flow and consider dependencies between outputs and
  services when changing execution order.

## Formatting and Style

Preserve the surrounding code's style in edits and chat examples.

- Four-space indentation, function braces on their own lines, dotted section
  dividers, and generous vertical whitespace.
- Align related declarations or comments where the surrounding code does so.
  Switch case bodies use the same indentation as their labels.
- Follow local naming conventions; the project intentionally mixes styles.
- Prefer clear names and direct functions over compressed expressions or layers.
- Keep changes scoped. Avoid unrelated formatting, renaming, or cleanup.

These conventions guide improvements without requiring incidental mistakes to
be copied. Read the relevant modules before proposing structural changes.
