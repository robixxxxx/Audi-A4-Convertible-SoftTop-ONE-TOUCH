# Audi A4 Convertible Soft Top Basic ONE TOUCH Module
This project provides an Arduino-based solution to control the roof of an Audi A4 Cabrio using original roof button. The provided code manages the open and close operations of the roof by double-click .

## 1. Hardware Requirements
- Arduino board (e.g., Arduino Uno)
- Two BC547 transistors
- Two 1kOhm resistors
- Connecting wires
- Male goldpin 2x3
- Female golpin 2x3

### The wiring diagram is available in the schematics folder

## 2. Software

### Installation
1. Install the [Visual Studio Code](https://code.visualstudio.com) with [PlatformIO](https://platformio.org) extension or [Arduino IDE](https://www.arduino.cc/en/software)
2. Connect your Arduino board to your computer via USB.
3. Open IDE and clone repository or create a new project and copy the code
5. Verify and upload the code to your Arduino

### Functionality
- Button Debouncing: Prevents false triggering of button presses.
- Button Sequence Handling: Manages the sequence of button presses to ensure the correct operation of the roof.
- Safety Timeout: Automatically stops the roof if it moves beyond the predefined time.
### Usage
- Opening the Roof: Press the "Open" button twice within 800ms to start opening the roof. Pressing the button more than twice will stop the roof.
- Closing the Roof: Press the "Close" button twice within 800ms to start closing the roof. Pressing the button more than twice will stop the roof.
## Ensure the vehicle is parked and it is safe to operate the roof before using this system.