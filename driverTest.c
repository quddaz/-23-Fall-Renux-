#include <stdid.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
# define MAX_SIZE 10


int main(void){
	int td;
	char buf[MAX_SIZE];

	td = open("/dev/my_virt_de",O_RDWR);
	if(td < 0){
		printf("Fail: %s\n",strerror(errno)):
		return 0;
	}
	write(td,"hello",5);
	read(td, buf,5);
	printf("%s\n",buf);
	
	close(td);
	return 0;
}
