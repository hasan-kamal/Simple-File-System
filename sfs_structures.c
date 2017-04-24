/*

@author Luv Sharma (2015051), Hasan Kamal(2015039)
This source contains the structure definitions used by this filesystem

*/

#define MAX_NUM_INODES 20
#define MAX_NUM_DATA_BLOCKS 20

//superblock definition
typedef struct{
	int num_bytes;
	int imap_block_index;
	int d_map_block_index;
	int inodes_start_block;
	int data_start_block;
} superblock;

typedef struct{
	int num_inodes;
	int is_allocated[MAX_NUM_INODES];
} imap;

typedef struct{
	int num_data_blocks;
	int is_allocated[MAX_NUM_DATA_BLOCKS];
} dmap;

typedef struct{
	int id;
	int size;
	int data_block_index; //pointer to block containing corresponding user-data
} inode;

void display_superblock(superblock *s){
	printf("num_bytes:%d\n", s->num_bytes);
	printf("imap_block_index:%d\n", s->imap_block_index);
	printf("d_map_block_index:%d\n", s->d_map_block_index);
	printf("inodes_start_block:%d\n", s->inodes_start_block);
	printf("data_start_block:%d\n", s->data_start_block);
}

void display_imap(imap *im){
	printf("num_inodes:%d\n", im->num_inodes);
	for(int i=0; i<im->num_inodes; i++){
		//if(im->is_allocated[i])
			printf("index_inode:%d is_allocated:%d\n", i, im->is_allocated[i]);
	}
}

void display_dmap(dmap *dm){
	printf("num_data_blocks:%d\n", dm->num_data_blocks);
	for(int i=0; i<dm->num_data_blocks; i++){
		//if(dm->is_allocated[i])
			printf("index_dblock:%d is_allocated:%d\n", i, dm->is_allocated[i]);
	}
}

void display_inode(inode *n){
	printf("inode id: %d\n", n->id);
	printf("inode size: %d\n", n->size);
	printf("inode data_block_index: %d\n", n->data_block_index);
}