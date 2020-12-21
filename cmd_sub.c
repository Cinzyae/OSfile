//
// Created by MACHENIKE on 12/21/2020.
//
/*
 *
typedef struct inodeS {
    uint32_t size;           // 文件大小
    uint16_t file_type;      // 文件类型
    uint16_t link;           // 连接数
    uint32_t block_point[INODE_POINT_NUM]; // 数据块指针
} inode;
 */
#include <stdint.h>
#include <malloc.h>
#include "cmd_sub.h"
#include "filesys.h"
#include "ulib.h"

int set_sp_block(uint16_t file_type, int *nblock, int *ninode) {
    int i, block_map, inode_map;
    sp_block *p = NULL;

    //读sp_block块
    p = malloc(MEM_BLOCK_SIZE);
    read_block(0, (uint32_t *) p);

    //sp_block块是否有空
    if (!(p->free_block_count) || !(p->free_inode_count))
        return -1;
    (p->free_block_count) -= 1;
    (p->free_inode_count) -= 1;
    if (file_type == TYPE_FOLDER)
        p->dir_inode_count += 1;

    //修改sp_block块
    // TODO now
//    *nblock = i * 32 + block_map;
//    *ninode = i * 32 + inode_map;

    //写sp_block块
    write_block(0, (uint32_t *) p);
    free(p);
    return 0;
}

int set_inode_block(uint16_t file_type, int *nblock, int *ninode) {
    inode *p = NULL;

    //读inode块
    p = malloc(MEM_BLOCK_SIZE);
    read_block(*ninode, (uint32_t *) p);

    //写inode块
    write_block(*ninode, (uint32_t *) p);
    free(p);
    return 0;
}
