//
// Created by MACHENIKE on 12/21/2020.
//
#include <stdint.h>
#include <malloc.h>
#include <mem.h>
#include "cmd_sub.h"

int touch_sub(int ninode, int ndata, uint16_t file_type, char *content) {
//-----
    // 读取home_inode
    int home_inode = 0;
    ino_list *p = NULL;
    p = malloc(sizeof(ino_list));
    read_inode(home_inode, (uint32_t *) p);
    // inode_num_list:inode在块中的序号
    int inode_num_list = home_inode % 32;
    int free_dir = p->inodes[inode_num_list].size;
    if (free_dir >= 48) {
        return -1;
    }
    //修改inode
    p->inodes[inode_num_list].size += 1;
    p->inodes[inode_num_list].link += 1;
//-----
    //读取、修改dir
    int group = free_dir / 8;
    int rest = free_dir % 8;
    int data_block = p->inodes[inode_num_list].block_point[group];
    dir_list *q = NULL;
    q = malloc(sizeof(dir_list));
    read_block(data_block, (uint32_t *) q);
    q->dir_items[rest].inode_id = ninode;
    q->dir_items[rest].type = file_type;
    q->dir_items[rest].valid = 1;
    strcpy(q->dir_items[rest].name, content);
//-----
    //读取、修改new_inode
    ino_list *p2 = NULL;
    p2 = malloc(sizeof(ino_list));
    read_inode(ninode, (uint32_t *) p2);
    int p2_num_list = ninode % 32;
    p2->inodes[p2_num_list].size = 8;
    p2->inodes[p2_num_list].file_type = file_type;
    p2->inodes[p2_num_list].link = 1;
    p2->inodes[p2_num_list].block_point[0] = ndata;
//-----
    write_inode(home_inode, (uint32_t *) p);
    write_block(data_block, (uint32_t *) q);
    write_inode(ninode, (uint32_t *) p2);
    free(p);
    free(q);
    free(p2);
    return 0;
}

int set_sp_block(uint16_t file_type, int *ninode, int *ndata) {
    int i, block_map, inode_map;

    //读sp_block块
    sp_block *p = NULL;
    p = malloc(MEM_BLOCK_SIZE);
    read_block(0, (uint32_t *) p);

    //sp_block块是否有空
    if (!(p->free_block_count) || !(p->free_inode_count))
        return -1;
    //修改sp_block块
    *ninode = p->free_inode_count;
    *ndata = p->free_block_count;
    (p->free_block_count) -= 1;
    (p->free_inode_count) -= 1;
    if (file_type == TYPE_FOLDER)
        p->dir_inode_count += 1;

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
