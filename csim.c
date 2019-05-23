#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include "cachelab.h"

typedef struct {
	int v;
	int tag;
	int lastuse;
}line;

typedef struct {
	line *linep;
}set;

typedef struct {
	set *setp;
}cache;

int snum; 
int lnum; 
int lastcheck;

int s=4,e=1,b=4;

cache c;
int hits,misses,evictions;

void setcache(){
	c.setp = malloc(snum*sizeof(set));
	for(int i=0;i<snum;i++)
		c.setp[i].linep = malloc(lnum*sizeof(line));
}

void pushcache(int address){
	int tagbit=0,setbit=0,blockbit=0;
	blockbit = ~((-1)<<b);
	setbit = (~((-1)<<(b+s)))^blockbit;
	tagbit = (-1)<<(b+s);


	int tagval,setval;//,blockval;
	tagval = (address & tagbit)>>(b+s);
	setval = (address & setbit)>>b;
//	blockval = address & blockbit;


	int oldindex=0;
	int olduse=-1;

	for(int i=0;i<snum;i++){
		if(setval == i){
			for(int j=0;j<e;j++){
				if(olduse > c.setp[i].linep[j].lastuse || olduse<0){
					olduse = c.setp[i].linep[j].lastuse;
					oldindex=j;
				}
				if(c.setp[i].linep[j].v==0){
					misses++;
					c.setp[i].linep[j].v=1;
					c.setp[i].linep[j].tag=tagval;
					c.setp[i].linep[j].lastuse=++lastcheck;
					break;
				}
				else if(c.setp[i].linep[j].tag == tagval){
					hits++;
					c.setp[i].linep[j].lastuse=++lastcheck;
					break;
				}
				else if(j==e-1){
					misses++;
					evictions++;
					c.setp[i].linep[oldindex].v=1;
					c.setp[i].linep[oldindex].tag=tagval;
					c.setp[i].linep[oldindex].lastuse=++lastcheck;
					break;

				}
			}
		}
	}

}
int main(int argc, char *argv[])
{
	char a;
	char cmd;
	int addr;

	FILE *file = 0;
	while((a=getopt(argc, argv, "s:E:b:t:")) != -1){
		switch (a) {
			case 's':
				s = atoi(optarg); 
				break;
			case 'E':
				e = atoi(optarg);
				break;
			case 'b':
				b = atoi(optarg);
				break;
			case 't': 
				if (!(file = fopen(optarg, "r")))
					return 1; 
				break;
			default:
				return 1;
		}
	}

	snum = 2<<s;
	lnum = e;

	setcache();

	while(fscanf(file, " %c %x%*c%*d", &cmd, &addr) != EOF){
		if (cmd == 'I')
			continue;

		pushcache(addr);
		if (cmd=='M')
			pushcache(addr);
	}

//	printf("%d %d %d",hits,misses,evictions);
    	printSummary(hits, misses, evictions);
    	
	fclose(file);
	for(size_t i = 0; i < snum; ++i)
	free(c.setp[i].linep);
	free(c.setp);
	return 0;


	return 0;
}





