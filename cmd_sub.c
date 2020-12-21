//
// Created by MACHENIKE on 12/21/2020.
//
#include <stdint.h>
#include <malloc.h>
#include <mem.h>
#include "cmd_sub.h"

int check_home(int ninode, uint16_t file_type, char *content) {
    //读取home_inode
    int home_inode = 0;
    ino_list *p = NULL;
    p = malloc(sizeof(ino_list));
    read_inode(home_inode, (uint32_t *) p);
    //判断是否有空
    //inode_num_list:inode在块中的序号
    int inode_num_list = home_inode % 32;
    int free_dir = p->inodes[inode_num_list].size;
    if (free_dir >= 48) {
        return -1;
    }
    //修改dir
    int group = free_dir / 8;
    int rest = free_dir % 8;
    int data_block = p->inodes[inode_num_list].block_point[group];
    //读取 data块，根据相应dir块写传入的ninode
    //修改
    //修改新的ninode，写文件（在外面？）
    //善后
    // TODO now
//
//
//    dir_item *q = (dir_item *) (p->inodes[inode_num_list].block_point);
//    q = q + p->inodes[inode_num_list].size;
//    q->inode_id = ninode;
//    q->valid = 1;
//    q->type = file_type;
//    strcpy(q->name, content);
//    //修改inode
//    p->inodes[inode_num_list].size++;
//    p->inodes[inode_num_list].link++;

    write_inode(ninode, (uint32_t *) p);
    free(p);
    return 0;
}

int set_sp_block(uint16_t file_type, int *ninode) {
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
    *ninode = i * 32 + inode_map;

    //写sp_block块
    write_block(0, (uint32_t *) p);
    free(p);
    return 0;
}

int set_inode_block(uint16_t file_type, int *nblock, int *ninode) {
//    inode *p = NULL;
//
//    //读inode块
//    p = malloc();
//    read_block(*ninode, (uint32_t *) p);
//
//    //写inode块
//    write_block(*ninode, (uint32_t *) p);
//    free(p);
    return 0;
}
