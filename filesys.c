#include <stdint.h>
#include <mem.h>
#include <malloc.h>
#include <process.h>
#include "disk.h"
#include "filesys.h"


int write_block(unsigned int block_num, uint32_t *p) {
    unsigned int mem_block_num = block_num * 2;
    disk_write_block(mem_block_num, (char *) p);
    disk_write_block(mem_block_num + 1, (char *) (p + 16));
}

int read_block(unsigned int block_num, uint32_t *p) {
    unsigned int mem_block_num = block_num * 2;
    disk_read_block(mem_block_num, (char *) p);
    disk_read_block(mem_block_num + 1, (char *) (p + 16));
}

void init_superBlock(sp_block *superBlock) {
    superBlock->magic_num = Ext2;
    superBlock->free_block_count = 4063;//4096-1-32
    superBlock->free_inode_count = 1024;
    superBlock->dir_inode_count = 0;
    memset(superBlock->block_map, 0, sizeof(uint32_t) * 128);
    memset(superBlock->inode_map, 0, sizeof(uint32_t) * 32);
    write_block(0, (uint32_t *) superBlock);
}

void init_inode(inode *inodeList) {
    int i, j;
    for (i = 0; i < 1024; i++) {
        inodeList[i].size = 0;
        inodeList[i].file_type = 0;
        inodeList[i].link = 0;
        for (j = 0; j < 6; j++) {
            inodeList[i].block_point[j] = 0;
        }
    }
}

void init_sys() {
    if (open_disk()) {
        char *sysbase = malloc(33 * MEM_BLOCK_SIZE);//sizeof(sp_block) + 1024 * sizeof(inode)
        init_superBlock((sp_block *) sysbase);
        init_inode((inode *) (sysbase + MEM_BLOCK_SIZE));//+ sizeof(sp_block)
        int i;
        uint32_t *p = (uint32_t *) sysbase;
        for (i = 0; i < 33; i++) {
            write_block(i, p);
            p += 32;
        }
        free(sysbase);
    }
}

void shutdown_sys() {
    // TODO : to exit
    close_disk();
    exit(0);
}