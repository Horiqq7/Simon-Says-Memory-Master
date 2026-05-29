# Simon Says - Ultimate Memory Master 🧠📟

> An interactive memory game implemented on the ATmega328P platform, featuring dynamic difficulty, hardware-generated audio, and permanent High Score storage.

▶️ **[Project Demo](https://youtube.com/shorts/Y8jYl1PYTbY)**

## 📖 Introduction
The **Simon Says - Ultimate Memory Master** project is an interactive memory game. The purpose of the project is to create a device capable of generating sequences of visual and auditory signals that the player must reproduce correctly. 

The project evolved from a simple memory game into a more advanced embedded system featuring:
* **Dynamic difficulty adjustment**
* **EEPROM High Score storage**
* **Hardware timer generated audio**
* **Multiple integrated peripherals** (ADC, UART, Timers, GPIO)

## ⚙️ Hardware Design
The hardware implementation is centered around the **ATmega328P Xplained Mini** development board, assembled on a breadboard for modular organization.

### Component List:
* 1x ATmega328P Xplained Mini
* 1x TM1637 4-Digit Display
* 4x LEDs (Red, Green, Blue, Yellow)
* 4x Push-buttons
* 1x Passive Buzzer
* 4x 330 Ohm Resistors
* Breadboard and jumper wires

### Pin Configuration:
| Component | Pins | Description |
| :--- | :--- | :--- |
| **Buttons** | `PD2`, `PD3`, `PD4`, `PD5` | GPIO inputs with internal pull-up resistors |
| **LEDs** | `PB0`, `PB1`, `PB2`, `PB3` | GPIO outputs |
| **Buzzer** | `PD6` | Timer0 `OC0A` output for hardware PWM audio |
| **TM1637 Display** | `PC0` (DIO), `PC1` (CLK) | Software bit-banging communication |
| **ADC Entropy** | `PC2` | Floating analog input for random seed generation |
| **UART** | `TX/RX` | Serial communication at 9600 baud rate |

## 💻 Software Design
The firmware was developed using the **avr-gcc** toolchain.

* **State Machine:** Sequential control flow loop managing *Idle, Sequence Generation, User Input, Validation*, and *Game Over* states.
* **EEPROM:** Permanent memory storage to save the High Score across power cycles.
* **Timers:** Non-blocking audio generation using `Timer0` in CTC mode.
* **Randomizer:** A pseudo-random algorithm prevents repetitive patterns (e.g., no LED appears >2 times in 4 steps), seeded by environmental ADC noise + user reaction time.

## 🚀 How to Run / Build
1. Clone this repository.
2. Open the project in your IDE (e.g., VS Code + PlatformIO).
3. Build and upload the firmware to your ATmega328P.
4. Open a Serial Monitor at `9600` baud to view debug logs, game levels, and EEPROM resets.

---
*Created for the Microprocessors (PM) Laboratory, 2026.*