Matthew Selva & Lorenzo DeSimone - The Three Commanders

|=|=|=|=|=|=| Overview |=|=|=|=|=|=|

There are Three Commanders that can be run, each different from the other.
Each commander has its own commands that make it unique.

The boring commander runs three commands on its own with little interaction with the user beyond displaying information.
	whoami: Prints the username of the primary user of the machine
	last: Prints the most recent login information
	ls: Prints all files in the specified directory (-al /home)

The custom commander reads a text file, custom.txt, and runs the commands in order. The custom commander also implements two new custom commands. Similar information is displayed as with the boring commander.
	ccd: Changes the current directory
	cpwd: Prints the working directory

The multitasking commander will do everything the Custom Commander does, but with two additional features: the ability to run background commands and the ability to print the running job(s).




|=|=|=|=|=|=| Testing |=|=|=|=|=|=|

Our primary form of testing was simply compiling the code, running it, and assessing the results. For example, we tested each of the specific commands that can be run through the following proccesses:
  -We tested 'whoami' by ensuring the output was the correct username of the system user.
  -We tested 'last' by logging out of our Virtual Machine to ensure the correct information was then displayed.
  -We tested 'ls' by comparing the printed results to the actual contents of the directory.
  -We tested 'ccd' using the 'ls' command, checking if the correct files were listed in the directory we ccd'd to.
  -We tested 'cpwd' in a similar way to the above process, though in this case we just ensured the correct working directory was printed.
  -We tested 'sleep' by ensuring the printed termination stats matched the time we specified when calling the command.
  -We tested 'cproclist' by simply making sure the proccesses that were printed matched with the lines specified in the command line.

Aside from the commands themselves, we tested our text files in many ways. Our primary goal was ensuring our functions could successfully read and process the information in the text file, regardless of extraneous information. For example, whitespace, invalid commands, and other inputs that aren't properly executable, will not disrupt our function from reading from the text file. We tested this in various ways, primarily by inputting obscure text on various lines of our text files. We also tested changing the order of the information in our text files, to ensure the function would process correctly regardless of the order commands are implemented.

Regarding file size, we were given a limit of 32 lines, 128 characters each. Our program returns errors in custom.c and multi.c if the text file's size exceeds these limits. Testing this was trivial - simply making sure the program runs properly as long as the limits are not exceeded.

For smaller problems, such as helper functions, we regularly tested these, such as StringSplitter, by using the printf function to print the results, typically the contents of an array, to ensure they were what we expected.



|=|=|=|=|=|=| How did you keep track of Background Processes? |=|=|=|=|=|=|

We kept track of Background Processes using both an array, and a struct. We built a struck to track the stats of each process (including the command itself), and made an array containing the information on each background process. We then simply started to run these background commands normally, while constantly looping through a function that checks the status of the process (using wait3). Once a process finished, the stats were printed accordingly. When cproclist is called, the contents of the aforementioned array are printed, properly listing the name of each background process.




