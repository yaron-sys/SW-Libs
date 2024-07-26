#include <unistd.h>	    /* getpid */

#include "uuid.h"


uuid_t UuidCreate(void)
{
	static size_t count = 0;
	uuid_t new_uid = {0};
	
	++count;
	
	new_uid.pid = getpid();
	gettimeofday(&new_uid.time, NULL);
	new_uid.ctr = count;
	
	return (new_uid);
}
	
int UuidIsequal(uuid_t uid1, uuid_t uid2)
{
	return ((uid1.pid == uid2.pid) &&
			(uid1.time.tv_sec == uid2.time.tv_sec) &&
			(uid1.time.tv_usec == uid2.time.tv_usec) &&
			(uid1.ctr == uid2.ctr));
}


uuid_t UuidGetInvalidID(void)
{
	static const uuid_t invalid_uid = {0};
	
	return (invalid_uid);
}
