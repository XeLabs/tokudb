/* Copyright (C) 2012 Monty Program Ab

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef MYSQL_SERVICE_LOGGER_INCLUDED
#define MYSQL_SERVICE_LOGGER_INCLUDED

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#endif

/**
  @file
  logger service

  Log file with rotation implementation.

  This service implements logging with possible rotation
  of the log files. Interface intentionally tries to be similar to FILE*
  related functions.

  So that one can open the log with logger_open(), specifying
  the limit on the logfile size and the rotations number.

  Then it's possible to write messages to the log with
  logger_printf or logger_vprintf functions.

  As the size of the logfile grows over the specified limit,
  it is renamed to 'logfile.1'. The former 'logfile.1' becomes
  'logfile.2', etc. The file 'logfile.rotations' is removed.
  That's how the rotation works.

  The rotation can be forced with the logger_rotate() call.

  Finally the log should be closed with logger_close().

@notes:
  Implementation checks the size of the log file before it starts new
  printf into it. So the size of the file gets over the limit when it rotates.

  The access is secured with the mutex, so the log is threadsafe.
*/

#include <sys/types.h>
#include <my_dir.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct logger_handle_st LOGGER_HANDLE;

void logger_init_mutexes();
LOGGER_HANDLE *logger_open(const char *path,
                           unsigned long long size_limit,
                           unsigned int rotations,
                           int thread_safe,
                           MY_STAT *stat);
int logger_close(LOGGER_HANDLE *log);
int logger_vprintf(LOGGER_HANDLE *log, const char *fmt, va_list argptr);
int logger_printf(LOGGER_HANDLE *log, const char *fmt, ...);
int logger_write(LOGGER_HANDLE *log, const char *buffer, size_t size);
int logger_rotate(LOGGER_HANDLE *log); 
int logger_sync(LOGGER_HANDLE *log);
int logger_reopen(LOGGER_HANDLE *log, MY_STAT *stat);
void logger_set_size_limit(LOGGER_HANDLE *log, unsigned long long size_limit);
void logger_set_rotations(LOGGER_HANDLE *log, unsigned int rotations);

#ifdef __cplusplus
}
#endif

#endif /*MYSQL_SERVICE_LOGGER_INCLUDED*/
