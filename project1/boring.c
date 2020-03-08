// Boring Commander by Matthew Selva & Lorenzo DeSimone

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <sys/time.h>
#include  <sys/resource.h>


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

// CommandHandler takes in a command name and runs it using execvp
void CommandHandler(int pid, char* command, char* env[]) {
    	if (pid == 0) {
        	execvp(command, env);
   	} else {
	        wait(NULL);
        	printf("\n");
    }
}


// CommandRunner takes in a command and sends it to CommandHandler to be executed
void CommandRunner(char* arr[], char* name) {
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

	// Creating the child process
        int pid = fork();
        CommandHandler(pid,arr[0],arr);
        

	// Return the final stats of the child
	gettimeofday(&end_timer, NULL);
        getrusage(RUSAGE_CHILDREN, &info);
        fault_post = info.ru_majflt;
        recl_post = info.ru_minflt;
        Stats(start_timer,end_timer,fault_init,recl_init,fault_post,recl_post);
}


int main(int arc, char* argv[]) {
	char* whoami[] = {"whoami", NULL};
	char* last[] = {"last", "-n", "1", NULL};
	char* ls[] = {"ls", "-al", "/home", NULL};

    	CommandRunner(whoami, "whoami");
	CommandRunner(last, "last");
	CommandRunner(ls, "ls -al /home");
}
