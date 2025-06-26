# TV Remote Emulator

The goal of the project is the emulate the TV remote's IR LED output to create macros to simplify already simple tasks.

## Parts

- [ ] Arduino
- [ ] Arduino SD module
- [ ] 220 ohms resistor
- [ ] A couple of wires
- [ ] 18650s for power source 

## Things I Learned

Some things I have started to learn from this project is that remotes are typically made up of a single HB8101pk microcontroller. This mirocontroller has a clock cycle of 38kHz so when I got a bitstream with a samplerate of 100kHz I was getting lots of sequences of 1's and 0's with a few sections that stays high for 56 samples or 169 samples. These correspond to when the IR LED is completely off called spaces. The flickers are called marks which is what the IR sensor interprets. To make everything fit on the arduino I can capture the sizes of the spaces and sizes and multiple the number of samples captured by 10. This is because at 100kHz each sample is captured at 10 microseconds per sample. 
