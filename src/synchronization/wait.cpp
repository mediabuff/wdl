// wait.cpp
//
// Free Function Implementations:
//	- wdl::synchronization::wait_one()
//	- wdl::synchronization::wait_all()
//	- wdl::synchronization::wait_array()
//
// Synchronization utilties.

#include <wdl/synchronization/wait.hpp>

namespace wdl::synchronization
{
	// wdl::synchronization::wait_one()
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_one(
		HANDLE const handle, 
		unsigned long const timeout
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(handle, timeout)
		);
	}

    // wdl::synchronization::wait_handles()
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_handles(
		HANDLE const handles[], 
		unsigned const size)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForMultipleObjects(size, handles, true, INFINITE)
		);
	}
}