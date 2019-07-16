# Simple-File-System

### Introduction
- This is an implementation of a (pseudo-) simple file system `sfs`

### Repository structure
-  `src/` contains the source code
    - `src/sfs_demo.c` contains a menu-driven application to test `sfs` 
- `build.sh` bash shell-script which can be used to build `sfs` from source

### Build
To build `sfs`,
- `cd` into the root of this repository and run command  `./build.sh`

### Example run
```bash
Hasans-MacBook-Pro:Simple-File-System hasankamal$ ./build.sh 

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
1
enter filesystem_file name: test_fs
filesystem id: 0

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
3
enter filesystem id: 0
enter file name: hello.txt
enter text to write(without spaces): HeyThere!
write successful

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
2
enter filesystem id: 0
enter file name: hello.txt
data found in given file:
HeyThere!

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
3
enter filesystem id: 0
enter file name: world.txt
enter text to write(without spaces): WorldIsABigPlace
write successful

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
2
enter filesystem id: 0
enter file name: world.txt
data found in given file:
WorldIsABigPlace

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
2
enter filesystem id: 0
enter file name: hello.txt
data found in given file:
HeyThere!

select command:
1. create_sfs
2. read_file
3. write_file
4. print_inode_bitmap
5. print_data_bitmap
6. print_superblock
7. print_file_list
8. print_inode_list
9. exit
9
removing files
exiting
```
