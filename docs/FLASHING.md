# Experimental RC52 flashing

> **RC52 only—do not flash RCC6, RC32, or RC52 variants with different display/radio hardware.**

There is no endorsed release binary yet. These instructions are for a specifically identified RC52-L62 qualification unit and an exact GitHub Actions artifact.

1. Confirm the connected device is the intended Heltec RC52 and record its stable USB identity.
2. Download the artifact from the successful `RC52 Build` run for the exact source commit.
3. Verify `firmware.uf2` against the artifact’s `SHA256SUMS`.
4. Attach a suitable 868/915 MHz antenna before allowing LoRa transmission.
5. Enter the UF2 bootloader with the board’s reset sequence or a 1200-baud touch.
6. Confirm the mounted UF2 volume identifies an nRF52840 RC52 bootloader.
7. Copy only `firmware.uf2` to that volume.
8. Do not erase flash and do not write a bootloader, SoftDevice, or merged image.
9. After reboot, verify the exact application through TFT, BLE, USB stability, and LoRa receipts before treating the flash as successful.

If the screen stays blank, the device resets repeatedly, or USB identity changes unexpectedly, stop. Preserve the unit and capture its 115200-baud boot output instead of reflashing blindly.
