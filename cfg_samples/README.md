Mouse layouts on X11 and UInput both start at 272 (0x110), but these layouts are different.<br>
`config.json` Configured for X11 layout and сonfigures the clicker as follows:
 - Holding of `9 button (additional key)` trigger a LMK `1`
 - Holding of `8 button (additional key)` trigger a RMK `3`

UInput layouts has some differents. There is a key matching table:
| Key Name | X11 Keycode | UInput Keycode |
|----------|-------------|----------------|
| LMK      | 1           | 1              |
| RMK      | 3           | 2              |
| MIDDLE   | 2           | 3              |
| FORWARD  | 9           | 9              |
| BACK     | 8           | 8              |

<br>
This shit will be remade in the future.