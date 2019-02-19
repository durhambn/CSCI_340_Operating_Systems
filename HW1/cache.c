
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2018
// cache.c file
// 
// Homework 1
//
// -----------------------------------

#include "cache.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int initializeCache( unsigned int number_of_lines ) {

	int line, retVal;

	retVal = OK;

	cache = malloc( sizeof( cache_line ) * number_of_lines );

	if ( cache != NULL ) {

		for ( line=0; line<number_of_lines; line++ ) {

			cache[line] = (cache_line*) malloc( sizeof( cache_line ) );
			cache[line]->tag = UNK;
			cache[line]->hit_count = ZERO;

		}

	} else
		retVal = FAIL;

	return retVal;

} // end initializeCache function


int cread( unsigned int cmf, unsigned int* hex_addr, unsigned int* found, unsigned int* replace ) {

	/* TODO: You complete


	*/
	// cmf - cashe mapping function
	// hex_addr - value at int address; memory address pointer
	// found - variable pointer (1=cache hit, 0=cache miss)
	// replace - variable pointer (1=replacement, 0=no replacement)
	//cprint();
	
	(*found) = MISS;
	(*replace) = NO;
	int soffset;
	int sline;
	int stag;
	int byte;

	if((*hex_addr)>=pow(2,8)){
		byte =FAIL;
		return byte;
	}		
			if (cmf == DM){ //don't worry about hit count
		// 1 is direct mapping
		//see if there is a value in cache line entered, find line number from hex entered
			soffset = (*hex_addr & 0x03);
			sline = ((*hex_addr & 0x1C)>>2);
			stag = ((*hex_addr & 0xE0)>>5);
			if( cache[sline]->tag == -1){
			//means line is empty
				(*found) = MISS;
				(*replace) = NO;
				//insert block, update tag bits, hit_count to 1
				cache[sline]->tag = stag;
				cache[sline]->hit_count =1;
			}

			else if(cache[sline]->tag !=1 && cache[sline]->tag == stag ){
			//cache line tag matches search
				//Check if word is the same??
				(*found) = HIT;
				(*replace) = NO;
				cache[sline]->hit_count +=1;
				//return byte;
			 
			}
			else if(cache[sline]->tag !=1 && cache[sline]->tag != stag){
			//line is full but not searched tag
				cache[sline]->tag = stag;
				(*found) = MISS;
				(*replace) = YES;
				cache[sline]->hit_count = 1;

			}
			byte = phy_memory[block_location[(stag<<3)+sline] + soffset];
			return byte;
		}
		else if (cmf == FA){
		// 2 is fully associative mapping
		soffset = (*hex_addr & 0x03);
		stag = ((*hex_addr & 0xFc)>>2);
		int lfu=0;
		int i;
		byte = phy_memory[block_location[stag] + soffset];
			//loop to see if tag is already in cache
			for(i = 0; i<NUM_OF_LINES; i++){
				if(cache[i]->tag == stag){
					//found in cache already
					(*found) = HIT;
					(*replace) = NO;
					cache[i]->hit_count +=1;
					return byte;
				}

			}
			if((*found) == MISS){
			//search not found; find empty line
				for(i = 0; i<NUM_OF_LINES; i++){
					//if empty line then enter line in cache
					if(cache[i]->tag == -1){
						(*replace) = NO;
						cache[i]->tag = stag;
						cache[i]->hit_count =1;
						return byte;
					}
					if((cache[i]->hit_count) < (cache[lfu]->hit_count)){
						lfu = i;
					}

				}
				if(i == NUM_OF_LINES){
					
					cache[lfu]->tag = stag;
					(*replace)=YES;
					cache[lfu]->hit_count = 1;
				}			
			}
		}
		
		
		//int retVal = phy_memory[block_location + offset]; // this is simply put here so the code complies, you must code correctly.
		int retVal = byte;
		return retVal;
	
} // end cread function



void cprint() {

	unsigned int line;

	printf("\n---------------------------------------------\n");
	printf("line\ttag\tnum of hits\n");
	printf("---------------------------------------------\n");

	for ( line=0; line<NUM_OF_LINES; line++ ) { 

		if ( cache[line]->tag == UNK ) {
			
			printf("%d\t%d\t%d\n", line, cache[line]->tag, cache[line]->hit_count );

		} else {

			printf("%d\t%02X\t%d\n", line, cache[line]->tag, cache[line]->hit_count );

		}

	}

} // end cprint function
