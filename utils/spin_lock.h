#ifndef SPIN_LOCK_H_
#define SPIN_LOCK_H_

typedef struct spin_lock spin_lock_t;

spin_lock_t *SpinLockCreate();
void SpinLockDestroy(spin_lock_t *lock);
void SpinLockLock(spin_lock_t *lock);
void SpinLockUnlock(spin_lock_t *lock);

#endif /* SPIN_LOCK_H_ */
