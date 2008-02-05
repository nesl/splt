
#ifndef __SP_MUTEX_H__
#define __SP_MUTEX_H__

typedef struct sp_mutex_t {
	uint8_t    nouse;
} sp_mutex_t;

typedef struct sp_mutexattr_t {
	uint8_t    nouse;
} sp_mutexattr_t;

/**
 * sp_mutex_init creates new mutex and optional initialize with attr (if not NULL)
 */
sp_ret_t  sp_mutex_init( sp_mutex_t *mutex, const sp_mutexattr_t *attr );




#endif
