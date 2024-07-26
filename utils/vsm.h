#ifndef VSM_H_
#define VSM_H_


typedef struct vsm vsm_t;	
	
/* initialize the memory buffer */	
vsm_t *VsmInit(void *mem, long size);	
	
/*clear the allocated memory */	
void VsmClear(vsm_t *vsm);	
	
/* allocate a single block O(n) */	
void *VsmAlloc(vsm_t *vsm, long size);	
	
/* "free" an allocated block  O(1) */	
void VsmFree(void *block);


#endif /* VSM_H_ */
