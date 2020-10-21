// Semaphore.h: interface for the CSemaphore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMAPHORE_H__38088D9E_C3F8_4651_B5AB_EA05982B3941__INCLUDED_)
#define AFX_SEMAPHORE_H__38088D9E_C3F8_4651_B5AB_EA05982B3941__INCLUDED_

#ifdef WIN32

#include <windows.h>
typedef HANDLE SEMA_HANDLE;

#else

#include <semaphore.h>
typedef sem_t SEMA_HANDLE;

#endif

class CSemaphore
{
public:
	CSemaphore();
	~CSemaphore();

	void lock(void);
	void unlock(void);

private:

	SEMA_HANDLE m_sema;
};

class CAutoLock
{
public:
	CAutoLock( CSemaphore& s ) : m_sema(s) { m_sema.lock(); }
	~CAutoLock() { m_sema.unlock(); }

private:
	CSemaphore& m_sema;

	//-- Disable some unsafe methods which are generated automatically by compiler
	CAutoLock();
	CAutoLock(const CAutoLock&);
	CAutoLock& operator=(const CAutoLock&);
	CAutoLock* operator&();
	const CAutoLock* operator&() const;
};

#endif // !defined(AFX_SEMAPHORE_H__38088D9E_C3F8_4651_B5AB_EA05982B3941__INCLUDED_)
