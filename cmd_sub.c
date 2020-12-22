//
// Created by MACHENIKE on 12/21/2020.
//
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <mem.h>
#include "cmd_sub.h"

//根据传入的ninode找到对应块，比较content与名字，返回对应的inode_id给下一次迭代
int find_next_inode(int ninode, char *content) {
    int i, j;
    //读取ninode
    //如果该inode块大小为零，表示没有子文件夹，返回-1
    ino_list *p = NULL;
    printf_inode_block(1023, 0);
    p = malloc(MEM_BLOCK_SIZE);
    printf_inode_block(1023, 1);
    read_inode(ninode, (uint32_t *) p);
    int p_num_list = ninode % 32;
    int size = p->inodes[p_num_list].size;
    if (size == 0) {
        return -1;
    }
    //读取ninode指向的data块中8个dir_item信息
    //找到名字和传入content一样的，返回对应的inode_id给下一次迭代
    //没找到名字就返回0
    int group = size / 8;
    int rest = size % 8;
    for (i = 0; i < group; i++) {
        int data_block = p->inodes[p_num_list].block_point[i];
        dir_list *q = NULL;
        q = malloc(sizeof(dir_list));
        read_block(data_block, (uint32_t *) q);
        for (j = 0; j < 8; j++) {
            if (strcmp(q->dir_items[j].name, content) == 0) {
                return q->dir_items[j].inode_id;
            }
        }
        free(q);
    }
    int data_block = p->inodes[p_num_list].block_point[group];
    dir_list *q = NULL;
    q = malloc(sizeof(dir_list));
    read_block(data_block, (uint32_t *) q);
    for (j = 0; j < rest; j++) {
        if (strcmp(q->dir_items[group].name, content) == 0) {
            return q->dir_items[j].inode_id;
        }
    }
    //善后
    free(q);
    free(p);
    return 0;
}

int show_folder(int ninode) {
    int i, j;
    ino_list *p = NULL;
    p = malloc(sizeof(ino_list));
    read_inode(ninode, (uint32_t *) p);
    int p_num_list = ninode % 32;
    int size = p->inodes[p_num_list].size;
    int group = size / 8;
    int rest = size % 8;
    for (i = 0; i < group; i++) {
        int data_block = p->inodes[p_num_list].block_point[i];
        dir_list *q = NULL;
        q = malloc(sizeof(dir_list));
        read_block(data_block, (uint32_t *) q);
        for (j = 0; j < 8; j++) {
            printf("\t\ttype:%d,name:%s\n", q->dir_items[j].type, q->dir_items[j].name);
        }
        free(q);
    }
    int data_block = p->inodes[p_num_list].block_point[group];
    dir_list *q = NULL;
    q = malloc(sizeof(dir_list));
    read_block(data_block, (uint32_t *) q);
    for (j = 0; j < rest; j++) {
        printf("\t\ttype:%d,name:%s\n", q->dir_items[j].type, q->dir_items[j].name);
    }
    //善后
    free(q);
    free(p);
    return 0;
}

int build_new(int ninode, int new_ninode, int new_ndata, uint16_t file_type, char *content) {
//-----
    // 读取ninode
    ino_list *p = NULL;
    p = malloc(sizeof(ino_list));
    read_inode(ninode, (uint32_t *) p);
    // inode_num_list:inode在块中的序号
    int inode_num_list = ninode % 32;
    int free_dir = p->inodes[inode_num_list].size;
    if (free_dir >= 48) {
        return -1;
    }
    //修改inode
    p->inodes[inode_num_list].size += 1;
    p->inodes[inode_num_list].link += 1;
    int group = free_dir / 8;
    int rest = free_dir % 8;
    int data_block = p->inodes[inode_num_list].block_point[group];
    write_inode(ninode, (uint32_t *) p);
    free(p);
//-----
    //读取、修改dir
    dir_list *q = NULL;
    q = malloc(sizeof(dir_list));
    read_block(data_block, (uint32_t *) q);
    q->dir_items[rest].inode_id = new_ninode;
    q->dir_items[rest].type = file_type;
    q->dir_items[rest].valid = 1;
    strcpy(q->dir_items[rest].name, content);
    write_block(data_block, (uint32_t *) q);
    free(q);
//-----
    //读取、修改new_inode
    ino_list *p2 = NULL;
    p2 = malloc(sizeof(ino_list));
    read_inode(new_ninode, (uint32_t *) p2);
    int p2_num_list = new_ninode % 32;
    p2->inodes[p2_num_list].size += 1;
    p2->inodes[p2_num_list].file_type = file_type;
    p2->inodes[p2_num_list].link += 1;
    p2->inodes[p2_num_list].block_point[0] = new_ndata;//
    write_inode(new_ninode, (uint32_t *) p2);
    free(p2);
    return 0;
}

int set_sp_block(uint16_t file_type, int *new_ninode, int *new_ndata) {
    //读sp_block块
    sp_block *p = NULL;
    p = malloc(MEM_BLOCK_SIZE);
    read_block(0, (uint32_t *) p);

    //sp_block块是否有空
    if (!(p->free_block_count) || !(p->free_inode_count))
        return -1;
    //修改sp_block块
    *new_ninode = p->free_inode_count;
    *new_ndata = p->free_block_count;
    (p->free_block_count) -= 1;
    (p->free_inode_count) -= 1;
    if (file_type == TYPE_FOLDER)
        p->dir_inode_count += 1;
// TODO : modify maps (bits)
    //善后
    write_block(0, (uint32_t *) p);
    free(p);
    return 0;
}

void printf_sp_block(int i) {
    printf("\n-----------------\n");
    printf("%d\n", i);
    sp_block *q = NULL;
    q = malloc(MEM_BLOCK_SIZE);
    read_block(0, (uint32_t *) q);
    printf("magic_num:%d\t", q->magic_num);
    printf("free_block_count:%d\t", q->free_block_count);
    printf("free_inode_count:%d\t", q->free_inode_count);
    printf("dir_inode_count:%d\t", q->dir_inode_count);
    printf("\n-----------------\n");
    free(q);
}

void printf_inode_block(int ninode, int i) {
    printf("\n&&\n");
    printf("mark:%d\n", i);
    ino_list *pp = NULL;
    pp = malloc(sizeof(ino_list));
    read_inode(ninode, (uint32_t *) pp);
    int pp_num_list = ninode % 32;
    printf("inode:%d\n", ninode);
    printf("size:%d\t", pp->inodes[pp_num_list].size);
    printf("file_type:%d\t", pp->inodes[pp_num_list].file_type);
    printf("link:%d\t", pp->inodes[pp_num_list].link);
    printf("block_point[0]:%d\n", pp->inodes[pp_num_list].block_point[0]);
    printf("&&\n");
    free(pp);
}