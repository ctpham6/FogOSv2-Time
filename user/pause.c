#include "kernel/types.h"
#include "user/user.h"

int main() {

	printf("Looping...\n");
	uint64 a = 0;
	for (int i = 0; i < 1550000000; i++) {
		// printf("ctime: %ld\n", ctime());
		a++;
	}
	printf("Done!\n");
	return 1;
	
}
