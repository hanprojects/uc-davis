#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "disk.h"
#include "fs.h"

#define FAT_EOC 0xFFFF
#define SB_SIG "ECS150FS"
#define SB_SIG_LEN 8
#define SB_UNUSED_LEN 4079
#define RD_UNUSED_LEN 10

// IMPORTANT NOTES:
// - No initialization to offset (size_t is nonnegative)
// - Dedicated to mom and TMS

// ------------------------------------------------------------------------- //
//                              DATA STRUCTURES                              //
// ------------------------------------------------------------------------- //

// Structure for super block
typedef struct __attribute__((packed)) SuperBlock
{
    uint8_t signature[SB_SIG_LEN];
    uint16_t total_virtual_disk;
    uint16_t root_dir_index;
    uint16_t data_start_index;
    uint16_t data_num;
    uint8_t FAT_num;
    uint8_t unused[SB_UNUSED_LEN];
}
SuperBlock;

// Structure for root directory
typedef struct __attribute__((packed)) RootDirectory
{
    uint8_t file_name[FS_FILENAME_LEN];
    uint32_t file_size;
    uint16_t first_data_index;
    uint8_t unused[RD_UNUSED_LEN];
}
RootDirectory;

// Structure for open-file table
typedef struct OpenFileTable
{
    size_t offset;
    int file_index_on_root;
}
OpenFileTable;

// Global structures
SuperBlock *super_block;
uint16_t *FAT_blocks;
RootDirectory *root_dir;
OpenFileTable open_file_table[FS_OPEN_MAX_COUNT];

int open_file_num = 0;

// ------------------------------------------------------------------------- //
//                             HELPER FUNCTIONS                              //
// ------------------------------------------------------------------------- //

// Function to count number of free FAT blocks
// Loop through FAT blocks (in bytes) and check if it's 0 (free)
// NOTE: data_num = block_count - FAT_num - 2
int count_free_FAT_blocks()
{
    int free_fat_num = 0;

    for (int i = 0; i < super_block->data_num; i++)
    {
        if (FAT_blocks[i] == 0)
        {
            free_fat_num += 1;
        }
    }

    return free_fat_num;
}

// Function to count number of free root file
// NOTE: Loop through root dir and check if the first char
// of filename is '\0'
int count_free_root_files()
{
    int free_file_num = 0;

    for (int i = 0; i < FS_FILE_MAX_COUNT; i++)
    {
        // Source: Piazza @544
        if (root_dir[i].file_name[0] == '\0')
        {
            free_file_num += 1;
        }
    }

    return free_file_num;
}

// Function to check if file already exists in root dir
// NOTE: Loop through root dir and find the filename
int get_file_index_in_root(const char *filename)
{
    int index_file_in_root = -1;

    for (int i = 0; i < FS_FILE_MAX_COUNT; i++)
    {
        // WARNING: NULL-terminated
        if (strncmp((char*)root_dir[i].file_name, filename,
                                        FS_FILENAME_LEN) == 0)
        {
            index_file_in_root = i;
            break;
        }
    }

    return index_file_in_root;
}

// Function to check if file is currently open
// NOTE: Loop through open-file table and check the file index
int is_file_open(int file_index)
{
    int is_open = 0;

    for (int i = 0; i < FS_OPEN_MAX_COUNT; i++)
    {
        if (open_file_table[i].file_index_on_root == file_index)
        {
            is_open = 1;
            break;
        }
    }

    return is_open;
}

// Function to get current FAT
int get_curr_FAT(int curr_FAT, int file_offset)
{
    for (int i = 0; i < file_offset/BLOCK_SIZE; i++)
    {
        curr_FAT = FAT_blocks[curr_FAT];
    }

    return curr_FAT;
}

// Function to get the next free FAT block
int get_next_free_FAT(int start)
{
    int next_free_FAT = FAT_EOC;

    for (int i = start + 1; i < super_block->data_num; i++)
    {
        if (FAT_blocks[i] == 0)
        {
            next_free_FAT = i;
            break;
        }
    }

    return next_free_FAT;
}

// Function to return min number
int min(int a, int b)
{
    if (a < b)
        return a;

    return b;
}

// Function to return max number
int max(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

// ------------------------------------------------------------------------- //
//                                  PHASE 1                                  //
// ------------------------------------------------------------------------- //

// fs_mount() - Mount the file system that @diskname contains
int fs_mount(const char *diskname)
{
    // If cannot open disk, return -1
    if (block_disk_open(diskname) == -1)
    {
        fprintf(stderr, "fs_mount - Cannot open disk\n");
        return -1;
    }

    // READ SUPERBLOCK - If unable to read superblock, return -1
    super_block = malloc(BLOCK_SIZE);

    if (!super_block) // Alloc fails
    {
        return -1;
    }

    if (block_read(0, super_block) == -1)
    {
        return -1;
    }

    // If not file system ECS150FS, return -1
    // WARNING: Use strncmp for NULL-terminated
	if (strncmp((char*)super_block->signature, SB_SIG, SB_SIG_LEN) != 0)
	{
        return -1;
	}

    // If number of virtual blocks do not match, return -1
    // NOTE: Read from project3 prompt
    if (super_block->total_virtual_disk != block_disk_count())
    {
        return -1;
    }

    // READ FAT - If unable to read FAT, return -1
    // WARNING: FAT entries is BLOCK_SIZE/2 max,
    // but data size can be BLOCK_SIZE bytes max
    FAT_blocks = malloc(super_block->FAT_num * BLOCK_SIZE);

    if (!FAT_blocks) // Alloc fails
    {
        return -1;
    }

    for (int i = 0; i < super_block->FAT_num; i++)
    {
        // NOTE: First FAT block does not matter
        if (block_read(i + 1, FAT_blocks + (i * BLOCK_SIZE)) == -1)
        {
            return -1;
        }
    }

    // READ ROOT DIRECTORY - If unable to read RD, return -1
    root_dir = malloc(BLOCK_SIZE);

    if (!root_dir) // Alloc fails
    {
        return -1;
    }

    if (block_read(super_block->FAT_num + 1, root_dir) == -1)
    {
        return -1;
    }

    // Initialize open file table
    for (int i = 0; i < FS_OPEN_MAX_COUNT; i++)
    {
        open_file_table[i].file_index_on_root = -1;
    }

    return 0;
}

// fs_umount() - Unmount the currently mounted file system
// and close the underlying virtual disk file.
int fs_umount(void)
{
    // If not FS currently mounted, return -1
    if (!super_block || block_disk_count() <= 0)
    {
        return -1;
    }

    // If there is still open fd, return -1
    if (open_file_num > 0)
    {
        return -1;
    }

    // WRITE TO SUPERLBOCK - If unable, return -1
    if (block_write(0, super_block) == -1)
    {
        return -1;
    }

    // WRITE TO ROOT DIR - If unable, return -1
    if (block_write(super_block->FAT_num + 1, root_dir) == -1)
    {
        return -1;
    }

    // WRITE TO FAT BLOCK - If unable, return -1
    for (int i = 0; i < super_block->FAT_num; i++)
    {
        if (block_write(i + 1, FAT_blocks + (i * BLOCK_SIZE)) == -1)
        {
            return -1;
        }
    }

    // Close disk - If unable, return -1
    if (block_disk_close() == -1)
    {
        return -1;
    }

    // Free resources
    free(super_block);
    free(FAT_blocks);
    free(root_dir);

    // Reset open-file table
    for (int i = 0; i < FS_OPEN_MAX_COUNT; i++)
    {
        open_file_table[i].file_index_on_root = -1;
    }

    return 0;
}

// fs_info() - Display information about the mounted file system
// NOTE: Need to be exactly matched with the reference program
int fs_info(void)
{
    // Error checkings (no disk currently open)
    if (!super_block || block_disk_count() <= 0)
    {
        fprintf(stderr, "fs_info - No FS mounted");
        return -1;
    }

    // Get number of free FAT blocks and free root dir
    int free_FAT_blocks = count_free_FAT_blocks();
    int free_root_dir = count_free_root_files();

    // Print out information
    // Source: Piazza post @535 (for ratios)
    printf("FS Info:\n");
    printf("total_blk_count=%d\n", super_block->total_virtual_disk);
    printf("fat_blk_count=%d\n", super_block->FAT_num);
    printf("rdir_blk=%d\n", super_block->FAT_num + 1);
    printf("data_blk=%d\n", super_block->FAT_num + 2);
    printf("data_blk_count=%d\n", super_block->data_num);
    printf("fat_free_ratio=%d/%d\n", free_FAT_blocks, super_block->data_num);
    printf("rdir_free_ratio=%d/%d\n", free_root_dir, FS_FILE_MAX_COUNT);

    return 0;
}

// ------------------------------------------------------------------------- //
//                                  PHASE 2                                  //
// ------------------------------------------------------------------------- //

// fs_create() - Create a new and empty file named @filename
// in the root directory of the mounted file system
int fs_create(const char *filename)
{
    // If no FS mounted, return -1
    // Source: Piazza post @572
    if (!super_block || block_disk_count() <= 0)
    {
        fprintf(stderr, "fs_create - No FS mounted\n");
        return -1;
    }

    // If filename is NULL or empty, return -1
    if (filename == NULL || strlen(filename) == 0)
    {
        return -1;
    }

    // If filename is too long, return -1
    if (strlen(filename) > FS_FILENAME_LEN)
    {
        return -1;
    }

    // GET FIRST FREE ROOT FILE ENTRY - If filename is there, return -1
    int first_empty_index = -1;

    for (int i = 0; i < FS_FILE_MAX_COUNT; i++)
    {
        // If file already exist in root, return -1
        // NOTE: @filename string must be NULL-terminated
        if (strncmp((char*)root_dir[i].file_name, filename, FS_FILENAME_LEN) == 0)
        {
            return -1;
        }

        // Else, if first empty file, store its index
        if (root_dir[i].file_name[0] == '\0')
        {
            if (first_empty_index == -1)
            {
                first_empty_index = i;
            }
        }
    }

    // If no free empty file in root directory (all 128 used)
    if (first_empty_index == -1)
    {
        return -1;
    }

    else
    {
        // Reset other information (see p3 prompt)
        memset(&root_dir[first_empty_index], 0,
                sizeof(root_dir[first_empty_index]));

        // Set up file name, size and first index (see p3 prompt)
        strcpy((char*) root_dir[first_empty_index].file_name, filename);
        root_dir[first_empty_index].file_size = 0;
        root_dir[first_empty_index].first_data_index = FAT_EOC;

        return 0;
    }

    return -1; // Any other errors
}

// fs_delete - Delete the file named @filename from
// the root directory of the mounted file system.
int fs_delete(const char *filename)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() <= 0)
    {
        return -1;
    }

    // If filename is NULL or empty, return -1
    if (filename == NULL || strlen(filename) == 0)
    {
        return -1;
    }

    // If filename is too long, return -1
    if (strlen(filename) > FS_FILENAME_LEN)
    {
        return -1;
    }

    // If there is no file named @filename in root dir, return -1
    int file_index = get_file_index_in_root(filename);

    if (file_index == -1)
    {
        return -1;
    }

    // If the file is currently opened, return -1
    int is_open = is_file_open(file_index);

    if (is_open == 1)
    {
        return -1;
    }

    // DELETING PROCESS
    int curr_index = root_dir[file_index].first_data_index;

    while (curr_index != FAT_EOC)
    {
        uint16_t saved_FAT = FAT_blocks[curr_index];
        FAT_blocks[curr_index] = 0;
        curr_index = saved_FAT;
    }

    // Reset file name and root memory
    // Source: Piazza post @545_f1
    root_dir[file_index].file_name[0] = '\0';
    root_dir[file_index].file_size = 0;

    // NOTE: Piazza @556 says zero-out is unnecessary

    return 0;
}

// fs_ls() - List information about the files
// located in the root directory
// NOTE: Must be exactly the same with reference program
int fs_ls(void)
{
    // If no FS currently mounted, return -1
    if (!super_block || block_disk_count() <= 0)
    {
        return -1;
    }

    // Print information of list of files
    printf("FS Ls:\n");

    for (int i = 0; i < FS_FILE_MAX_COUNT; i++)
    {
        printf("file: %s, ", root_dir[i].file_name);
        printf("size: %d, ", root_dir[i].file_size);
        printf("data_blk: %d\n", root_dir[i].first_data_index);
    }

    return 0;
}

// ------------------------------------------------------------------------- //
//                                  PHASE 3                                  //
// ------------------------------------------------------------------------- //

// fs_open() - Open file @filename
// NOTE: Increment open_file_num after opening file
// NOTE: Piazza @565 says a file can be opened multiple times
int fs_open(const char *filename)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If filename is NULL or empty, return -1
    if (filename == NULL || strlen(filename) == 0)
    {
        return -1;
    }

    // If filename is too long, return -1
    if (strlen(filename) > FS_FILENAME_LEN)
    {
        return -1;
    }

    // If file is not on root dir to open, return -1
    int file_index = get_file_index_in_root(filename);

    if (file_index == -1)
    {
        return -1;
    }

    // If already max open files, return -1;
    if (open_file_num == FS_OPEN_MAX_COUNT)
    {
        return -1;
    }

    // Find the first empty spot on open-file table
    int first_open_empty = -1;

    for (int i = 0; i < FS_OPEN_MAX_COUNT; i++)
    {
        if (open_file_table[i].file_index_on_root == -1)
        {
            first_open_empty = i;
            break;
        }
    }

    // Put opened file on open-file table
    open_file_table[first_open_empty].file_index_on_root = file_index;
    open_file_table[first_open_empty].offset = 0;

    open_file_num += 1;
    return first_open_empty;
}

// fs_close() - Close file descriptor @fd
// NOTE: Decrement open_file_num after closing file
int fs_close(int fd)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If fd is invalid (out of bound or not open)
    if (fd < 0 || fd >= FS_OPEN_MAX_COUNT
               || open_file_table[fd].file_index_on_root == -1)
    {
        return -1;
    }

    // Reset information about the closed file on open-file table
    open_file_table[fd].file_index_on_root = -1;

    open_file_num -= 1;
    return 0;
}

// fs_stat() - Return file size associated with @fd
int fs_stat(int fd)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If fd is invalid (out of bound or not open)
    if (fd < 0 || fd >= FS_OPEN_MAX_COUNT
               || open_file_table[fd].file_index_on_root == -1)
    {
        return -1;
    }

    // Return current size of file associated with @fd
    int file_index = open_file_table[fd].file_index_on_root;
    int file_size = root_dir[file_index].file_size;

    return file_size;
}

// fs_lseek() - Set the file offset (used for read and write operations)
// associated with file descriptor @fd to the argument @offset
// NOTE: fs_lseek(fd, fs_stat(fd)); (call form)
int fs_lseek(int fd, size_t offset)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If fd is invalid (out of bound or not open)
    if (fd < 0 || fd >= FS_OPEN_MAX_COUNT
               || open_file_table[fd].file_index_on_root == -1)
    {
        return -1;
    }

    // Get file size from fs_stat
    int file_size = fs_stat(fd);

    // If @offset is larger than the current file size, return -1
    if (offset >= (size_t)file_size)
    {
        return -1;
    }

    open_file_table[fd].offset = offset;
    return 0;
}

// ------------------------------------------------------------------------- //
//                                  PHASE 4                                  //
// ------------------------------------------------------------------------- //

// fs_write() - Attempt to write @count bytes of data from buffer pointer
// by @buf into the file referenced by file descriptor @fd.
// Source: project3.pdf Discussion - (small/large op, bounce buffer, etc.)
int fs_write(int fd, void *buf, size_t count)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If fd is invalid (out of bound or not open)
    if (fd < 0 || fd >= FS_OPEN_MAX_COUNT
               || open_file_table[fd].file_index_on_root == -1)
    {
        return -1;
    }

    // If buf is NULL, return -1
    if (buf == NULL)
    {
        return -1;
    }

    // If count == 0, no bytes need to be written
    if (count == 0)
    {
        return 0;
    }

// ------------------ WRITING FILE SETUP ------------------ //

    int num_bytes_written = 0;
    int num_bytes_left = count;
    char *buf_copy = (char*) buf;

    int file_index = open_file_table[fd].file_index_on_root;
    int file_offset = open_file_table[fd].offset;
    int file_size = root_dir[file_index].file_size;

    int left_offset = file_offset % BLOCK_SIZE;
    int right_offset;

    int start_FAT = root_dir[file_index].first_data_index;
    int FAT_iterator = get_curr_FAT(start_FAT, file_offset);

    // Get number of blocks to write
    int num_blocks_required = (count + left_offset)/BLOCK_SIZE;
    if ((count + left_offset) % BLOCK_SIZE != 0)
    {
        num_blocks_required += 1;
    }

    // Get number of extra blocks from current file block num
    int extra_blocks = num_blocks_required;

    if (file_size > 0)
    {
        // Get number of blocks in current file
        int file_block_num = file_size/BLOCK_SIZE;
        if (file_size % BLOCK_SIZE != 0)
        {
            file_block_num += 1;
        }

        // Get number of extra offset blocks
        // NOTE: look at project3.pdf to visualize
        int offset_block_num = file_offset/BLOCK_SIZE;
        if (file_offset % BLOCK_SIZE != 0)
        {
            offset_block_num += 1;
        }

        // If extra bytes not exceed file size, no need
        // to add extra blocks
        int new_block_num = (count + file_offset)/BLOCK_SIZE;
        if ((count + file_offset) % BLOCK_SIZE != 0)
        {
            new_block_num += 1;
        }

        if ((count + file_offset) <= (size_t)file_size
            || new_block_num == file_block_num)
        {
            extra_blocks = 0;
        }

        else
        {
            // WARNING: +1 to avoid segfault
            extra_blocks += (offset_block_num - file_block_num + 1);
        }
    }

    // If extra blocks exceed the number of free FAT blocks,
    // write as much as possible
    int free_FAT_num = count_free_FAT_blocks();

    if (free_FAT_num == 0)
    {
        return 0;
    }

    extra_blocks = min(extra_blocks, free_FAT_num);

    // If freshly new file, update first data index on
    // root directory
    if (start_FAT == FAT_EOC)
    {
        FAT_iterator = get_next_free_FAT(0);
        root_dir[file_index].first_data_index = FAT_iterator;
    }

    // Allocate FAT blocks to write extra blocks of bytes
    // if larger than current file size
    if (extra_blocks > 0)
    {
        // Store the index of potential FAT EOC block
        int prev = FAT_iterator;

        // Reach to the end of file block chain
        while (start_FAT != FAT_EOC)
        {
            prev = start_FAT;
            start_FAT = FAT_blocks[start_FAT];
        }

        // At the block of FAT_EOC, replace and allocate
        // extra FAT blocks from there
        // NOTE: first-first strategy
        for (int i = 0; i < extra_blocks; i++)
        {
            FAT_blocks[prev] = get_next_free_FAT(i);
            prev = FAT_blocks[prev];
        }

        FAT_blocks[prev] = FAT_EOC;
    }

    // If not enough free FAT blocks to write,
    // write as much as possible
    int num_blocks_to_write = min(num_blocks_required, free_FAT_num);

// ------------------ WRITE TO FILE HERE --------------------- //

    char *bounce_buffer = malloc(BLOCK_SIZE);
    if (!bounce_buffer) // Alloc fails
    {
        return -1;
    }

    // Determine right offset for the first block
    if ((left_offset + num_bytes_left) <= BLOCK_SIZE)
    {
        right_offset = num_bytes_left;
    }
    else
    {
        right_offset = BLOCK_SIZE - left_offset;
    }

    // Write first block (use block_read to modify)
    block_read(FAT_iterator + super_block->data_start_index, bounce_buffer);
    memcpy(bounce_buffer + left_offset, buf_copy, right_offset);
    block_write(FAT_iterator + super_block->data_start_index, bounce_buffer);

    num_bytes_written += right_offset;
    num_bytes_left -= right_offset;
    buf_copy += right_offset;

    // If only need to write one block, set FAT_EOC and return
    if (num_blocks_to_write == 1)
    {
        FAT_blocks[FAT_iterator] = FAT_EOC;
    }

    // Else, direct writing to file
    else if (num_blocks_to_write > 1)
    {
        // Go to next middle blocks
        FAT_iterator = FAT_blocks[FAT_iterator];

        int i = 1;
        while (i < num_blocks_to_write - 1)
        {
            // In middle blocks, right offset is always BLOCK_SIZE
            right_offset = BLOCK_SIZE;

            // Direct writing in middle blocks
            block_write(FAT_iterator + super_block->data_start_index, buf_copy);

            // Update number of bytes written and num bytes left
            // and buffer
            num_bytes_written += right_offset;
            num_bytes_left -= right_offset;
            buf_copy += right_offset;

            // Go to next FAT block
            FAT_iterator = FAT_blocks[FAT_iterator];

            i++;
        }

        // If number of bytes left is larger than the capacity
        // of the last block, write as much as possible
        right_offset = min(num_bytes_left, BLOCK_SIZE);

        // Write to the last block (need bounce buffer)
        block_read(FAT_iterator + super_block->data_start_index, bounce_buffer);
        memcpy(bounce_buffer, buf_copy, right_offset);
        block_write(FAT_iterator + super_block->data_start_index, bounce_buffer);

        num_bytes_written += right_offset;
        num_bytes_left -= right_offset;
        buf_copy += right_offset;

        // Set FAT_EOC to the last FAT block
        FAT_blocks[FAT_iterator] = FAT_EOC;
    }

    // Update offset of file on open-file table
    open_file_table[fd].offset += (size_t)num_bytes_written;

    // Update file size on root directory
    int updated_size = file_offset + num_bytes_written;
    root_dir[file_index].file_size = max(updated_size, file_size);

    return num_bytes_written;
}

// fs_read() - Attempt to read @count bytes of data
// from the file referenced by file descriptor @fd
// Source: project3.pdf Discussion (bounce buffer, read/memcpy, etc.)
int fs_read(int fd, void *buf, size_t count)
{
    // If no FS mounted, return -1
    if (!super_block || block_disk_count() == 0)
    {
        return -1;
    }

    // If fd is invalid (out of bound or not open)
    if (fd < 0 || fd >= FS_OPEN_MAX_COUNT
               || open_file_table[fd].file_index_on_root == -1)
    {
        return -1;
    }

    // If buf is NULL, return -1
    if (buf == NULL)
    {
        return -1;
    }

    // If count == 0, no bytes need to be written
    if (count == 0)
    {
        return 0;
    }

// ------------------ READING FILE SETUP ------------------ //

    int num_bytes_read = 0;
    int num_bytes_left = count;
    char *buf_copy = (char*) buf;

    int file_index = open_file_table[fd].file_index_on_root;
    int file_size = root_dir[file_index].file_size;
    int file_offset = (int)open_file_table[fd].offset;

    int num_blocks_to_read = 0;
    int left_offset = file_offset % BLOCK_SIZE;
    int right_offset = 0;

    int start_FAT = root_dir[file_index].first_data_index;
    int FAT_iterator = get_curr_FAT(start_FAT, file_offset);

    // If freshly new file never written, return 0
    if (start_FAT == FAT_EOC)
    {
        return 0;
    }

    // Set bytes to read (as much as possible from current offset)
    num_bytes_left = min(count, file_size - file_offset);

    num_blocks_to_read = num_bytes_left/BLOCK_SIZE;
    if (num_bytes_left % BLOCK_SIZE != 0)
    {
        num_blocks_to_read += 1;
    }

// ------------------ READING FILE HERE ------------------ //

    // Source: Piazza @594 (no need to allocate
    // BLOCK_SIZE * num_blocks_to_read)
    char *bounce_buffer = malloc(BLOCK_SIZE);
    if (!bounce_buffer) // Alloc fails
    {
        return -1;
    }

    // Determine right offset for the first block
    if (num_bytes_left + left_offset <= BLOCK_SIZE)
    {
        right_offset = num_bytes_left;
    }

    else
    {
        right_offset = BLOCK_SIZE - left_offset;
    }

    // Read the first block with bounce buffer
    block_read(FAT_iterator + super_block->data_start_index, bounce_buffer);
    memcpy(buf_copy, bounce_buffer + left_offset, right_offset);

    num_bytes_read += right_offset;
    num_bytes_left -= right_offset;
    buf_copy += right_offset;

    if (num_blocks_to_read > 1)
    {
        FAT_iterator = FAT_blocks[FAT_iterator];
        int i = 1;

        // In middle blocks, just need direct reading
        while (i < num_blocks_to_read - 1)
        {
            right_offset = BLOCK_SIZE;

            // Read bytes in middle blocks - only direct reading
            block_read(FAT_iterator + super_block->data_start_index, buf_copy);

            // Update number of bytes that have been read,
            // number of bytes left to read and buffer
            num_bytes_read += right_offset;
            num_bytes_left -= right_offset;
            buf_copy += right_offset;

            // Get the next FAT block
            FAT_iterator = FAT_blocks[FAT_iterator];

            i++;
        }

        // Double check if num_bytes_left not larger
        // than the last block can handle
        right_offset = min(num_bytes_left, BLOCK_SIZE);

        // Read the last block (need bounce buffer)
        block_read(FAT_iterator + super_block->data_start_index, bounce_buffer);
        memcpy(buf_copy, bounce_buffer, right_offset);

        num_bytes_read += right_offset;
        num_bytes_left -= right_offset;
        buf_copy += right_offset;
    }

    // Update offset on open-file table
    open_file_table[fd].offset += (size_t)num_bytes_read;

    return num_bytes_read;
}
