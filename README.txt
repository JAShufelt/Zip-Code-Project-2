This program uses multiple .cpp and .h files, the .cpp with the main functions are "Initialization.cpp" and "Search.cpp"

*Initialization.cpp*
(must be compiled with buffer.cpp headerBuffer.cpp indexRecord.cpp and record.cpp)
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

*Search.cpp*
(must be compiled with indexRecord.cpp and record.cpp)

This program must be ran after the initialization program has been run.
In order for Search.cpp to function properly, there must be a _LENGTH-INDICATED data file
aswell as an _INDEX file for the corresponding .csv .

When executing, the first commandline argument must be the name of the .csv file.
Any additional commandline arguments will be checked if they are in the format of -Z(zip code).
If a CLA does not fit the convention of -Z(zip code) it will be ignored.

Example Compilation:
g++ Search.cpp indexRecord.cpp record.cpp -o Search.exe

Example Execution:
Search.exe us_postal_codes.csv -Z55449 -Z55014 -Z90210

You should get back a table of all zip code records found if you searched a real zip code, aswell as
a table of records not found if any fake zip codes were searched for. 



