#include "kernel/types.h"
#include "user/user.h"

void printtime(char *category, int raw_ns, int POSIX) {

	if (raw_ns <=0) {
		if (POSIX) {
			printf("%s\t0.00s\n", category);
		} else {
			printf("%s\t0m0.000s\n", category);
		}
	} else {
		uint64 seconds = (raw_ns % 60000000000) / 1000000000;
		if (POSIX) {
			uint64 p_ms = (((raw_ns) % 60000000000) / 100000000);
			if (p_ms < 10) {
				printf("%s\t%ld.0%lds\n", category, seconds, p_ms);
			} else {
				printf("%s\t%ld.%lds\n", category, seconds, p_ms);
			}
		} else {
			uint64 minutes = raw_ns / 60000000000;
			uint64 ms = (raw_ns % 60000000000) / 10000000;
			if (ms < 100) {
				printf("%s\t%ldm%ld.0%lds\n", category, minutes, seconds, ms);
			} else {
				printf("%s\t%ldm%ld.%lds\n", category, minutes, seconds, ms);
			}
		}
	}
	
}

int main(int argc, char *argv[]) {

	// A buffer
	ctime();

	printf("%ld\n", ctime());
	printf("%ld\n", ctime());
	printf("%ld\n", ctime());

	uint64 time_before = ctime();
	short POSIX = 0;
	short arg_start_idx = 1;
	if (argc == 1) {
		timtog();
		printtime("real", 0, 0);
		printtime("user", 0, 0);
		printtime("sys", 0, 0);
		return 1;
	} else if (argc >= 2){
		char *flag = "-p";
		if (strcmp(flag, argv[1]) == 0) {
			POSIX = 1;
			if (argc == 2) {
				printtime("real", 0, 1);
				printtime("user", 0, 1);
				printtime("sys", 0, 1);
				return 1;
			} else {
				arg_start_idx++;
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
		uint64 final_time = (time_after - time_before);
		// printf("\nrt: %ld\n", final_time);
		printtime("real", final_time, POSIX);
		// printf("ut: %ld\n", getut(pid));
		printtime("user", getut(pid), POSIX);
		// printf("kt: %ld\n", getkt(pid));
		printtime("sys", getkt(pid), POSIX);
		ret = 1;
	}
	return ret;
}
