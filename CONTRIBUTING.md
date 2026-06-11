# Contributing to GB Emulator

We welcome contributions! Please follow these guidelines to ensure a smooth collaborative process.

## Branch Naming

Use the following prefixes for your branches:
- `feat/` for new features (e.g., `feat/ppu-rendering`)
- `fix/` for bug fixes (e.g., `fix/timer-interrupt`)
- `docs/` for documentation updates
- `refactor/` for code refactoring

## Style Guide

- Follow standard C++ guidelines (C++17 or later).
- Use `snake_case` for variables and functions.
- Class names should use `PascalCase`.
- Provide meaningful commit messages describing *what* and *why*.
- Format code using the project's formatting tools before committing.

## Review Rules

- All pull requests must be reviewed by at least one maintainer.
- Ensure all tests pass (`ctest` or `./build/bin/emu_tests`) before requesting a review.
- Provide a clear description of the changes in the PR template.
- Resolve all comments and discussions before merging.
