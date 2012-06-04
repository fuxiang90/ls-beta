#include <stdio.h>
#include <time.h>
int main(int argc, char *argv[]){
    time_t t = time(NULL);
	    struct tm *gmt, *area;
		    tzset();
			    area = localtime(&t);
				    printf ("Local time: %s", asctime(area));
					    gmt = gmtime(&t);
						    printf("GMT time: %s", asctime(gmt));
							    return 0;
								}
