This program uses multiple .cpp and .h files, the .cpp with the main function is "Initialization.cpp"
This program initializes many things and prepares the program so that the search function can operate easily

To initialize a CSV... 
it must be in the same folder as Initialization.cpp.
Execute the executable with the name of the CSV in the commandline. 

Example: compile the program on putty to create the executable (g++ all of the .cpp files together)
and then when you do "./a.out us_postal_codes.csv".

Anything you write after an executable is a commandline argument.
This is the only legal commandline argument for the executable.
if you make a typo in filename, then it will run forever. It shouldn't run very long,
usually the program runs for about 45-60 seconds for me, so give it time.

Once the program is running, in the same directory as the program files it should construct
two new files, the Length-Indicated CSV file, and the Index File.

if you were to run the program using "us_postal_codes.csv", you should now have two new files...
"us_postal_codes_LENGTH_INDICATED"
"us_postal_codes_INDEX"

You'll know if its working by looking in the folder and seeing if the Length Indicated CSV was created.
It should have a file size of 0 bytes until the program finishes.

The index file is organized so that each line contains one zipcode (primary key) followed by the byte
offset to the beginning of the record, within the corresponding Length-Indicated CSV file.

The header of the index file will contain all the fields, and in the proper order. This should make it easy
to write a program that can search the index and then reconstruct a record through direct seeking via the byte offset.


