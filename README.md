# NeonPocketMC-RC52

Experimental MeshCore 1.17 BLE companion firmware for the Heltec RadioCore RC52-L62 with its vendor 220 x 128 NV3001B TFT.

> **RC52 only—do not flash RCC6, RC32, or RC52 variants with different display/radio hardware.**

## Experimental status

This repository is public for development and hardware qualification. There is **no endorsed binary release yet**. Do not distribute an Actions artifact as a release build. The first release candidate will be published only after the exact artifact passes the hardware checklist below.

The port is pinned to MeshCore 1.17.0 at upstream commit [`727fc0512ce08bfd7b499e46daa7fca6eeec730d`](https://github.com/meshcore-dev/MeshCore/commit/727fc0512ce08bfd7b499e46daa7fca6eeec730d). The RCC6 project supplied the visual reference only; its ESP32 hardware, Web/AP transport, and release history are not included.

## What is included

- One target: `heltec_rc52_companion_radio_ble`
- BLE companion transport only
- Native 220 x 128 RGB565 NeonPocket dashboard and inbox
- One hardware-SPI framebuffer with 8-row delta flushing
- Home, Inbox, Nearby, Radio, Bluetooth, Advert, and Power pages
- Local direct and `#channel` unread tracking with a bounded `32+` cache
- 60-second TFT timeout while BLE and LoRa continue running
- Warning below 3.45 V, cleared above 3.60 V; no automatic low-voltage shutdown
- Fail-closed display, memory, radio, and filesystem startup checks

Web/AP, USB companion, headless, and alternate RC52 hardware variants are deliberately out of scope.

## Hardware contract

| Function | Pins / behavior |
| --- | --- |
| TFT SPI1 | SCK 30, MOSI 35, CS 37, DC 28, reset 10 |
| TFT power | enable 45 active-low, backlight 9 active-high |
| SX1262 SPI | SCK 25, MISO 14, MOSI 22, CS 13 |
| SX1262 control | DIO1 11, BUSY 24, reset 32, RXEN 39 |
| Radio power | FEM 26, VFEM 16 |
| User button | 42, active-low |
| Battery ADC | control 4, ADC 31, provisional multiplier 4.9 |

The radio uses DIO2 RF switching, DIO3 at 1.8 V for the TCXO, DC-DC mode, and the RC52 radio-power startup sequence.

## Button behavior

- Screen off: the first gesture wakes the TFT and is consumed.
- Single press: next tab, inbox item, or message page.
- Double press: the current page action, including opening Inbox, toggling BLE, or sending a flood-scoped Advert.
- Long hold: show Power confirmation.
- Second long hold within eight seconds while confirmation is visible: enter nRF52 system-off after button release.

## Build

GitHub Actions is the supported build path. The `RC52 Build` workflow:

1. checks out the exact branch commit rather than a synthetic pull-request merge;
2. runs the upstream native unit tests;
3. builds `heltec_rc52_companion_radio_ble`;
4. verifies the UF2 family and application start address;
5. regression-builds `Heltec_t114_companion_radio_ble`; and
6. publishes short-lived `.uf2`, `.hex`, checksum, and UF2-info artifacts.

The artifact is application-only. It must be copied through the RC52 UF2 bootloader. **Never erase or replace the bootloader or SoftDevice.** See [docs/FLASHING.md](docs/FLASHING.md).

## Release gate

Before `v1.0.0-rc.1`, the exact Actions artifact must pass:

- three clean resets with stable USB and an upright, uncropped TFT;
- framebuffer allocation plus a recurring 16 KB diagnostic allocation;
- 30 minutes of BLE-authenticated sync and LoRa activity without reset or display corruption;
- all single/double/hold/wake/power-confirm actions;
- BLE pairing, sync, disconnect, and reconnect;
- direct and channel messages in both directions;
- independent reception of a flood-scoped Advert;
- LoRa TX and RX receipts through the Pi COM11 observer;
- battery ADC comparison against a multimeter; and
- verified 60-second TFT power-off.

After acceptance, the release will contain the exact `.uf2`, `.hex`, `SHA256SUMS`, source/license bundle, flashing instructions, and new straight-on photos of the running UI.

## License and upstream

This is a derivative of [MeshCore](https://github.com/meshcore-dev/MeshCore). Preserve the upstream license notices and third-party licenses when redistributing source or binaries. The repository’s top-level license is in [license.txt](license.txt).
