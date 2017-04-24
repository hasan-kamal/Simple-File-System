/*

@author Luv Sharma (2015051), Hasan Kamal(2015039)
This source is the low-level implementation of our Simple File System

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sfs_structures.c"
#define BLOCK_SIZE 4*1024 //4KB
#define MAX_NUM_FILESYSTEMS 10

int counter = 0;
char *filesystems[MAX_NUM_FILESYSTEMS];

int min(int x, int y){
	return x<y?x:y;
}

int create_sfs(char *filename, int n_bytes){
	filesystems[counter++] = strdup(filename);

	FILE* f = fopen(filename, "w");

	//write superblock
	fseek(f, 0, SEEK_SET);
	superblock s = {.num_bytes=n_bytes, .imap_block_index=1, .d_map_block_index=2, .inodes_start_block=3, .data_start_block=4};
	fwrite(&s, sizeof(s), 1, f);
	
	//write imap
	fseek(f, s.imap_block_index*BLOCK_SIZE, SEEK_SET);
	int num_inodes = min((int) (BLOCK_SIZE/sizeof(inode)), MAX_NUM_INODES);
	imap im = {.num_inodes=num_inodes, .is_allocated={0}};
	fwrite(&im, sizeof(im), 1, f);

	//write dmap
	fseek(f, s.d_map_block_index*BLOCK_SIZE, SEEK_SET);
	int num_data_blocks = min((int) (n_bytes/BLOCK_SIZE - 4), MAX_NUM_DATA_BLOCKS);
	dmap dm = {.num_data_blocks=num_data_blocks, .is_allocated={0}};
	fwrite(&dm, sizeof(dm), 1, f);

	fclose(f);

	return counter-1;
}

int read_data(int disk, int block_num, void *block){
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return;
	}

	FILE* f = fopen(filesystems[disk], "r");

	//read superblock
	fseek(f, 0, SEEK_SET);
	superblock s;
	fread(&s, sizeof(s), 1, f);
	
	//read specific block
	fseek(f, block_num*BLOCK_SIZE, SEEK_SET);
	fread(block, BLOCK_SIZE, 1, f);
	
	fclose(f);
	return BLOCK_SIZE;
}

int write_data(int disk, int block_num, void *block){
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return;
	}

	FILE* f = fopen(filesystems[disk], "w");

	//read superblock
	fseek(f, 0, SEEK_SET);
	superblock s;
	fread(&s, sizeof(s), 1, f);
	
	//write specific block
	fseek(f, block_num*BLOCK_SIZE, SEEK_SET);
	fwrite(block, BLOCK_SIZE, 1, f);
	
	fclose(f);
	return BLOCK_SIZE;
}

// int read_file(int disk, char *file_name, void *block){

// }

// int write_file(int disk, char *file_name, void *block){

// }

void print_file_list(int file_system_id){
	
}

void print_inode_bitmaps(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	FILE* f = fopen(filesystems[file_system_id], "r");

	//read superblock
	fseek(f, 0, SEEK_SET);
	superblock s;
	fread(&s, sizeof(s), 1, f);
	
	//read imap
	fseek(f, s.imap_block_index*BLOCK_SIZE, SEEK_SET);
	imap im;
	fread(&im, sizeof(im), 1, f);
	display_imap(&im);

	fclose(f);
}

void print_data_bitmaps(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	FILE* f = fopen(filesystems[file_system_id], "r");

	//read superblock
	fseek(f, 0, SEEK_SET);
	superblock s;
	fread(&s, sizeof(s), 1, f);
	
	//read dmap
	fseek(f, s.d_map_block_index*BLOCK_SIZE, SEEK_SET);
	dmap dm;
	fread(&dm, sizeof(dm), 1, f);
	display_dmap(&dm);
	
	fclose(f);
}

void print_superblock(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	FILE* f = fopen(filesystems[file_system_id], "r");

	//read superblock
	fseek(f, 0, SEEK_SET);
	superblock s;
	fread(&s, sizeof(s), 1, f);
	display_superblock(&s);

	fclose(f);
}