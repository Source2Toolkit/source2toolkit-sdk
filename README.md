# Source2Toolkit SDK

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![Website](https://img.shields.io/badge/Website-source2toolkit.net-blue)](https://www.source2toolkit.net)
[![Discord](https://img.shields.io/discord/1178027657594687608?color=7289da&logo=discord&logoColor=white)](https://source2toolkit.dev/discord)

**Source2Toolkit SDK** is the core development kit required for building plugins for Source2Toolkit.

It bundles everything you need — headers, SDK, hooking system and build helpers — so you can focus purely on writing your plugin.

👉 **Docs & guides:** https://www.source2toolkit.net

---

## What is this?

Source2Toolkit SDK is a lightweight development layer that provides:

- Preconfigured **s2sdk (CS2)** -- AlliedModders' Source 2 SDK, formerly hl2sdk's `cs2` branch  
- Integrated **KHook** hooking (virtual, vtable and function detours), shared with Metamod  
- Ready-to-use **Source 2 headers & interfaces**  
- Cross-platform build configuration  
- Simple plugin build system  

No setup. No hunting dependencies. Just build.

---

## Why use it?

- **Zero setup** – everything included (SDK, hooks, protobufs)  
- **Fast builds** – optimized CMake configuration  
- **Clean integration** – designed specifically for Source2Toolkit  
- **Cross-platform** – Windows & Linux support out of the box  
- **Minimal boilerplate** – create plugins in seconds  

---

## Quick Start

👉 Full docs: https://www.source2toolkit.net

### 1. Add SDK to your project

```bash
git submodule add https://github.com/Source2Toolkit/source2toolkit-sdk.git
git submodule update --init --recursive
```

### 2. Get the dependencies

**s2sdk** ([alliedmodders/s2sdk](https://github.com/alliedmodders/s2sdk), `cs2`
branch) comes with the SDK as the `vendor/s2sdk` submodule, pinned to the
commit the toolkit headers are built against -- the `--recursive` above
already fetched it, game protobufs included (s2sdk's own `.proto` files, which
the SDK generates into its `Protobufs` library). Nothing else to set up.

If you keep your own s2sdk checkout instead, point any one of these at it; the
first one set wins over the submodule:

```
S2SDK_CS2, S2SDKCS2, S2SDK, HL2SDK_CS2, HL2SDKCS2
```

`tools/deps.py` remembers that choice and keeps everything current:

```bash
python source2toolkit-sdk/tools/deps.py init
python source2toolkit-sdk/tools/deps.py update
```

`init` asks, once, whether s2sdk is a checkout you already have or the
submodule, and saves the answer in `deps.json`. `update` pulls your checkout
or puts the submodule on the pinned commit, moves `vendor/khook` to the KHook
commit metamod-source pins (anything else refuses to load on the toolkit core)
and moves `vendor/hl2sdk-manifests` to its latest. `deps.py status` shows what
is checked out. Answer up front with `--s2sdk submodule` when there is no
terminal.

---

### 3. Minimal plugin setup (4 lines)

```cmake
cmake_minimum_required(VERSION 3.18)
project(my-plugin CXX)

add_subdirectory(source2toolkit-sdk)
add_s2toolkit_plugin(my_plugin plugin.cpp)
```

---

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

Output:
```
my_plugin.stx
```

---

## What's Included

- **s2sdk (CS2)** -- the `vendor/s2sdk` submodule, or your own checkout  
- **Protobufs** -- the game's, generated from s2sdk's own `.proto` files  
- **KHook** (virtual, vtable & function hooks) -- as the `vendor/khook` submodule, headers only    
- **Tier0 / Tier1 / Mathlib**  
- **Schema system headers**  
- **Preconfigured compiler flags & linking**  

---

## Helper API

### add_s2toolkit_plugin

```cmake
add_s2toolkit_plugin(my_plugin plugin.cpp)
```

Automatically:
- Links SDK  
- Sets correct output (`.stx`)  
- Applies all required flags  

---

## Requirements

- CMake 3.18+  
- C++20 compiler  
- Python 3 and git (for `tools/deps.py`)  
- Source2Toolkit installed on server  

---

## Documentation

- Docs: https://www.source2toolkit.net  
- Getting Started: https://www.source2toolkit.net/docs
- API Reference: https://www.source2toolkit.net/docs/core-api

---

## Contributing

Fork the repository, branch off `main`, and open the pull request against
`main` -- `dev` is the maintainer's own working branch. The full walkthrough
(upstream remote, rebasing, what a pull request needs) is in
[CONTRIBUTING.md](CONTRIBUTING.md).

## License

This project is licensed under the GNU General Public License v3.0, with a
linking exception for Valve's engines and games, a dual-licensing exception
and an MIT exception for derivative works. See [LICENSE_INFO.txt](LICENSE_INFO.txt) for the terms
and [LICENSE](LICENSE) for the full GPLv3 text.

### Do I have to open-source my plugins?

No. The GPLv3 license carries an explicit MIT exception for derivative works:
your plugins, or anything built against the SDK, can stay closed-source or
commercial. The exception covers what you build on the toolkit, not the
toolkit itself -- a modified Source2Toolkit or SDK stays under the GPLv3.

---

<div align="center">
  <strong>Everything you need to build Source2Toolkit plugins. Nothing more.</strong>
</div>
