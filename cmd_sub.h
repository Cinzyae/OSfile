//
// Created by MACHENIKE on 12/21/2020.
//

#ifndef OSFILE_CMD_SUB_H
#define OSFILE_CMD_SUB_H

#include "filesys.h"

int ls_sub(int ninode, char *content);

int touch_sub(int ninode, int ndata, uint16_t file_type, char *content);

int set_sp_block(uint16_t file_type, int *ninode, int *ndata);

int set_inode_block(uint16_t file_type, int *nblock, int *ninode);

#endif //OSFILE_CMD_SUB_H
