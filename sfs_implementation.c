/*

@author Luv Sharma (2015051), Hasan Kamal(2015039)
This source is the low-level implementation of our Simple File System

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sfs_structures.c"
#define BLOCK_SIZE 4*1024 //4KB
#define FILE_NUM_BYTES 1024*1024 //1024KB
#define MAX_NUM_FILESYSTEMS 10

int counter = 0;
char *filesystems[MAX_NUM_FILESYSTEMS];

int min(int x, int y){
	return x<y?x:y;
}

int read_data(int disk, int block_num, void *block, int num_bytes){
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return -1;
	}

	FILE* f = fopen(filesystems[disk], "r");

	//read specific block
	fseek(f, block_num*BLOCK_SIZE, SEEK_SET);
	fread(block, num_bytes, 1, f);
	
	fclose(f);
	return BLOCK_SIZE;
}

int write_data(int disk, int block_num, void *block, int num_bytes){
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return -1;
	}

	//read all data
	FILE* f_all = fopen(filesystems[disk], "r");
	void *all_data;
	if(f_all!=NULL){
		 all_data = malloc(FILE_NUM_BYTES);
		fread(all_data, FILE_NUM_BYTES, 1, f_all);
		fclose(f_all);
		remove(filesystems[disk]);
	}

	//write into a new temp file
	FILE *f = fopen(filesystems[disk], "w");
	if(f_all!=NULL)
		fwrite(all_data, FILE_NUM_BYTES, 1, f);
	fseek(f, block_num*BLOCK_SIZE, SEEK_SET);
	fwrite(block, num_bytes, 1, f);
	
	fclose(f);
	return BLOCK_SIZE;
}

int create_sfs(char *filename, int n_bytes){
	filesystems[counter++] = strdup(filename);

	//FILE* f = fopen(filename, "w");

	// //write superblock
	// fseek(f, 0, SEEK_SET);
	// superblock s = {.num_bytes=n_bytes, .imap_block_index=1, .d_map_block_index=2, .inodes_start_block=3, .data_start_block=4};
	// fwrite(&s, sizeof(s), 1, f);
	
	// //write imap
	// fseek(f, s.imap_block_index*BLOCK_SIZE, SEEK_SET);
	// int num_inodes = min((int) (BLOCK_SIZE/sizeof(inode)), MAX_NUM_INODES);
	// imap im = {.num_inodes=num_inodes, .is_allocated={0}};
	// fwrite(&im, sizeof(im), 1, f);

	// //write dmap
	// fseek(f, s.d_map_block_index*BLOCK_SIZE, SEEK_SET);
	// int num_data_blocks = min((int) (n_bytes/BLOCK_SIZE - 4), MAX_NUM_DATA_BLOCKS);
	// dmap dm = {.num_data_blocks=num_data_blocks, .is_allocated={0}};
	// fwrite(&dm, sizeof(dm), 1, f);

	// fclose(f);

	//write superblock
	superblock s = {.num_bytes=n_bytes, .imap_block_index=1, .d_map_block_index=2, .inodes_start_block=3, .data_start_block=4};
	write_data(counter-1, 0, &s, sizeof(s));

	//write imap
	int num_inodes = min((int) (BLOCK_SIZE/sizeof(inode)), MAX_NUM_INODES);
	imap im = {.num_inodes=num_inodes, .is_allocated={0}};
	write_data(counter-1, s.imap_block_index, &im, sizeof(im));
	
	//write dmap
	int num_data_blocks = min((int) (n_bytes/BLOCK_SIZE - 4), MAX_NUM_DATA_BLOCKS);
	dmap dm = {.num_data_blocks=num_data_blocks, .is_allocated={0}};
	write_data(counter-1, s.d_map_block_index, &dm, sizeof(dm));

	//write inodes block
	all_inodes all_in;
	all_in.num_inodes = num_inodes;
	for(int i=0; i<num_inodes; i++){
		all_in.inodes[i].id = i;
		all_in.inodes[i].size = -1;
		all_in.inodes[i].data_block_index = -1;
		all_in.inodes[i].file_name = strdup("null");
	}
	write_data(counter-1, s.inodes_start_block, &all_in, sizeof(all_in));

	return counter-1;
}

int read_file(int disk, char *file_name, void *block){
	//return <0 value if invalid input
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return -1;
	}

	//read superblock
	superblock s;
	read_data(disk, 0, &s, sizeof(s));
	
	//read imap
	imap im;
	read_data(disk, s.imap_block_index, &im, sizeof(im));
	
	//read all inodes
	all_inodes ai;
	read_data(disk, s.inodes_start_block, &ai, sizeof(ai));
	
	for(int i=0; i<ai.num_inodes; i++){
		if((im.is_allocated[i]==1)&&(strcmp(ai.inodes[i].file_name, file_name)==0)){
			//file found
			read_data(disk, s.data_start_block+ai.inodes[i].data_block_index, block, BLOCK_SIZE);
			return 1;
		}
	}

	printf("error: file not found\n");
	return -1;
}

int write_file(int disk, char *file_name, void *block_to_write, int num_bytes_to_write){
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return -1;
	}

	//read superblock
	superblock s;
	read_data(disk, 0, &s, sizeof(s));
	
	//read imap
	imap im;
	read_data(disk, s.imap_block_index, &im, sizeof(im));
	
	//read dmap
	dmap dm;
	read_data(disk, s.d_map_block_index, &dm, sizeof(dm));
	
	//read all inodes
	all_inodes ai;
	read_data(disk, s.inodes_start_block, &ai, sizeof(ai));
	
	for(int i=0; i<ai.num_inodes; i++){
		if((im.is_allocated[i]==1)&&(strcmp(ai.inodes[i].file_name, file_name)==0)){
			//existing file, overwrite contents
			write_data(disk, s.data_start_block+ai.inodes[i].data_block_index, block_to_write, num_bytes_to_write);
			return 1;
		}
	}

	//did not match, so allocate a new inode instead
	for(int i=0; i<ai.num_inodes; i++){
		if(im.is_allocated[i]==0){
			im.is_allocated[i] = 1; //mark allocated

			//find an unallocated datablock
			int x;
			for(x=0; x<MAX_NUM_DATA_BLOCKS; x++){
				if(dm.is_allocated[x]==0){
					dm.is_allocated[x] = 1; //mark allocated
					break;
				}
			}

			ai.inodes[i].file_name = strdup(file_name);
			ai.inodes[i].size = 1;
			ai.inodes[i].data_block_index = x; //points to xth datablock

			//propagate all changes to disk
			//write_data(disk, 0, &s, sizeof(s));
			write_data(disk, s.imap_block_index, &im, sizeof(im));
			write_data(disk, s.d_map_block_index, &dm, sizeof(dm));
			write_data(disk, s.inodes_start_block, &ai, sizeof(ai));
			write_data(disk, s.data_start_block+x, block_to_write, num_bytes_to_write);

			return 1;
		}
	}

	printf("file cannot be added, all inodes allocated already\n");
	return -1;
}

void print_file_list(int disk){
	//return <0 value if invalid input
	if(disk<0 || disk>=counter){
		printf("invalid id\n");
		return;
	}

	//read superblock
	superblock s;
	read_data(disk, 0, &s, sizeof(s));
	
	//read imap
	imap im;
	read_data(disk, s.imap_block_index, &im, sizeof(im));
	
	//read all inodes
	all_inodes ai;
	read_data(disk, s.inodes_start_block, &ai, sizeof(ai));
	
	int num_files = 0;
	for(int i=0; i<ai.num_inodes; i++){
		if(im.is_allocated[i]==1){
			printf("%s\n", ai.inodes[i].file_name);
			num_files++;
		}
	}

	if(num_files==0)
		printf("no files in system currently\n");
}

void print_inode_list(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	//read superblock
	superblock s;
	read_data(file_system_id, 0, &s, sizeof(s));
	
	//read all inodes list
	all_inodes ai;
	read_data(file_system_id, s.inodes_start_block, &ai, sizeof(ai));
	display_all_inodes(&ai);
}

void print_inode_bitmaps(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	//read superblock
	superblock s;
	read_data(file_system_id, 0, &s, sizeof(s));
	
	//read imap
	imap im;
	read_data(file_system_id, s.imap_block_index, &im, sizeof(im));
	display_imap(&im);
}

void print_data_bitmaps(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	//read superblock
	superblock s;
	read_data(file_system_id, 0, &s, sizeof(s));
	
	//read dmap
	dmap dm;
	read_data(file_system_id, s.d_map_block_index, &dm, sizeof(dm));
	display_dmap(&dm);
}

void print_superblock(int file_system_id){
	if(file_system_id<0 || file_system_id>=counter){
		printf("invalid id\n");
		return;
	}

	//read superblock
	superblock s;
	read_data(file_system_id, 0, &s, sizeof(s));
	display_superblock(&s);
}

void remove_all_files(){
	for(int i=0; i<counter; i++)
		remove(filesystems[i]);
}