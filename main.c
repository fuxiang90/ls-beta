#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <unistd.h>
#include "ls.h"
const int filesize = 30;
int flagno = 0;
int flaglist = 0;
int flagall = 0;
int main(int argc,   char *argv[])
{

	int opt;
    char currentfilename[filesize];
    getcwd(currentfilename,filesize);
    if(currentfilename == NULL)
    {
        printf("error\n");
    }
    puts(currentfilename);

	opt = 0;

	//while(1)
	{
		opt = getopt(argc,argv,"la");
		switch(opt)
		{
			case 'l':flaglist = 1; break;
			case 'a':flagall = 1;  break;
			default :
                flagno = 1; break;
		}
        //flaglist = 1;
		PrintFile(currentfilename);
		//break;
	}

	return 0;
}
