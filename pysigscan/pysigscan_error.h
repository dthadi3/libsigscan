/*
 * Error functions
 *
 * Copyright (C) 2014-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _PYSIGSCAN_ERROR_H )
#define _PYSIGSCAN_ERROR_H

#include <common.h>
#include <types.h>

#include "pysigscan_libcerror.h"
#include "pysigscan_python.h"

#define PYSIGSCAN_ERROR_STRING_SIZE	2048

#if defined( __cplusplus )
extern "C" {
#endif

void pysigscan_error_fetch(
      libcerror_error_t **error,
      int error_domain,
      int error_code,
      const char *format_string,
      ... );

void pysigscan_error_fetch_and_raise(
      PyObject *exception_object,
      const char *format_string,
      ... );

void pysigscan_error_raise(
      libcerror_error_t *error,
      PyObject *exception_object,
      const char *format_string,
      ... );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYSIGSCAN_ERROR_H ) */

