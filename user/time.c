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
		uint64 seconds = ((raw_ns % 60000000000) / 1000000000);
		// printf("SECONDS: %ld\n", seconds);
		if (POSIX) {
			uint64 p_ms = ((((raw_ns) % 60000000000) % 1000000000) / 10000000);
			if (p_ms < 10) {
				printf("%s\t%lu.0%lus\n", category, seconds, p_ms);
			} else {
				printf("%s\t%lu.%lus\n", category, seconds, p_ms);
			}
		} else {
			uint64 minutes = raw_ns / 60000000000;
			uint64 ms = ((raw_ns % 60000000000) % 1000000000) / 1000000;
			if (ms < 100) {
				if (ms > 9) {
					printf("%s\t%lum%lu.0%lus\n", category, minutes, seconds, ms);
				} else {
					printf("%s\t%lum%lu.00%lus\n", category, minutes, seconds, ms);	
				}
			} else {
				printf("%s\t%lum%lu.%lus\n", category, minutes, seconds, ms);
			}
		}
	}
	
}

int main(int argc, char *argv[]) {

	// A buffer
	ctime();
	ctime();
	ctime();

	// printf("%ld\n", ctime());
	// printf("%ld\n", ctime());
	// printf("%ld\n", ctime());

	short POSIX = 0;
	short arg_start_idx = 1;
	if (argc == 1) {
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

	timtog();
	uint64 time_before = ctime();
	int pid = fork();
	int ret = 0;
	if (pid == 0) {
		timtog();
		time_before = ctime();
		ret = exec(argv[arg_start_idx], &argv[arg_start_idx]);
	} else {
		uint64 utime, kerntime;
		wait2(0, &utime, &kerntime);
		uint64 time_after = ctime();
		// printf("before: %ld\n", time_before);
		// printf("after: %ld\n", time_after);
		// printf("kern: %ld\n", kerntime);
		// printf("user: %ld\n", utime);
		uint64 final_time = (time_after - time_before);
		printf("\nrt: %lu\n", final_time);
		printtime("real", final_time, POSIX);
		printf("ut: %lu\n", utime);
		printtime("user", utime, POSIX);
		printf("kt: %lu\n", kerntime);
		printtime("sys", kerntime, POSIX);
		ret = 1;
	}
	return ret;
}
