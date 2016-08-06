#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 

#define MAX_LINE_LEN 1024

void sig_handler(int signo)
{
	if (signo == SIGINT)
    	printf("\n");
}

char *sh_read()
{
	char *line;
	int i,j;
	int quoteflag=0;
	int flag=0;
	char tmp[MAX_LINE_LEN];
	line=(char *)malloc(MAX_LINE_LEN*sizeof(char));
	memset(line,'\0',MAX_LINE_LEN);
	gets(line);
	for(i=0;i<strlen(line);i++)
	{
		if(line[i] == '"')
		{
			quoteflag++;
			continue;
		}
	}
	if(quoteflag % 2 == 1)
	{
		line[strlen(line)]='\n';
		printf("> ");
		do{
			gets(line+strlen(line));
			quoteflag=0;
			for(i=0;i<strlen(line);i++)
			{
				if(line[i] == '"')
				{
					quoteflag++;
					continue;
				}
			}
			if(quoteflag % 2 == 1)
			{
				line[strlen(line)]='\n';
				printf("> ");
			}
		}while(quoteflag % 2 == 1);
	}
	for(i=strlen(line)-1;i>=0;i--)
	{
		if(line[i] == ' ' || line[i] == '\n' || line[i] == '\r')
		{
			continue;
		}
		if(line[i] == '|')
		{
			line[strlen(line)] = ' ';
			flag=1;
			do{
				printf("> ");
				memset(tmp,'\0',MAX_LINE_LEN);
				gets(tmp);
				for (j=0;j<strlen(tmp);j++)
				{
					if(tmp[j] != ' ' && tmp[j] != '\n' && tmp[j] != '\r')
					{
						flag=0;
						break;
					}
				}
				if(flag == 0)
            		strncpy(line+strlen(line),tmp,strlen(tmp));
			}while(flag==1);
		}
		else {
			break;
		}
	}
	return line;
}

char ***sh_parse(char *line,int op[])
{
	int i;
	int j;
	char ***args=(char ***) malloc(MAX_LINE_LEN*sizeof(char **));
	int num=0,len=0;
	int comnum=0;
	int flag=0;
	int quoteflag=0;
	args[0]=(char **) malloc(MAX_LINE_LEN*sizeof(char *));
	args[0][num]=(char *) malloc(MAX_LINE_LEN*sizeof(char));
	memset(args[0][num],'\0',MAX_LINE_LEN);
	for(i=0;i<strlen(line);i++)
	{
		if(line[i] == '"')
		{
			quoteflag++;
			continue;
		}
		if(quoteflag % 2 == 0 && ( line[i] == '|' || line[i] == '<' || line[i] == '>'))
		{
			args[comnum][num] = NULL;
			comnum++;
			if(line[i] == '|')	op[comnum]=1;
			if(line[i] == '<')	op[comnum]=2;
			if(line[i] == '>' && line[i+1] != '>')	op[comnum]=3;
			if(line[i] == '>' && line[i+1] == '>')
			{
				op[comnum]=4;
				i++;
			}
			op[0]++;
			num=0;
			len=0;
			flag=0;
			args[comnum]=(char **) malloc(MAX_LINE_LEN*sizeof(char *));
			args[comnum][num]=(char *) malloc(MAX_LINE_LEN*sizeof(char));
			memset(args[comnum][num],'\0',MAX_LINE_LEN);
			continue;
		}
		if(quoteflag % 2 == 0 && (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') )
		{
			if(flag == 0) continue;
			num++;
			len=0;
			flag=0;
			args[comnum][num]=(char *)malloc(MAX_LINE_LEN*sizeof(char));
			memset(args[comnum][num],'\0',MAX_LINE_LEN);
			continue;
		}
		args[comnum][num][len++]=line[i];
		flag=1;
	}
	if(strlen(line) == 0) args[0][0] = NULL;
	else if(line[strlen(line)-1] == ' ') args[comnum][num] = NULL;
	return args;
}

int sh_exec(char ***args, int op[])
{
	pid_t pid[MAX_LINE_LEN];
	int pipe_fd[2];
	int filein,fileout;
  	int status;
  	int j=0;
  	int i=0;
  	char *path;

	if(args[0][0] == NULL) return 1;
	if(strcmp(args[0][0],"exit") == 0)
	{
		return 0;
	}
	if( strcmp(args[0][0],"cd") == 0 )
  	{
  		if (args[0][1][0] == '~')
  		{
  			path =(char *) malloc(MAX_LINE_LEN*sizeof(char));
  			memset(path,'\0',MAX_LINE_LEN);
  			strcpy(path,getenv("HOME"));
            strncpy(path+strlen(getenv("HOME")),args[0][1]+1,strlen(args[0][1]));
            printf("%s\n",path);
  		}
  		else
  		{
  			path = malloc(sizeof(char) * strlen(args[0][1]));
  			memset(path,'\0',strlen(args[0][1]));
  			strcpy(path,args[0][1]);
  		}
   		if (chdir(path) != 0)
  		{
      		perror("lsh");
    	}
      	free(path);
    	return 1;
  	}
  	if(op[0] >= 1) pipe(pipe_fd);
  

  		i=0;
  		pid[i]=fork();
  		if(pid[i] != 0)
  		{
  			if(op[i+1] == 1)
  			{
  				pid[i+1]=fork();
  				if(pid[i+1] == 0)
                {
                    close(pipe_fd[1]);
                    close(0); 
                    dup2(pipe_fd[0], 0);
                    close(pipe_fd[0]);
                    if( execvp(args[i+1][0],args[i+1]) == -1 )
					{	
						perror("ve482sh $");
					}
                }
                else
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    waitpid(pid[i+1],&status,0);
                }
  			}
  			waitpid(pid[i],&status,0);
  		}
  		else
  		{
  			if(op[i+1] == 1)
  			{
  				close(pipe_fd[0]);
                close(1);
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
  			}
  			if(op[i+1] == 2)
  			{
  				filein = open(args[i+1][0], O_RDONLY, S_IRUSR | S_IWUSR | S_IXUSR);
                close(0); 
                dup2(filein, 0);
                close(filein); 
  			}
  			if(op[i+1] == 3)
  			{
  				fileout = open(args[i+1][0], O_RDWR | O_CREAT | O_TRUNC, 00700);
                close(1); 
                dup2(fileout, 1);
                close(fileout); 
  			}
  			if(op[i+1] == 4)
  			{
  				fileout = open(args[i+1][0], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
                close(1); 
                dup2(fileout, 1);
                close(fileout); 
  			}
   			if( execvp(args[i][0],args[i]) == -1 )
			{	
				perror("ve482sh $");
			}
  		}

	return 1;
}

void sh_loop()
{
	char *line;
	char ***args;
	int op[MAX_LINE_LEN];
	int status,ch_status;
	int i=0,j;
	do{
		for (i=0;i<MAX_LINE_LEN;i++) op[i]=0;
		printf("ve482sh $> ");
		signal(SIGINT, sig_handler);
		line = sh_read();
		args = sh_parse(line,op);
  		status = sh_exec(args,op);
		free(line);
		free(args);
	}while(status);
}


int main(void)
{
	sh_loop();
	return 0;
}