//
// Created by MACHENIKE on 12/18/2020.
//
/*
 * device block size:512 B
 * device block number:8192
 * device block total:4 MB
 *
 * inode block size:256 bits = 32 B
 * inode block number:1024
 * inode block total:32 KB
 *
 * dir_item block size:128 B
 * data_item block size:1 KB
 * dir_item/data_item block total:4 MB - 33 KB
 *
 * Mem_block size:1 KB
 * Mem_block number:4096
 * Mem_block total:4 MB
*/

#ifndef OSFILE_FILESYS_H
#define OSFILE_FILESYS_H

#define Ext2 641326
#define MEM_BLOCK_SIZE 1024
#define BLOCK_MAP_NUM 128
#define INODE_MAP_NUM 32
#define INODE_POINT_NUM 6
#define TYPE_FOLDER 0
#define TYPE_FILE 1

#include <stdint.h>

typedef struct super_block {
    int32_t magic_num;        // 幻数， 用于识别文件系统
    int32_t free_block_count; // 空闲数据块数
    int32_t free_inode_count; // 空闲索引节点数
    int32_t dir_inode_count;  // 被占用的目录inode数
    uint32_t block_map[BLOCK_MAP_NUM];  // 数据块占用位图，记录数据块的使用情况
    uint32_t inode_map[INODE_MAP_NUM];   // inode占用位图，记录索引节点表的使用情况
} sp_block;
//total one Mem_block

typedef struct inodeS {
    uint32_t size;           // 文件大小
    uint16_t file_type;      // 文件类型
    uint16_t link;           // 连接数
    uint32_t block_point[INODE_POINT_NUM]; // 数据块指针
} inode;
//256 bits one inode;total 32 Mem_block

typedef struct dir_itemS {
    uint32_t inode_id; // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;      // 当前目录项是否有效
    uint8_t type;        // 当前目录项类型（文件/目录）
    char name[121];  // 目录项表示的文件/目录的文件名/目录名
} dir_item;
//1024 bits

int write_block(unsigned int block_num, uint32_t *p);

int read_block(unsigned int block_num, uint32_t *p);

void init_superBlock(sp_block *superBlock);

void init_inode(inode *inodeList);

void init_sys();

void shutdown_sys();

#endif //OSFILE_FILESYS_H
