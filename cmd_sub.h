//
// Created by MACHENIKE on 12/21/2020.
//

#ifndef OSFILE_CMD_SUB_H
#define OSFILE_CMD_SUB_H

#include "filesys.h"

int find_next_inode(int ninode, char *content);

int show_folder(int ninode);

int build_new(int ninode, int new_ninode, int new_ndata, uint16_t file_type, char *content);

int set_sp_block(uint16_t file_type, int *new_ninode, int *new_ndata);

int get_new_inode(uint16_t file_type, int *new_ninode);

int get_new_block(int *new_ndata);

int find_the_inode(int ninode, char *content);

void printf_sp_block(int i);

void printf_inode_block(int ninode, int i);

char *parse_content(char *content, char *whitespace);

void data_cpy(int src, int dst);

#endif //OSFILE_CMD_SUB_H
