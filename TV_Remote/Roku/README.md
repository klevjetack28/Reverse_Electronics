# TV Remote Emulator

The goal of the project is the emulate the TV remote's IR LED output to create macros to simplify already simple tasks.

## About The Project

This project had one goal in mind. The ability to create a macro for my TV to select a streaming service, Hulu, and go to my favorites to watch the current favorite show, Modern Family.

I have had this goal in mind and it ended up being a very fun project. Just something silly  to put some of my engineering abilities to work. 

## Step 1

Starting the project took a clear path. The first thing I needed to do was inspect the PCB inside the remote. My initial discovery was a single microcontroller. This microcontroller is the (insert name) which I was easily able to find a datasheet for online. This is important because this microcontroller is popular for IR remotes and I was able to find the output pin for my remote.

## Step 2

The next step was to use my logic analyzer to analyze the sequence of marks and spaces the remote would send. Something important about using the logic analyzer is using the correct sample rate. An IR remotes sends its signals at 38 kHz so I need to sample the signal at two times, or higher, to accurately catch every signal. With the correct connections and the correct sample rate I could gather the samples. I would go through and press each button once to get a single sequence of the signal. I would do this for every button on the remote. After each press I would need to download the entire captured signal in some form of binary. I chose a binary instead of the other savage options because I thought it was simple to write a C script for. Once I downloaded I binary for each press I was done with the logic analyzer and was moving on to writing my script. 

## Step 3

Writing my script in C was once of the more challenging parts of the project. This required learning a lot about how IR remotes send signals. 

### Marks and Spaces

The first thing I needed to learn was how it sends signals. What a mark is and what a space is. A mark is when the carrier in the microcontroller pulses at the 38 kHz for a specified length of time. This means that when we want the LED to turn on the carrier will pulse. When we have what is called a space the signal will remain high. This is important in emulation because the IR receiver expects the LED to pulse. Otherwise it does not read it correctly. 

### Protocols

The other part you need to learn is the protocols that the microchips follow. Each IR remote uses predefined protocols for sending signals. It typically starts with a long mark followed by a long spaces and then the sequence will start. There is more to the sequence like at about halfway through it has another long space where it start to reverse the sequence as confirmation. 

###  Variability

The final thing I want to mention is about the variability in the signals typically they range from values like 560 microseconds for a mark or a space and 1690 microseconds for a longer mark. This is the precise times but have +-100 microseconds of leeway. This is important in my script because I sampled at 100kHz so I might have extra lows or highs for it capturing a value twice. This might lead me to having values of 540 or 1670. Even though it is not 100% we can see that these values are close to the original and within those margins of error. 


With these three points of research in mind we were able to perfect our script. It took me multiple iterations of the script to finally implement these three features.

I started with just capturing how many ticks the signal is high and how many it is low and  generating a dynamic array of even indexes as high and odd indexes as low. Once I learned about marks and spaces I had to parse my array that I generated. I needed to find a way to break the highs and lows I generated them and turn them into a protocol. Where small bursts of highs and lows I would compress into marks and anything long would be compressed into a space. In my script I took it a step further and parsed this new array into Arduino syntax so I could copy it all into the Arduino seamlessly. 

The other main goal of my script is that it would be able to run every file in the directory I stored the binaries and press them with one call of the script. Making it an easy process for me for smooth development. After that I mentioned how I parsed it all into Arduino syntax where I would write another bash script to concat each file contents into a new file and create a signal Map with the name of each array it parsed. Copy all of that to my clipboard so I didn’t need to do anything other than wait and press ctrl+v in Arduino if I have any changes. 

## Step 4

Arduino was lots of fun to work with. Taking a step back from my C script when I was first working on it I didn’t start by using every signal I just tried one and at random I chose the power button. Programming with the Arduino is when I had to research a lot about how the signal was sent and how I needed to change my script to accommodate.

### Programming

Programming on the Arduino started with one unsigned int16 array for my signal sequence. I needed to emulate sending the sequence and this involves pulsing during marks and staying high during spaces. This involved working with registers to set up the Timer on my pin for PWM. You can turn on and off this PWM so during marks you would turn it on and spaces you would turn it off. I also included separate button on the circuit to manually send the signal. This is included in my main loop that just check for button presses. There wasn’t really much code in the arduino it mainly just looped through the signal array and sent the signal as needed. The process that took the most time was just making sure it was pulsing correctly and the C script gave the correct array to parse through. 

### Circuit 

The circuit was really fun to work with. At first I made a simple circuit to just turn the IR LED. I connect the PWM pin to a resistor which is connected to the cathode of the led then ground it. This was my simple circuit to test the signal I was sending but I ran into an issue. The IR LED was not sending at a strong enough signal. It was not getting enough amperage to send the signal through long distances. I needed to put it right up against the IR receiver for it to interpret the signal. To deal with this I used a TIP102 Darlington transistor. This transistor is special because it boosts current on a system. This is exactly what I needed for my circuit. This was able to power the circuit from the coach instead of right against the receiver.

## Steps 5

Now that May’s script was automated and the circuit worked from the coach what do I do next? I set up 8 buttons for macros in my Arduino code. These macros are stored in an array that when the button is pressed it will loop through the signals one after another. The next issue I ran into was that the signals were going insta toy after each other so I needed to add a delay and wait for the signal to finish. In addition to this delay sometimes the macro might open an app and have a much larger delay than pressing the volume up five times. This meant that I changed the array my macro was stored in to a 2D array so set custom delays. 

Later I changed this to have a 4x4 button matrix. The purpose is because I wanted to decrease the number of wires the buttons took up. With adding buttons for macros and the ability to change the macros required as of right now 16 buttons. This is far too many inputs when I need to include an output wire for the IR LED as well as the LCD. By adding a button matrix I cut the number of inputs required in half to 8 inputs.

## Step 6

With the decrease in input wires the next challenge was dynamically changing the macros. I set up an LCD with the driver included using only 2 input pins. I would use 10 buttons to have macros 0 - 9 and the other 6 would be used for Up, Down, Left, Roght, OK, and Back. I would use the LCD as an interface for using the buttons to change the macros. Saving the macros to persist in storage was easy woth arduinos EEPROM. Woth a way to persist data on start I now needed a way to interface woth the macros and change them at will. I used two arrays one for the macros and one for the delays where the uses could select up and down which signal they wanted and the delay to follow. This means if your pressing Ok on an app and it takes 7 seconds to load you would select down to the "OK" signal and next be prompted woth the delay selecting the "DELAY_7500" delay. after selecting the delay it would be instantly saved to EEPROM. 

## Conclusion

With all this in mind I was able to create macros that communicate with my tv.

## Things I Learned

Some things I have started to learn from this project is that remotes are typically made up of a single HB8101pk microcontroller. This mirocontroller has a clock cycle of 38kHz so when I got a bitstream with a samplerate of 100kHz I was getting lots of sequences of 1's and 0's with a few sections that stays high for 56 samples or 169 samples. These correspond to when the IR LED is completely off called spaces. The flickers are called marks which is what the IR sensor interprets. To make everything fit on the arduino I can capture the sizes of the spaces and sizes and multiple the number of samples captured by 10. This is because at 100kHz each sample is captured at 10 microseconds per sample. 
