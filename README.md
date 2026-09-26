# Party-System X Beta

An experimental Android launcher for running **Vintage Story 1.21.6** natively on ARM64 phones, built on a fork of [dnbootstrap](https://github.com/artdeell/dnbootstrap).

> ⚠️ **Beta.** Expect bugs. This is a fan project and is not affiliated with or endorsed by Anego Studios / the Vintage Story team.
> **No game files are included.** You must own Vintage Story and supply your own copy.

---

## What's new in this build

- **Tries OpenGL ES 3.2 first.** If 3.2 fails to start, the launcher falls back to **ES 3.1**, which boots to the main menu with only the known minor bugs.
- Previous builds targeted ES 3.1 only.
- <!-- Add any other changes here: branding, fixes, etc. -->

---

## Tested hardware

This build was designed and tested on:

| | |
|---|---|
| **Phone** | Moto G Power 5G (2024) |
| **Android** | 15 |
| **CPU** | MediaTek Dimensity 7020 (ARM64) |
| **GPU** | IMG BXM-8-256 |
| **Renderer** | OpenGL ES (`.gles` build). Vulkan is not used due to known driver issues on this GPU. |

Other ARM64 phones may work, but they are untested. A community compatibility list is planned.

---

## Requirements

- An ARM64 Android phone
- A **Vintage Story account** (purchased from the official site or itch.io)
- The **Vintage Story 1.21.6** Linux `.tar.gz` archive

---

## Installation

1. **Buy Vintage Story** from the official website or itch.io if you haven't already.
2. **Download the game archive.** Get the **Linux `.tar.gz`** for version **1.21.6** from wherever you purchased it, and save it to your phone.
3. **Download the APK** from the [Releases](../../releases) page. Install it, and allow installs from unknown sources if prompted.
4. **Launch the app** and select the Vintage Story `.tar.gz` archive when asked.
5. If the game **reaches the main menu**, log in with your Vintage Story account.

That's it. Control and touch-layout configuration will be documented in a future update.

---

## Known issues

- <!-- List current minor bugs here -->

---

## Reporting bugs

Please open an [Issue](../../issues) and include your phone model, Android version, and whether the game started on ES 3.2 or fell back to 3.1.

---

## Credits

- [dnbootstrap](https://github.com/artdeell/dnbootstrap) by artdeell, the base this project is forked from
- Vintage Story by Anego Studios
- Port work by Raven Krystal / Blackfeather Studios
