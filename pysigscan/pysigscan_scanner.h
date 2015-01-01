/*
 * Python object definition of the libsigscan scanner
 *
 * Copyright (C) 2014-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYSIGSCAN_SCANNER_H )
#define _PYSIGSCAN_SCANNER_H

#include <common.h>
#include <types.h>

#include "pysigscan_libbfio.h"
#include "pysigscan_libsigscan.h"
#include "pysigscan_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pysigscan_scanner pysigscan_scanner_t;

struct pysigscan_scanner
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libsigscan scanner
	 */
	libsigscan_scanner_t *scanner;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pysigscan_scanner_object_methods[];
extern PyTypeObject pysigscan_scanner_type_object;

PyObject *pysigscan_scanner_new(
           void );

PyObject *pysigscan_scanner_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysigscan_scanner_new_open_scanner_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pysigscan_scanner_init(
     pysigscan_scanner_t *pysigscan_scanner );

void pysigscan_scanner_free(
      pysigscan_scanner_t *pysigscan_scanner );

PyObject *pysigscan_scanner_signal_abort(
           pysigscan_scanner_t *pysigscan_scanner,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

