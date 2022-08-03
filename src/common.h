#ifndef COMMON_H_F90FCDA30876403FA059C0361EF19FC3
#define COMMON_H_F90FCDA30876403FA059C0361EF19FC3

#define _GNU_SOURCE

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/unixsupport.h>
#include <caml/fail.h>

#include <errno.h>

#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
# if __MAC_OS_X_VERSION_MAX_ALLOWED >= 120000
#  define USE_POSIX_SPAWN
# endif
#endif

#endif // COMMON_H
