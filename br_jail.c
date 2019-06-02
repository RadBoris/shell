#include <stdio.h>
#include <unistd.h>

int enter_chroot(const char * root) {
	char cwd[256];
	chdir(root);
	 printf("entered %s", root);
	if (chroot(root) != 0) {
		perror("chroot");
		return 1;
	} else {
		if (chdir("../root") == 0){
			printf( "Directory changed to root\n");
			//execvp ("/bin/bash -i", "pwd");
			execl ("/bin/bash", "bin/bash", NULL);
		} else {
			if (getcwd(cwd, sizeof(cwd)) == NULL){
     				 perror("getcwd() error");
			}
    			else {
      			    printf("current working directory is: %s\n", cwd);
			    perror("Could not break of jail root");
			}
  		}
	}
}

int read_print(const char* path) {
	FILE *f;
	f = fopen(path, "r");
	if (f == NULL) {
		perror(path);
		return 1;
	} else {
		char buffer[50];
		while (fgets(buffer, sizeof(buffer), f)) {
			printf("%s", buffer);
		}
	}
	return 0;
}

int main(void) {
	read_print("/etc/hostname");

	if(enter_chroot("/root/test_chroot")) {
		return 1;
	}
	int val = read_print("/etc/hostname");
	return val;
}
