#include <stdlib.h>

#include "spin_lock.h"


struct spin_lock
{
	int locked;	
};

spin_lock_t *SpinLockCreate(void)
{
	spin_lock_t *sl = (spin_lock_t *)calloc(1, sizeof(*sl));

	return (sl);
}

void SpinLockDestroy(spin_lock_t *sl)
{
	free(sl); sl = NULL;
}

void SpinLockLock(spin_lock_t *sl)
{
	while (sl->locked);
	while (__sync_lock_test_and_set(&(sl->locked), 1));
}

void SpinLockUnlock(spin_lock_t *sl)
{	
	__sync_lock_release(&(sl->locked));
}
