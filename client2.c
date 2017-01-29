#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include <netinet/tcp.h>
#include<fcntl.h>


int main(int argc,char *argv[])
{
	struct sockaddr_in server;
	struct stat obj;
	int sock;
	int c=0;
	int choice=1,view,flag=1;
	char buf[100], command[5], filename[20], *f,name[20];
	int k, size, status,pass;
	int filehandle;

	if(argc!=2){
		printf("Format of input: ./client portNumber\n");
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
	      printf("socket creation failed");
	      exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	k = connect(sock,(struct sockaddr*)&server, sizeof(server));
	if(k == -1){
	      printf("Connect Error");
	      exit(1);
	}
	int i = 1;

	int y=0;
	//system("vlc closer.mp3 -f --fullscreen");
	system("clear");

	printf("\n\n---------- AUDIO STREAMING ------------\n\n\n");
   

	system("clear");

	while(choice==1){

		printf("1. List of tracks\n2. Play the track\n3. Exit\nEnter your choice: ");
		scanf("%d",&choice);

		switch(choice){

		case 1: y=1;
			char buf1[100];
			printf("Are we there yet\n");
			strcpy(buf1, "find . -name '*.mp3'");
        		send(sock, buf1, 100, 0);
			recv(sock, &size, sizeof(int), 0);
			printf("%s\n",buf1);
        		f = malloc(size);
        		recv(sock, f, size, 0);
			filehandle = creat("temp.txt", O_WRONLY);
			//system("chmod +x temp.txt");
			write(filehandle, f, size, 0);
			close(filehandle);
			system("clear");
		        printf("The tracks are: \n");
		        if(c==0){
				c=1;
        			system("cat temp.txt>file1.txt");
        			system("cat temp.txt");
        		}
			else system("cat file1.txt");

			break;

		
		case 2:	printf("Enter the name of the track(along with folder specifics): ");				//taking the filename as input
			scanf("%s",filename);
		
			strcpy(buf, "get ");							
			strcat(buf, filename);
			strcat(buf, ".mp3");



			send(sock, buf, 100, 0);				// send the name of file to server
			recv(sock, &size, sizeof(int), 0);

			//receive the file size value

		//	printf("File name: %s\n",buf);
//		  printf("==%d==",size);
			if(!size){
				printf("No such file on the remote directory\n\n");
			return 1;
			}
			f = malloc(size);
			recv(sock, f, size, 0);

			strcpy(buf,filename);
			strcat(buf,".mp3");

			while(1){
				filehandle = open(buf, O_CREAT | O_EXCL | O_WRONLY, 0666);
				if(filehandle == -1){
					sprintf(filename + strlen(filename), "%d", i);//needed only if same directory is used for both server &client
					strcpy(buf,filename);
					strcat(buf,".mp3");
				//	printf("%s %s\n",buf, filename);
				}
				else break;
			}
			write(filehandle, f, size, 0);
			close(filehandle);
			strcpy(buf, "cat ");
			strcat(buf, filename);
			strcat(buf, ".mp3");
			printf("%s\n",buf);
			printf("Connecting to the server...\n");
			sleep(1);
			printf("Streaming Audio Now!\n");
			sleep(1);
			char t1[100];
        		strcpy(t1,"vlc ");
        		strcat(t1,filename);
			strcat(t1,".mp3");
        		strcat(t1," -f --fullscreen");
			system(t1);

			break;

		case 3: printf("Exiting\n");
			return 1;

		default: printf("Invalid choice, enter again\n");

		}
	}

}
