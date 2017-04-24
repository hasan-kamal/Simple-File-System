/*

@author Luv Sharma (2015051), Hasan Kamal(2015039)
This source demonstrates the high-level usage of our Simple File System's implementation

*/

#include <stdio.h>
#include "sfs_implementation.c"
#define FILE_NUM_BYTES 1024*1024 //1024KB
#define STR_LEN 1000

int main(){

	int choice;
	do{
		printf("\nselect command:\n");
		printf("1. create_sfs\n");
		printf("2. read_file\n");
		printf("3. write_file\n");
		printf("4. print_inode_bitmap\n");
		printf("5. print_data_bitmap\n");
		printf("6. print_superblock\n");
		printf("7. print_file_list\n");
		printf("8. exit\n");
		scanf("%d", &choice);
		switch(choice){
			case 1:{
				printf("enter file name: ");
				char fname[STR_LEN];
				scanf("%s", fname);
				int id = create_sfs(fname, FILE_NUM_BYTES);
				printf("filesystem id: %d\n", id);
				break;
			}
			case 2:{

				break;
			}
			case 3:{

				break;
			}
			case 4:{
				int id;
				printf("enter file id: ");
				scanf("%d", &id);
				print_inode_bitmaps(id);
				break;
			}
			case 5:{
				int id;
				printf("enter file id: ");
				scanf("%d", &id);
				print_data_bitmaps(id);
				break;
			}
			case 6:{
				int id;
				printf("enter file id: ");
				scanf("%d", &id);
				print_superblock(id);
				break;
			}
			case 7:{
				int id;
				printf("enter file id: ");
				scanf("%d", &id);
				print_file_list(id);
				break;
			}
			default:{
				if(choice!=8)
					printf("wrong input, try again\n");
				break;
			}
		}
	}while(choice!=8);

	return 0;
}