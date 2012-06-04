#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>
#include <locale.h>
#include <dirent.h>
#include <time.h>
#include <utime.h> /*  使用utime 这个函数*/
#include <pwd.h>
#include <grp.h>
const static int FILENAME_MAX_LEN = 256+2;

extern int flagall;
extern int flagno;
extern int flaglist;
# define N 256
struct filenode
{
    char filename[N];
    char modtime[30];
    unsigned char type;
    unsigned short int reclen;
    int fsize;
    int fgid;
    int fuid;
    int fmode;
    char fstrmode[10 + 1];
};

int cmp(const void *p1, const void *p2)
{
    return strcmp( ((struct filenode* ) p1)->filename, ((struct filenode *) p2)->filename );
}
int getFIleTime(char *filename ,struct filenode * tnode)
{
    //stat(entry->d_name,&statbuf);
    //stat(entry->d_name,&statbuf);
}
/*其实可以用宏来做 ，呵呵 这里直接操作了 mode*/
int getCharMode(int mode,char * str)
{
    if(mode < 0 )
        return -1;

    str[9] = (mode>>0) & 1 ?'x':'-';
    str[8] = (mode>>1) & 1 ?'w':'-';
    str[7] = (mode>>2) & 1 ?'r':'-';
    str[6] = (mode>>3) & 1 ?'x':'-';
    str[5] = (mode>>4) & 1 ?'w':'-';
    str[4] = (mode>>5) & 1 ?'r':'-';
    str[3] = (mode>>6) & 1 ?'x':'-';
    str[2] = (mode>>7) & 1 ?'w':'-';
    str[1] = (mode>>8) & 1 ?'r':'-';


    if (S_ISDIR(mode))    str[0]= 'd';
    else if (S_ISCHR(mode))    str[0] = 'c';
    else if (S_ISBLK(mode))    str[0] = 'b';
    else if (S_ISFIFO(mode))    str[0] = 'f' ;
    else  str[0] = '-';

    str[10] = '\0';

    return 1;

}
int GetFileName(char * dirname ,struct filenode  filenametable[] )
{
    int filenum = 0;
    char *dir ;

    dir = (char *)malloc(sizeof(char)*(strlen(dirname) + 2));
    strcpy(dir,dirname);
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    struct tm *local;
    time_t t;
    if( (dp = opendir(dir))  == NULL )
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return -1;
    }

    chdir(dir);
    while( (entry = readdir(dp)) != NULL)
    {
//        if( strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 )
//            continue;
//        else
        {
            strcpy(filenametable[filenum].filename, entry->d_name);
            filenametable[filenum].type  = entry->d_type;
            filenametable[filenum].reclen  = entry->d_reclen;


            stat(entry->d_name,&statbuf);
            struct utimbuf utm ;
            t = statbuf.st_mtime;
            local = localtime(&t);
            strcpy(filenametable[filenum].modtime,asctime(local));
            //不知为何 modtime 最后多了个\n
            int mlen = strlen(filenametable[filenum].modtime);
            filenametable[filenum].modtime[mlen-1] = '\0';
            filenametable[filenum].fmode = statbuf.st_mode;
            filenametable[filenum].fgid = statbuf.st_gid;
            filenametable[filenum].fuid = statbuf.st_uid;
            filenametable[filenum].fsize = statbuf.st_size;
            filenum ++;

            //printf ("Local time: %s", asctime(local));
            //printf("%d-%d-%d %2d:%2d\n",local->tm_year,local->tm_mon,local->tm_mday,local->tm_hour,local->tm_min);
//            if(stat(entry->d_name,&statbuf) >= 0  )
//            {
//                printf("%d \t",statbuf.st_gid);
//            }

        }
    }

    closedir(dp);

    return filenum;
}

int PrintFile(char * dirname)
{
    struct filenode filenametable[30];
    int i = 0,j = 0;
    int filecount = GetFileName(dirname,filenametable);

    qsort(filenametable,filecount,sizeof(struct filenode),cmp);

    /*只打印文件名*/
    if(flagno == 1)
    {
        for(i = 0 ; i < filecount ; i ++)
            if(flagall == 1 || (filenametable[i].filename[0] != '.'))
                printf("%s\t",filenametable[i].filename);

        printf("\n");
    }
    else if (flagno == 0)
    {

        for(i = 0 ; i < filecount ; i ++)
                printf("%s\t",filenametable[i].filename);
        printf("\n");
    }
    else if(flaglist == 1)
    {

        for(i = 0 ; i < filecount ; i ++)
        {
            int x = filenametable[i].fmode;

            getCharMode(x,filenametable[i].fstrmode);
            //printf("%s\t",getpwuid(filenametable[i].fuid)->pw_name);
            if(flagall == 1 || (filenametable[i].filename[0] != '.'))
                printf("%s\t%s\t%s\t%-7d\t%s\t%s\n",filenametable[i].fstrmode,
                       getpwuid(filenametable[i].fuid)->pw_name,getgrgid(filenametable[i].fgid)->gr_name ,
                       filenametable[i].fsize,filenametable[i].modtime,
                       filenametable[i].filename);
        }

    }


    return 0;
}
