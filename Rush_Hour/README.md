# Rush Hour Console Reverse Engineering

This is my first documented project on my process of reverse engineering a retro game console. I 
document my entire process from tracing the PCB to wiring it to a logic analyzer, finally trying to 
add my own components ( sort of this is a draft idk what the end goal is )

## Playing the Game

## Hardware Overview

This section documents the internal components of the console.

### Physical Design

- Inside of a plastic handheld case.
- Black and whtite LCD display.
- Speaker on the back.
- Operates using two AA batteries.

### Internal Components

- **Main PCB** contains a chip-on-board (COB).
![Main PCB - COB View](Assets/Hardware/COB_View.jpg)
- **LCD display** powered with horizontal zerba stripe connections.
![Main PCB - LCD View](Assets/Hardware/LCD_Veiw.jpg)
- **Speaker** wired to audio output.
- **Buttons** connected with ribbon cable.
![Button PCB - Button Interface](Assets/Hardware/Button_Interface.jpg)
- **Power section** includes:
![Power Section](Assets/Hardware/Power_Section.jpg)
    - 10 microfarad electrolytic capacitor (smoothing).
    - 0.1 microfarad ceramic caps (104) (decoupling).
    - Two metal oxide varistor (MOVs) for surge protection.
    - Various resistors.

## Circuit Schematics

## Logic Analyzer

## Ect

## Difficulties

## What I Learned

Where the COM pins were located because the multimeter averages volatages so I could tell that when an icon was blinking 
it would turn the voltage from high to low averaging out in the middle. I saw this on one of the COM pins when it was in an off
state verse on and located the COM pins.

That all the connections to the COB are not just for both input and output there are a lot of pins connected to capacitors to
keep a constant voltage and have less spikes.

The Buttons have one connection to VCC and GND and their ribbon cable. once the button is pressed it completes the circuit.
It is very simple and makes sense just it was my first time tracing it andexeriencing it in front of me so it was really cool.
