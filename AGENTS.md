# Repository Guidelines

## Project Structure & Module Organization

This repository is an HC32F460xE firmware project migrated from Keil MDK toward CMake + `arm-none-eabi-gcc`.

- `CMakeLists.txt` is the top-level orchestrator only: it adds subdirectories and links the final `proj_hc32.elf`.
- `Platform/` contains CMSIS, startup, linker script, drivers, HAL, support code, and the legacy Keil project.
- `Arduino/` provides the Arduino-compatible core.
- `Libraries/` contains LVGL, Adafruit components, parser, logger, and device libraries. Each component target owns its include paths.
- `Simulator/App/` contains LVGL app pages, resources, and app utilities reused by firmware.
- `USER/` contains the firmware entry point and board-specific application logic.

## Build, Test, and Development Commands

- `cmake --preset gcc-debug` configures a Debug GCC build and generates `build/gcc-debug/compile_commands.json` for clangd.
- `cmake --build --preset gcc-debug` builds Debug firmware, plus `.elf`, `.hex`, `.bin`, and `.map`.
- `cmake --preset gcc-release` configures an optimized Release build.
- `cmake --build --preset gcc-release` builds the Release firmware image.

The legacy Keil project remains at `Platform/project/proj_hc32.uvprojx` for reference.

## Coding Style & Naming Conventions

Use C11 and C++17 in the CMake build. Keep CMake modular: add sources and include directories in the owning subdirectory target, expose headers with `PUBLIC` only when dependent modules include them, and avoid adding broad include lists at the top level.

Follow existing naming: lower_snake_case for C-style functions such as `slave_i2c_update`, PascalCase for many C++ classes, and `HAL_` or `HAL::` patterns for hardware abstraction code.

## Testing Guidelines

There is no unit test suite yet. Treat successful GCC builds as the minimum validation:

- `cmake --build --preset gcc-debug`
- `cmake --build --preset gcc-release`

For UI or LVGL page changes, also validate the simulator path if it is relevant to the change.

## Commit & Pull Request Guidelines

Recent commits use short imperative summaries, sometimes bilingual, such as `update add linux port` or `Refactor main and hal_init functions for improved readability`. Keep commits focused by subsystem.

Pull requests should include a concise description, build results, linked issue if applicable, and screenshots or recordings for UI-visible LVGL changes.

## Agent-Specific Instructions

Keep vendored-library edits minimal. Do not reformat generated resources or third-party code unless the task explicitly requires it.
