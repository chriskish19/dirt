/**********************************************************/
//
// File: defines.hpp
//
// Purpose: define macros used throughout dirt.core
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

// Testing values
// no acutal copying takes place, saves my ssd when testing
#define DISABLE_COPY 1
#define DISABLE_DELETE 1
#define TEST_ENTRY 1
#define TEST_TIME_WAIT 30						// wait x seconds before flooding with work


// monitoring values
#define BUFFER_SIZE 1024 * 1024 * 4				// 4 MB Buffer
#define BUFFER_TIME 10							// 10 seconds
#define MAX_THREADS 4							// 4 threads
#define MAX_QUEUE_SPLIT 1000					// 1000 files per queue


// Logger values
#define LOGGER_VERBOSE 0

