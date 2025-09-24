#include "kernel/types.h"
#include "user/user.h"

void printtime(char *category, int raw_ms, int POSIX) {

	if (raw_ms <=0) {
		printf("%s\t%d", category, raw_ms);
	
}

int main(int argc, char *argv[]) {

	// A buffer
	ctime();

	uint64 time_before = ctime();
	short POSIX = 0;
	short arg_start_idx = 1;
	if (argc == 1) {
		timtog();
		printf("real	0m0.000s\n");
		printf("user	0m0.000s\n");
		printf("sys 	0m0.000s\n");
		return 1;
	} else if (argc >= 2){
		char *flag = "-p";
		if (strcmp(flag, argv[1]) == 0) {
			POSIX = 1;
			if (argc == 2) {
				printf("real	0.00s\n");
				printf("user	0.00s\n");
				printf("sys 	0.00s\n");
				return 1;
			} else {
				arg_start_idx++;
				printf("Ay you doing sumn DIF\n");
			}
			
		}
	}

	int pid = fork();
	int ret = 0;
	timtog();
	if (pid == 0) {
		ret = exec(argv[arg_start_idx], &argv[arg_start_idx]);
	} else {
		wait(0);
		uint64 time_after = ctime();
		uint64 final_time = (time_after - time_before) % 1000000;
		// printf("%ld\n", time_before);
		// printf("%ld\n", time_after);
		uint64 seconds = (final_time % 60000) / 1000;
		if (POSIX == 1) {
			//printf("Aight u POSIX-ing\n");
			uint64 p_ms = (((final_time) % 1000) / 10);
			printf("real	%ld.%lds\n", seconds, p_ms);
			printf("user	%ld.%lds\n", seconds, p_ms);
			printf("sys 	%ld.%lds\n", seconds, p_ms);
		} else {
			// printf("NO POSIX\n");
			uint64 minutes = final_time / 60000;
			uint64 ms = (final_time) % 1000;
			printf("real	%ldm%ld.%lds\n", minutes, seconds, ms);
			printf("user	%ldm%ld.%lds\n", minutes, seconds, ms);
			printf("sys 	%ldm%ld.%lds\n", minutes, seconds, ms);
		}
		ret = 1;
	}
	printf("%d\n", (getkt() % 1000000));
	return ret;
}
