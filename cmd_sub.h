//
// Created by MACHENIKE on 12/21/2020.
//

#ifndef OSFILE_CMD_SUB_H
#define OSFILE_CMD_SUB_H

int set_sp_block(uint16_t file_type, int *nblock, int *ninode);

int set_inode_block(uint16_t file_type, int *nblock, int *ninode);

#endif //OSFILE_CMD_SUB_H
