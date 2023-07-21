/*
Do not modify this file.
Make all of your changes to main.c instead.
*/
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ucontext.h>

#include "page_table.h"



// structure holding the meta-data for page table
struct page_table {
	int fd;			// points to page table emulated by a file
	char *virtmem;		// pointer to start of virtual memory
	int npages;		// no of pages in virtual memory
	char *physmem;		// pointer to start of physical memory
	int nframes;		// no of frames in physical memory
	int *page_mapping;	// pointer to start of mapping between physical and virtual memory
	int *page_bits;		// pointer to start of permission bits list 
	page_fault_handler_t handler;	//page fault handle. Will be written by us
};



// create a placeholder pointer for page table
struct page_table *the_page_table = 0;

//void remap_file_pages( pt->virtmem + page * PAGE_SIZE, PAGE_SIZE, 0, frame, 0);




static void internal_fault_handler( int signum, siginfo_t *info, void *context )
{

// get appropriate address of fault based on machine
#ifdef i386
	char *addr = (char*)(((struct ucontext *)context)->uc_mcontext.cr2);
#else
	char *addr = info->si_addr;
#endif

	// get page table
	struct page_table *pt = the_page_table;

	// if page table valid
	if(pt) {
		int page = (addr - pt->virtmem) / PAGE_SIZE;	// find page in virtual memory

		if(page>=0 && page<pt->npages) {	// if page is within bounds and is in memory
			pt->handler(pt,page);
			return;
		}
	}

	// if page table not valid then illegal memory access therefore segmentation fault, abort the action
	fprintf(stderr,"segmentation fault at address %p\n",addr);
	abort();
}



/* Create a new page table, along with a corresponding virtual memory
that is "npages" big and a physical memory that is "nframes" bit
 When a page fault occurs, the routine pointed to by "handler" will be called. */
struct page_table * page_table_create( int npages, int nframes, page_fault_handler_t handler )
{
	int i;
	struct sigaction sa;
	struct page_table *pt;
	char filename[256];

	pt = malloc(sizeof(struct page_table));
	if(!pt) return 0;			// if malloc fails return 0 i.e. page table not created

	the_page_table = pt;		// assign the created page to be pointed to by the placeholder pointer created earlier		

	sprintf(filename,"/tmp/pmem.%d.%d",getpid(),getuid());	// generate a unique file name

	// create a new file which emulates page table
	pt->fd = open(filename,O_CREAT|O_TRUNC|O_RDWR,0777);
	if(!pt->fd) return 0;		// if file creation fails, then return 0


	// truncate the file to precisely PAGE_SIZE*npages
	ftruncate(pt->fd, PAGE_SIZE*npages);

	// Call the unlink function to remove the specified FILE.	//DOUBT
	unlink(filename);

	// creates a new mapping for (emulating) physical memory in the virtual address space of process
	pt->physmem = mmap(0, nframes*PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, pt->fd, 0);
	
	//assign total no of frames
	pt->nframes = nframes;

	// creates a new mapping for (emulating) virtual memory in the virtual address space of process
	pt->virtmem = mmap(0, npages*PAGE_SIZE, PROT_NONE, MAP_SHARED|MAP_NORESERVE, pt->fd, 0);

	//assign total no of pages
	pt->npages = npages;

	// create space to store file bits for all pages
	pt->page_bits = malloc(sizeof(int)*npages);

	// create space to store page mapping for all possible pages
	pt->page_mapping = malloc(sizeof(int)*npages);

	// assign page-fault handler
	pt->handler = handler;

	// make page bits for all pages to be 0
	for(i=0;i<pt->npages;i++) pt->page_bits[i] = 0;


	// set the action the process should take upon receiving a particular signal
 	sa.sa_sigaction = internal_fault_handler;	// the specific signal and the action is stored in the internal fault handler.


	// sa_flags specifies a set of flags which modify the behavior of the signal.
	/*	SA_SIGINFO (since Linux 2.2)
                  The  signal handler takes three arguments, not one.  In this
                  case, sa_sigaction should  be  set  instead  of  sa_handler.
                  This flag is meaningful only when establishing a signal han‐
                  dler.
	*/
	sa.sa_flags = SA_SIGINFO;			


	//'sigfillset' initializes a signal set to contain all signals. The signal set sa_mask is the set of signals that are blocked when the 		signal handler is being executed. So, when you are executing a signal handler all signals are blocked and you don't have to worry for 		another signal interrupting your signal handler.
	sigfillset( &sa.sa_mask );


	// The  sigaction()  system  call  is used to change the action taken by a process on receipt of a specific signal.
	sigaction( SIGSEGV, &sa, 0 );	// sigaction system call

	// it works when SIGSEGV is received. His signal means the page is not found in virtual memory

	return pt;
}



/* Delete a page table and the corresponding virtual and physical memories. */
// This does not delete the disk
void page_table_delete( struct page_table *pt )
{
	// unmap the mappings of physical memory and virtual memory for the virtual address space of the process.
	munmap(pt->virtmem,pt->npages*PAGE_SIZE);
	munmap(pt->physmem,pt->nframes*PAGE_SIZE);

	// free the list of page bits
	free(pt->page_bits);

	// free the list of page mappings
	free(pt->page_mapping);

	// close the file descriptor which points to the page table	
	close(pt->fd);

	// free the page table structure which contains information about page table
	free(pt);
}



/*
Set the frame number and access bits associated with a page.
The bits may be any of PROT_READ, PROT_WRITE, or PROT_EXEC logical-ored together.
*/
void page_table_set_entry( struct page_table *pt, int page, int frame, int bits )
{
	// if page out of bounds
	if( page<0 || page>=pt->npages ) {
		fprintf(stderr,"page_table_set_entry: illegal page #%d\n",page);
		abort();
	}

	// if frame out of bounds
	if( frame<0 || frame>=pt->nframes ) {
		fprintf(stderr,"page_table_set_entry: illegal frame #%d\n",frame);
		abort();
	}

	// otherwise map frame to page.
	pt->page_mapping[page] = frame;

	// Assign page bits received as parameters
	pt->page_bits[page] = bits;

	// Create a nonlinear  mapping, that is, a mapping in which the pages of the file are mapped into a nonsequential order in memory.
	//remap_file_pages( pt->virtmem + page * PAGE_SIZE, PAGE_SIZE, 0, frame, 0);

	// changes protection of the page as per the parameter protection bits passed
	mprotect(pt->virtmem + page * PAGE_SIZE, PAGE_SIZE, bits);
}



/*
Get the frame number and access bits associated with a page.
"frame" and "bits" must be pointers to integers which will be filled with the current values.
The bits may be any of PROT_READ, PROT_WRITE, or PROT_EXEC logically ORed together.
*/
void page_table_get_entry( struct page_table *pt, int page, int *frame, int *bits )
{
	// if page out of bounds. error
	if( page<0 || page>=pt->npages ) {
		fprintf(stderr,"page_table_get_entry: illegal page #%d\n",page);
		abort();
	}

	// otherwise get frame mapped to page and get page bits
	*frame = pt->page_mapping[page];
	*bits = pt->page_bits[page];
}



/* Print out the page table entry for a single page. */
void page_table_print_entry( struct page_table *pt, int page )
{
	// if page not in bounds of page table
	if( page<0 || page>=pt->npages ) {
		fprintf(stderr,"page_table_print_entry: illegal page #%d\n",page);
		abort();
	}

	// take out permission bits of the page
	int b = pt->page_bits[page];

	// print out the entry with page no., frame no. and permission bits
	printf("page %06d: frame %06d bits %c%c%c\n",
		page,
		pt->page_mapping[page],
		b&PROT_READ  ? 'r' : '-',
		b&PROT_WRITE ? 'w' : '-',
		b&PROT_EXEC  ? 'x' : '-'
	);

}



/* Print out the state of every page in a page table. */
void page_table_print( struct page_table *pt )
{
	int i;
	for(i=0;i<pt->npages;i++) {
		page_table_print_entry(pt,i);
	}
}



/* Return the total number of frames in the physical memory. */
int page_table_get_nframes( struct page_table *pt )
{
	return pt->nframes;
}



/* Return the total number of pages in the virtual memory. */
int page_table_get_npages( struct page_table *pt )
{
	return pt->npages;
}



/* Return a pointer to the start of the virtual memory associated with a page table. */
char * page_table_get_virtmem( struct page_table *pt )
{
	return pt->virtmem;
}



/* Return a pointer to the start of the physical memory associated with a page table. */
char * page_table_get_physmem( struct page_table *pt )
{
	return pt->physmem;
}
