/*

@author Luv Sharma (2015051), Hasan Kamal(2015039)
This source demonstrates the high-level usage of our Simple File System's implementation

*/

#include <stdio.h>
#include <string.h>
#include "sfs_implementation.c"
#define STR_LEN 1000

void display_menu(){
	printf("\nselect command:\n");
	printf("1. create_sfs\n");
	printf("2. read_file\n");
	printf("3. write_file\n");
	printf("4. print_inode_bitmap\n");
	printf("5. print_data_bitmap\n");
	printf("6. print_superblock\n");
	printf("7. print_file_list\n");
	printf("8. print_inode_list\n");
	printf("9. exit\n");
}

int main(){

	int choice;
	display_menu();
	scanf("%d", &choice);
	while(choice!=9){
		
		switch(choice){
			case 1:{
				printf("enter filesystem_file name: ");
				char fname[STR_LEN];
				scanf("%s", fname);
				int id = create_sfs(fname, FILE_NUM_BYTES);
				printf("filesystem id: %d\n", id);
				break;
			}
			case 2:{

				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);

				char fname[STR_LEN];
				printf("enter file name: ");
				scanf("%s", fname);

				char buffer[STR_LEN];
				if(read_file(id, fname, buffer)>=0)
					printf("data found in given file:\n %s\n", buffer);
				else
					printf("invalid input, try again\n");

				break;
			}
			case 3:{

				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);

				char fname[STR_LEN];
				printf("enter file name: ");
				scanf("%s", fname);

				char buffer[STR_LEN];
				printf("enter text to write(without spaces): ");
				scanf("%s", buffer);

				if(write_file(id, fname, buffer, strlen(buffer))>=0)
					printf("write successful\n");
				else
					printf("invalid input, try again\n");

				break;
			}
			case 4:{
				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);
				print_inode_bitmaps(id);
				break;
			}
			case 5:{
				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);
				print_data_bitmaps(id);
				break;
			}
			case 6:{
				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);
				print_superblock(id);
				break;
			}
			case 7:{
				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);
				print_file_list(id);
				break;
			}
			case 8:{
				int id;
				printf("enter filesystem id: ");
				scanf("%d", &id);
				print_inode_list(id);
				break;
			}
			default:{
				printf("wrong input, try again\n");
				break;
			}
		}

		display_menu();
		scanf("%d", &choice);
	}

	printf("removing files\n");
	remove_all_files();
	printf("exiting\n");

	return 0;
}