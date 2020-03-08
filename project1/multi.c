// Multitasking Commander by Matthew Selva & Lorenzo DeSimone

/*
	STEP 2: Designating which processes are background or not (boolin')
	STEP 3: Display message when background things terminate
	STEP 4: cproclist
	STEP 5: get wasted with troy and maybe patricia
	STEP 6: update README
*/

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <stdbool.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <sys/time.h>
#include  <sys/resource.h>

#define LSIZ 1000
#define RSIZ 32
#define fname "multi.txt"
char* arr[128] = {};
char* backProc[32] = {};
int * status;
int procCount = 0;
long int BstartTime = 0;

//struct to hold the information of a single process
struct procInfo {
    char *process_name;
    int pid;
    int process_num;
    struct timeval start;
    struct rusage process_usage;
};

// Checks if a value is in an array
bool isValueInArray(char *val, char *arr[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return true;
	}
	return false;
}

// Stats function sets up the Statistics that will be implemented later
void Stats(struct timeval start,struct timeval end,long fault_init,long recl_init, long fault_post, long recl_post) {
	long fault_dif;
	long recl_dif;
	
	// Calculations for Statistics:
    	float time;
    	time =  ((float) end.tv_usec - (float) start.tv_usec)/1000 + ((float)end.tv_sec - (float) start.tv_sec)*1000;
    	fault_dif = (fault_post - fault_init);
    	recl_dif = (recl_post - recl_init);


    	// Printing the Statistics
    	printf("--- Satistics ---\n");
    	printf("Elapsed Time: %.3f milliseconds\n", time);
    	printf("Page Faults: %ld\n", fault_dif);
    	printf("Page Faults (reclaimed): %ld\n", recl_dif);
    	printf("--- End of Satistics ---\n\n");
}

// Helper function that splits strings into individual words
void StringSplitter(char str[]) {
	char* dst = (char *) malloc(strlen(str) + 1);
	strcpy(dst, str);
		
	char* tok = strtok(dst, " ");
	int i = 0;	
	while (tok != NULL) {
		i++;
		arr[i-1] = tok;
		tok = strtok(NULL, " ");
}}


// CommandHandler takes in a command name and runs it using execvp
void CommandHandler(int pid, char* command, char* env[]) {
	if (pid == 0) {
		if ( strcmp(command, "cpwd") == 0)  {
			printf("Current directory: %s\n", getcwd(command, 128));
		} else {
			execvp(command, env);
	}} 
	else {
		wait(NULL);
		printf("\n");
}}

// Checks current process for if it's running in the background
void backCheck(struct procInfo** ps) {
    	struct timeval start_timer;
    	struct timeval end_timer;
    	struct rusage info;
    	long fault_init, recl_init, fault_post, recl_post;
    	while (1) {
        	pid_t t = wait3(NULL, WNOHANG, &info);
        	if (t == 0) {
            		// Background Process still running
            		break;
        	}
        	if (t > 0) {
            		int processi;

            		for (int i = 0; i < procCount; i++) {
                		if (ps[i]->pid == t) {
                    		processi = i;
            		}}

		start_timer.tv_sec = BstartTime;
		gettimeofday(&end_timer, NULL);
		
		getrusage(RUSAGE_CHILDREN, &info);
        	fault_post = info.ru_majflt;
        	recl_post = info.ru_minflt;

            	printf("\n -- Job Complete [%d: %s] --\n", (ps[processi]-> process_num - 1), ps[processi]->process_name);
            	printf("Process ID: %d\n\n", t);
            	Stats(start_timer,end_timer,fault_init,recl_init,fault_post,recl_post); 
		procCount--;
        	} else {
            	break;
}}}

// backCommandRunner runs specified processes in the background
int backCommandRunner(int p, char* CommandName, char* CommandList[],struct procInfo** ps){
	struct procInfo* aprocess = (struct procInfo*) malloc(sizeof(aprocess));
    
    	aprocess->process_name = CommandName;
    
    	struct timeval start_timer;
    	gettimeofday(&start_timer, NULL);
    	aprocess->start = start_timer;
	BstartTime = start_timer.tv_sec;

    	int pid = fork();
    	aprocess->pid = pid;
    	aprocess->process_num = procCount + 1;

    	if  (pid == 0) {
	 	CommandHandler(pid,arr[0],arr);
		int parent = getppid();
		_exit(parent); 
        }

    	ps[procCount] = aprocess;
    	procCount++;
}

// CommandRunner takes in a command and sends it to CommandHandler to be executed
void CommandRunner(char* arr[], char* name, struct procInfo** ps) {
	struct timeval start_timer;
    	struct timeval end_timer;
    	struct rusage info;
    	long fault_init, recl_init, fault_post, recl_post;

	printf("\nRunning Command: %s\n", name);
   	
	// Stats that will be transferred to the child
   	getrusage(RUSAGE_CHILDREN, &info);
   	fault_init = info.ru_majflt;
    	recl_init = info.ru_minflt;
        gettimeofday(&start_timer, NULL);

	// Change working directory if "ccd" is detected
	if ( strcmp(arr[0], "ccd") == 0) {
		chdir(arr[1]);
		printf("Changed to directory: %s\n", arr[1]);
	}

	// Process the "cproclist" command	
	if ( strcmp(arr[0], "cproclist") == 0) {
		printf("-- Background Processes --\n");
		int z;
		for (z = 0; z < 32; z++) {
			if(backProc[z] != NULL) {
			printf("[%d] %s\n", (z - 1), backProc[z]);
	}}}

	// Creating the child process, and preventing the parent from running in CommandHandler
        int pid = fork();
	if (pid == 0) {
	        CommandHandler(pid,arr[0],arr);
		int parent = getppid();
		_exit(parent);
	} else {
		wait(NULL);
		printf("\n");
	}

	// Calculate the final stats
	gettimeofday(&end_timer, NULL);
        getrusage(RUSAGE_CHILDREN, &info);
        fault_post = info.ru_majflt;
        recl_post = info.ru_minflt;

	// Return the final stats if necessary
	if ( (strcmp(arr[0], "ccd") == 0) || (strcmp(arr[0], "cpwd") == 0) || (strcmp(arr[0], "cproclist") == 0) ) {       
		printf(" ");
	} else {
		Stats(start_timer,end_timer,fault_init,recl_init,fault_post,recl_post);
}}

int main(int argc, char *argv[]) {
	struct procInfo** procList = (struct procInfo**) malloc(RSIZ*sizeof(procList));
	char line[RSIZ][LSIZ];
	FILE *fptr = NULL; 
	int i = 0;
	int tot = 0;
	fptr = fopen(fname, "r");

	// Assign comand line arguments to an array
	int CLNums[32] = {};
	int k = 0;	

	// Breaks the text file down by each line
	while(!feof(fptr)) {
		if(fgets(line[i], LSIZ, fptr) != NULL) {

			// Returns errors if the text file is too large
			if (strlen(line[i]) > 128) {
				printf("\nERROR: One of your lines is too long.\n\n");
				exit(0);
			}
			if (i > 32) {
				printf("\nERROR: Too many lines.\n\n");
				exit(0);
			}

			line[i][strlen(line[i]) - 1] = '\0';
        		i++;
		}}

	// Converts each line of the text file into arrays of words, which are then sent to CommandRunner
	tot = i;
	for(i = 0; i < tot; ++i) {
		// Checking command line to see if a process should run in the background
		for(k = 1; k < argc; ++k) {
			CLNums[k] = (atoi(argv[k]));
			if(i + 1 == CLNums[k]) {
				printf("\nRunning command: %s\nBackground: ID [%d]: %s\n", line[i], (k - 1), line[i]);
				backProc[k] = line[i];
			}}
	
		StringSplitter(line[i]);

		if( (strcmp(arr[0], "whoami") == 0) || (strcmp(arr[0], "last") == 0) || (strcmp(arr[0], "ls") == 0) || (strcmp(arr[0], "ccd") == 0) || (strcmp(arr[0], "cpwd") == 0) || (strcmp(arr[0], "sleep") == 0) || (strcmp(arr[0], "cproclist") == 0)) {	
			if (isValueInArray(line[i], backProc, sizeof(backProc))) {
				backCommandRunner(100, line[i], backProc, procList);
			} else {
				CommandRunner(arr, line[i], procList);
		}} else {
			printf("\nERROR: '%s' is an Invalid Command\n\n", line[i]);
}}
	while(procCount > 0) {
		backCheck(procList);
}}
