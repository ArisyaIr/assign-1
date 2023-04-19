#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>

#define QUANTITY_LENGTH 4
#define QUANTITY_WAIT_INTERVAL 2

void getQUANTITY(char quantity[QUANTITY_LENGTH + 1]){
srand(getpid() + getppid());
 
quantity[0] = 49 + rand() % 7;

for(int i=1 ; i<QUANTITY_LENGTH; i++){
quantity[i] = 48 + rand() % 7;
}

quantity[QUANTITY_LENGTH] = '\0';
}

void sigint_handler(int sig);
int main()
{
signal(SIGINT, sigint_handler);

while(1){
int pipefds[2];
char quantity[QUANTITY_LENGTH + 1];
char buffer[QUANTITY_LENGTH + 1];
char string[100];

char s[100];

pipe(pipefds);
pid_t pid = fork();

if(signal(SIGINT, sigint_handler) == SIG_ERR
){
perror("signal");
exit(1);
}

if(pid==0){
getQUANTITY(quantity);
close(pipefds[0]);
write(pipefds[1], quantity, QUANTITY_LENGTH + 1);

printf("Please enter your pet's name :\n");
fgets(string, 100, stdin);
printf("Your pet's name is :\n%s", string);
printf("Generating quantity in parents and send to child...\n");

sleep(QUANTITY_WAIT_INTERVAL);

exit(EXIT_SUCCESS);
}

if(pid>0){
wait(NULL);

close(pipefds[1]);
read(pipefds[0], buffer, QUANTITY_LENGTH +1);

printf("Child received quantity '%s' from parent...\n\n", buffer);
}
}
return EXIT_SUCCESS;
}

void sigint_handler(int sig){
printf("ALERT!!! PROCESS IS BEING INTERRUPTED!!!\n");
exit(1);
}

