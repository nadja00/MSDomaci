#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
	FILE *fp1, *fp2, *fp3;
	char *str1, *str2;
	char sw0, sw1;
	int i, j;
	int diode = 0x01; 
	char ts1, ts2, ts3, ts4;
	size_t num_of_bytes = 6;
	int novo, staro = 0;
	int x = 0;
	
	while(1){
		
    fp1 = fopen ("/dev/switch", "r");
		if(fp1==NULL) {
			puts("Problem pri otvaranju fajla switch");
			return -1;
		}
		
		str1 = (char *)malloc(num_of_bytes+1);
		getline(&str1, &num_of_bytes, fp1);
		
		if(fclose(fp1)){
			puts("Problem pri zatvaranju fajla switch");
			return -1;
		}
		
		sw1 = str1[4] - 48;
		sw0 = str1[5] - 48;
		free (str1);
		
		x = sw1*2 + sw0; 
		
		fp2 = fopen ("/dev/button", "r");
		if(fp2==NULL)
		{
			puts("Problem pri otvaranju fajla button");
			return -1;
		}
		
		str2 = (char *)malloc(num_of_bytes+1);
		getline(&str2, &num_of_bytes, fp2);
		
		if(fclose(fp2))
		{
			puts("Problem pri zatvaranju fajla button");
			return -1;
		}	
		ts4 = str2[2]-48;
		ts3 = str2[3]-48;
		ts2 = str2[4]-48;
		ts1 = str2[5]-48;
		free(str2);
		usleep(100);
		
		
		if(ts1 && !ts2 && !ts3 && !ts4)
			novo = 1;
		else if(ts2 && !ts1 && !ts3 && !ts4)
			novo = 2;
		else if(ts3 && !ts1 && !ts2 && !ts4)
			novo = 3;
		else if(ts4 && !ts1 && !ts2 && !ts3)
			novo = 4;
		
		if(novo != staro && novo != 0){
			switch(novo) 
			{
				case 1: 
					diode = 0x01;
					break; 
				case 4: 
					diode = 0x08; 
					break; 
				case 2: 
					for( i=0; i < x ; i++)
					{
						diode >>= 1; 
						if( diode == 0)
							diode = 0x08;
					}
					break; 
				case 3: 
					for( j =0; j < x ; j++)
					{
						diode <<= 1; 
						if( diode == 0x10)
							diode = 0x01;
					}
					break; 
				default: 
						diode = 0x01; 
						break; 
						
			}
			usleep(100);
      staro = novo;
		}
		
		fp3 = fopen("/dev/led", "w");
			if(fp3 == NULL){
				printf("Problem pri otvaranju /dev/led\n");
				return -1;
			}
			
			fprintf(fp3,"%d",diode);
			
			if(fclose(fp3)){
				printf("Problem pri zatvaranju /dev/led\n");
				return -1;
			}
			
	}

}
