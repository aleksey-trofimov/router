#include "Semaphore.h"

CSemaphore::CSemaphore()
{

#ifdef WIN32
	m_sema = CreateSemaphore( NULL, 1, 1, NULL );
#else
	sem_init( &m_sema, 0, 1 );
#endif

}

CSemaphore::~CSemaphore()
{

#ifdef WIN32
	CloseHandle(m_sema);
#else
	sem_destroy( &m_sema );
#endif

}

void CSemaphore::lock(void)
{

#ifdef WIN32
	WaitForSingleObject( m_sema, INFINITE );
#else
	sem_wait( &m_sema );
#endif

}

void CSemaphore::unlock(void)
{

#ifdef WIN32
	ReleaseSemaphore( m_sema, 1, NULL );
#else
	sem_post( &m_sema );
#endif

}
