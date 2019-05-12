# LuteController
Command line application for reading midi input and turning them into lute console commands in Mordhau. 

After compiling the program (or grab the already complied one under release folder if you dare), connect first your midi input device and then run the program and leave it open. If you press enter while in the command line the program will exit. All you need then is to just have the Mordhau and open the console, then you are ready to play. Do note that the lute can only play around 2 octaves.

NOTE: There is a tiny delay when playing the sounds due to the time it takes for the console to parse the command and play the sound. I tested the performance of the actual program and it takes around 2ms to recieve the MIDI message from my device and write the console command so I don't have any ideas how to reduce the latency. This is mostly noticeable when playing with high tempo, so better to just stick to a bit more relaxed tempo.
