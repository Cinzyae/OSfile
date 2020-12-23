#include <stdint.h>
#include <mem.h>
#include <malloc.h>
#include <process.h>
#include "disk.h"
#include "filesys.h"


int write_block(unsigned int block_num, char *p) {
    unsigned int mem_block_num = block_num * 2;
    disk_write_block(mem_block_num, p);
    disk_write_block(mem_block_num + 1, p + 512);
}

int read_block(unsigned int block_num, char *p) {
    unsigned int mem_block_num = block_num * 2;
    disk_read_block(mem_block_num, p);
    disk_read_block(mem_block_num + 1, p + 512);
}

int find_inode_block(int ninode) {
    return ninode / 32 + 1;//add one sp_block
}

//传入一个inode，返回它所在的数据块
int read_inode(int ninode, char *p) {
    int ninode_block = find_inode_block(ninode);
    read_block(ninode_block, p);
}

//根据inode所在的数据块写
int write_inode(int ninode, char *p) {
    int ninode_block = find_inode_block(ninode);
    write_block(ninode_block, p);
}

void init_superBlock(sp_block *superBlock) {
    memset(superBlock, 0, MEM_BLOCK_SIZE);
    superBlock->magic_num = Ext2;
    superBlock->free_block_count = 4063;//4096-1-32
    superBlock->free_inode_count = 1024;
    superBlock->dir_inode_count = 0;
    memset(superBlock->block_map, 0, sizeof(uint32_t) * 128);
    memset(superBlock->inode_map, 0, sizeof(uint32_t) * 32);
    write_block(0, superBlock);
}

void init_inode(inode *inodeList) {
    memset(inodeList, 0, 32 * MEM_BLOCK_SIZE);
//    int i, j;
//    for (i = 0; i < 1024; i++) {
//        inodeList[i].size = 0;
//        inodeList[i].file_type = 0;
//        inodeList[i].link = 0;
//        for (j = 0; j < 6; j++) {
//            inodeList[i].block_point[j] = 0;
//        }
//    }
}

int init_sys() {
    if (open_disk()) {
//        char *sysbase = malloc(33 * MEM_BLOCK_SIZE);//sizeof(sp_block) + 1024 * sizeof(inode)
//        init_superBlock((sp_block *) sysbase);
//        init_inode((inode *) (sysbase + MEM_BLOCK_SIZE));//+ sizeof(sp_block)
//        int i;
//        uint32_t *p = (uint32_t *) sysbase;
//        for (i = 0; i < 33; i++) {
//            write_block(i, p);
//            p += 32;
//        }
//        free(sysbase);

//        char *sysbase = malloc(MEM_BLOCK_SIZE);
//        init_superBlock((sp_block *) sysbase);
//        write_block(0, (uint32_t *) sysbase);
//        free(sysbase);
//        int i;
//        char *p = malloc(MEM_BLOCK_SIZE);
//        memset(p, 0, MEM_BLOCK_SIZE);
//        for (i = 1; i < 4096; i++) {
//            write_block(i, (uint32_t *) p);
//        }
//        free(p);
        char *sysbase = malloc(MEM_BLOCK_SIZE);
        init_superBlock((sp_block *) sysbase);
        free(sysbase);
        return 1;
    }
    return 0;
}

void shutdown_sys() {
    // TODO : to exit
    close_disk();
    exit(0);
}