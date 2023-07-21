/*
Do not modify this file.
Make all of your changes to main.c instead.
*/

#include "disk.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


// functions in unistd.h

//pread() reads up to count bytes from file descriptor fd at offset off‐ set (from the start of the file) into the buffer starting at buf. The file offset is not changed.
//On success, pread() returns the number of bytes read (a return of  zero indicates  end  of file)
extern ssize_t pread (int __fd, void *__buf, size_t __nbytes, __off_t __offset);

//pwrite() writes up to count bytes from the buffer starting at buf to the file descriptor fd at offset offset. The file offset is not changed.
//pwrite() returns the number of bytes written.
extern ssize_t pwrite (int __fd, const void *__buf, size_t __nbytes, __off_t __offset);



// structure to store disk information
struct disk {
	int fd;
	int block_size;
	int nblocks;
};



/*
Create a new virtual disk in the file "filename", with the given number of blocks.
Returns a pointer to a new disk object, or null on failure.
*/
// NOTE: A file emulates the disk in this program
struct disk * disk_open( const char *diskname, int nblocks )
{
	struct disk *d;

	d = malloc(sizeof(*d));		// allocate size equal to the size of struct disk i.e. one entry in disk
	if(!d) return 0;		// return 0 if malloc fails

	
	d->fd = open(diskname,O_CREAT|O_RDWR,0777);	// create a new file which emulates disk with read, write and execute permissions and make file descriptor of the disk structure point to it

	// if fails to create new file, free allocated resources and the disk entry and return 0
	if(d->fd<0) {
		free(d);
		return 0;
	}

	// define block size and no of blocks that the disk needs
	d->block_size = BLOCK_SIZE;			// BLOCK_SIZE = 4096 as defined in disk.h
	d->nblocks = nblocks;

	// make the file to be precisely of nblocks*block_size size
	// if it returns <0 then that means an error and the file cannot be truncated
	// close the file and frre the resources
	if(ftruncate(d->fd,d->nblocks*d->block_size)<0) {
		close(d->fd);
		free(d);
		return 0;
	}

	return d;
}



/*
Write exactly BLOCK_SIZE bytes to a given block on the virtual disk.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to the data to write.
*/
void disk_write( struct disk *d, int block, const char *data )
{
	// if block is out of scope of this disk, give error
	if(block<0 || block>=d->nblocks) {
		fprintf(stderr,"disk_write: invalid block #%d\n",block);
		abort();
	}

	int actual = pwrite(d->fd,data,d->block_size,block*d->block_size);
	
	// if actual no of bytes written are not equal to bytes told to write, then error
	if(actual!=d->block_size) {
		fprintf(stderr,"disk_write: failed to write block #%d: %s\n",block,strerror(errno));
		abort();
	}
}



/*
Read exactly BLOCK_SIZE bytes from a given block on the virtual disk.
"d" must be a pointer to a virtual disk, "block" is the block number,
and "data" is a pointer to where the data will be placed.
*/
void disk_read( struct disk *d, int block, char *data )
{
	// if block is out of scope of this disk, give error
	if(block<0 || block>=d->nblocks) {
		fprintf(stderr,"disk_read: invalid block #%d\n",block);
		abort();
	}

	int actual = pread(d->fd,data,d->block_size,block*d->block_size);

	// if actual no of bytes read are not equal to bytes told to read, then error
	if(actual!=d->block_size) {
		fprintf(stderr,"disk_read: failed to read block #%d: %s\n",block,strerror(errno));
		abort();
	}
}



/*
Return the number of blocks in the virtual disk.
*/
int disk_nblocks( struct disk *d )
{
	return d->nblocks;
}



/*
Close the virtual disk. Which is actually a file emulating the disk
*/
void disk_close( struct disk *d )
{
	close(d->fd);
	free(d);
}
