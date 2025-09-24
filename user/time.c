#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

	// A buffer
	uint64 time_before = ctime();

	time_before = ctime();
	short POSIX = 0;
	short arg_start_idx = 1;
	if (argc == 1) {
		printf("0 seconds bih\n");
		return 1;
	} else if (argc >= 2){
		char *flag = "-p";
		if (strcmp(flag, argv[1]) == 0) {
			POSIX = 1;
			if (argc == 2) {
				printf("Still 0 bih\n");
				return 1;
			} else {
				arg_start_idx++;
				printf("Ay you doing sumn DIF\n");
			}
			
		}
	}

	int pid = fork();
	int ret = 0;
	if (pid == 0) {
		ret = exec(argv[arg_start_idx], &argv[arg_start_idx]);
	} else {
		wait(0);
	}

	uint64 time_after = ctime();
	uint64 final_time = time_after - time_before;
	printf("%ld\n", time_before);
	printf("%ld\n", time_before);
	uint64 minutes = final_time / 60000;
	uint64 seconds = (final_time % 60000) / 1000;
	uint64 ms = (final_time) % 1000;
	if (POSIX == 1) {
		printf("Aight u POSIX-ing\n");
		printf("Final Time: %ld\n OR...\n %ld minutes\n %ld seconds\n %ld ms\n", final_time, minutes, seconds, ms);
	} else {
		printf("NO POSIX\n");
		printf("Final Time: %ld\n OR...\n %ld minutes\n %ld seconds\n %ld ms\n", final_time, minutes, seconds, ms);	
	}
	
	return ret;
}
