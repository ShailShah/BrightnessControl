#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[1])
{	//Enter the command in the format \"set <number>\" where number is between 1 and max_brightness
	assert(argc>1);

	int max_brightness;
	char buf[100];
	char i=0;
	memset(buf,0,100);
	char * temp;
	system("cat /sys/class/backlight/acpi_video0/max_brightness > out");
	FILE *fin;
	fin=fopen("out","r");
	fscanf(fin,"%d",&max_brightness);
	fclose(fin);
//	printf("Enter the command in the format \"set <number>\" where number is between 1 and %d\n",max_brightness);
	printf("%s\n",argv[1]);
	temp=argv[1];
	temp+=4;
	int a=atoi(temp);
	if(a>max_brightness || (strstr(argv[1],"set")==NULL)){
		printf("\nERROR:- Sorry your number exceeded the max_brightness value or you entered incorrect command\n");
		return 0;	
	} 
	int fp=open("/dev/bright",O_RDWR);
	write(fp,argv[1],strlen(argv[1]));
	while(read(fp,&buf[i++],1));
	printf("Message from the driver=%s\n",buf);
	return 0;
}
