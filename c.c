#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*for getting file size using stat()*/
#include<sys/stat.h>
/*for sendfile()*/
#include<sys/sendfile.h>
/*for O_RDONLY*/
#include<fcntl.h>
#include<unistd.h>


FILE ext_copy_to_file(char mp[])
{
	FILE *fp1,*fp2;
	DIR *directory;
	char a;
	struct dirent *ptr_readdir;
	if(directory = opendir("c:\\downloads\\")!= NULL)
	{
		while(ptr_readdir = readdir(directory) ! = NULL)
		{
			char* extension;
			char* p;
			ext = strchr(ptr_readdir->d_name,'.');
			if(strcmp(ext+1,mp)==0)
			{
				fp1=fopen(ptr_readdir->d_name,"r");
				fp2=fopen("temp.txt","w");
				do
				{
					a=fgetc(fp1);
					fputc(a,fp2);
				}while(a!=EOF);
				fcloseall();
				return fp2;
			}
		}
		closedir(directory);
	}
}
int main(int argc,char *argv[])
{
	struct sockaddr_in server,client;
	struct stat obj;
	int sock1, sock2;
	char buf[100],command[5],filename[20],trial[20];
	int k, i, size, len, c;
	int filehandle;
	FILE *fp3;
	sock1 = socket(AF_INET, SOCK_STREAM, 0);

	if(argc!=2){
		printf("Format of input: ./server portNumber\n");
		return 1;
	}
	if(sock1 == -1){
		printf("Socket creation failed");
		exit(1);
	}
	server.sin_family=AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	k = bind(sock1,(struct sockaddr*)&server,sizeof(server));
	if(k == -1){
		printf("Binding error");
		exit(1);
	}
	k = listen(sock1,1);
	if(k == -1){
		printf("Listen failed");
		exit(1);
	}
	i=1;
	while(1)
	{
			len = sizeof(client);
			if(sock2 = accept(sock1,(struct sockaddr*)&client, &len)<0){
				printf("Accepting failed\n");
			}
			else{
				memset(buf, 0, sizeof(buf));
				memset(command, 0, sizeof(buf));
				recv(sock2, buf, 100, 0);
				sscanf(buf, "%s", command);
				if(strstr(command, "find"))
				{
					//system("find . -name '*.mp3' > temps.txt");
					char mp[]=".mp3";
					fp3=ext_copy_to_file(mp);
					i = 0;
					stat(fp3,&obj);
					size = obj.st_size;
					send(sock2, &size, sizeof(int),0);
					filehandle = open("temps.txt", O_RDONLY);
					sendfile(sock2,filehandle,NULL,size);
				}
				else if(strstr(command,"get"))
				{
					FILE *fp=fopen("text.txt","w+");
					char text[20];char *array;
					strcpy(text,"find . -name '*");
					sscanf(buf, "%s%s", filename, filename);
					//printf("%s\n",filename);
					strcat(text,filename);
					strcat(text,"' > text.txt");
					system(text);
					fseek(fp, 0, SEEK_END);
					int size = ftell(fp);
					fseek(fp, 0, SEEK_SET);
					array = (char*) malloc(size * sizeof(char));
					fread(array, sizeof(int), size, fp);
					array=array+2;
					sscanf(array, "%s", filename);
					//printf("%s\n",filename);
					fclose(fp);
					stat(filename,&obj);
					filehandle = open(filename, O_RDONLY);
					//printf("%d\n",filehandle);
					size = obj.st_size;
					printf("--%d--\n",size);
					if(filehandle == -1)
						  size = 0;
					send(sock2, &size, sizeof(int), 0);
					if(size)
						sendfile(sock2, filehandle, NULL, size);
				}
				close(sock2);
			}
	}
	close(sock1);
	return 0;
}