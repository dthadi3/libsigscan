/*
 * Scan tree node functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libsigscan_definitions.h"
#include "libsigscan_libcerror.h"
#include "libsigscan_libcnotify.h"
#include "libsigscan_scan_object.h"
#include "libsigscan_scan_tree_node.h"

/* Creates scan tree node
 * Make sure the value scan_tree_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_node_initialize(
     libsigscan_scan_tree_node_t **scan_tree_node,
     off64_t pattern_offset,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_initialize";

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( *scan_tree_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid scan tree node value already set.",
		 function );

		return( -1 );
	}
	*scan_tree_node = memory_allocate_structure(
	                   libsigscan_scan_tree_node_t );

	if( *scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create scan tree node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *scan_tree_node,
	     0,
	     sizeof( libsigscan_scan_tree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear scan tree node.",
		 function );

		memory_free(
		 *scan_tree_node );

		*scan_tree_node = NULL;

		return( -1 );
	}
	( *scan_tree_node )->pattern_offset = pattern_offset;

	return( 1 );

on_error:
	if( *scan_tree_node != NULL )
	{
		memory_free(
		 *scan_tree_node );

		*scan_tree_node = NULL;
	}
	return( -1 );
}

/* Frees scan tree node
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_node_free(
     libsigscan_scan_tree_node_t **scan_tree_node,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_free";
	uint16_t byte_value   = 0;
	int result            = 1;

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( *scan_tree_node != NULL )
	{
		for( byte_value = 0;
		     byte_value < 256;
		     byte_value++ )
		{
			if( ( *scan_tree_node )->scan_objects_table[ byte_value ] == NULL )
			{
				continue;
			}
			if( libsigscan_scan_object_free(
			     &( ( *scan_tree_node )->scan_objects_table[ byte_value ] ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free scan object for byte value: 0x%02" PRIx16 ".",
				 function,
				 byte_value );

				result = -1;
			}
		}
		if( libsigscan_scan_object_free(
		     &( ( *scan_tree_node )->default_scan_object ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free default scan object.",
			 function );

			result = -1;
		}
		memory_free(
		 *scan_tree_node );

		*scan_tree_node = NULL;
	}
	return( result );
}

/* Sets a scan object for a specific byte value
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_node_set_byte_value(
     libsigscan_scan_tree_node_t *scan_tree_node,
     uint8_t byte_value,
     libsigscan_scan_object_t *scan_object,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_set_byte_value";

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( scan_tree_node->scan_objects_table[ byte_value ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid scan tree node - scan object for byte value: 0x%02" PRIx8 " already set.",
		 function,
		 byte_value );

		return( -1 );
	}
	if( scan_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan object.",
		 function );

		return( -1 );
	}
	scan_tree_node->scan_objects_table[ byte_value ] = scan_object;

	return( 1 );
}

/* Sets the default scan object
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_node_set_default_value(
     libsigscan_scan_tree_node_t *scan_tree_node,
     libsigscan_scan_object_t *scan_object,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_set_default_value";

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( scan_tree_node->default_scan_object != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid scan tree node - default scan object already set.",
		 function );

		return( -1 );
	}
	if( scan_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan object.",
		 function );

		return( -1 );
	}
	scan_tree_node->default_scan_object = scan_object;

	return( 1 );
}

/* Retrieves the scan object for a specific byte value or the default if available
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libsigscan_scan_tree_node_get_scan_object(
     libsigscan_scan_tree_node_t *scan_tree_node,
     uint8_t byte_value,
     libsigscan_scan_object_t **scan_object,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_get_scan_object";

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( scan_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan object.",
		 function );

		return( -1 );
	}
	*scan_object = scan_tree_node->scan_objects_table[ byte_value ];

	if( *scan_object != NULL )
	{
		return( 1 );
	}
	*scan_object = scan_tree_node->default_scan_object;

	if( *scan_object != NULL )
	{
		return( 1 );
	}
	return( 0 );
}

/* Scans the buffer for a scan object that matches
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libsigscan_scan_tree_node_scan_buffer(
     libsigscan_scan_tree_node_t *scan_tree_node,
     const uint8_t *buffer,
     size_t buffer_size,
     size_t buffer_offset,
     libsigscan_scan_object_t **scan_object,
     libcerror_error_t **error )
{
	static char *function    = "libsigscan_scan_tree_node_scan_buffer";
	size_t scan_offset       = 0;
	uint8_t scan_object_type = 0;
	int result               = 0;

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( buffer_offset >= buffer_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid buffer offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( scan_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan object.",
		 function );

		return( -1 );
	}
	do
	{
		scan_offset = buffer_offset + scan_tree_node->pattern_offset;

		if( scan_offset >= buffer_size )
		{
/* TODO support boundary matches ? */
			*scan_object = NULL;

			return( 1 );
		}
		result = libsigscan_scan_tree_node_get_scan_object(
		          scan_tree_node,
		          buffer[ scan_offset ],
		          scan_object,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve scan object.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libsigscan_scan_object_get_type(
			     *scan_object,
			     &scan_object_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve scan object type.",
				 function );

				return( -1 );
			}
			if( scan_object_type == LIBSIGSCAN_SCAN_OBJECT_TYPE_SCAN_TREE_NODE )
			{
				if( libsigscan_scan_object_get_value(
				     *scan_object,
				     (intptr_t **) &scan_tree_node,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve scan object value.",
					 function );

					return( -1 );
				}
			}
			else if( scan_object_type == LIBSIGSCAN_SCAN_OBJECT_TYPE_SIGNATURE )
			{
				break;
			}
		}
	}
	while( result != 0 );

	return( result );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the scan tree node
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_node_printf(
     libsigscan_scan_tree_node_t *scan_tree_node,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_node_printf";
	uint16_t byte_value   = 0;

	if( scan_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree node.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "%s: scan tree node: 0x%08" PRIjx "\n",
	 function,
	 (intptr_t *) scan_tree_node );

	libcnotify_printf(
	 "%s: pattern offset: %" PRIi64 "\n",
	 function,
	 scan_tree_node->pattern_offset );

	for( byte_value = 0;
	     byte_value < 256;
	     byte_value++ )
	{
		if( scan_tree_node->scan_objects_table[ byte_value ] == NULL )
		{
			continue;
		}
		libcnotify_printf(
		 "%s: byte value: 0x%02" PRIx16 ": ",
		 function,
		 byte_value );

		if( libsigscan_scan_object_printf(
		     scan_tree_node->scan_objects_table[ byte_value ],
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print scan object for byte value: 0x%02" PRIx16 ".",
			 function,
			 byte_value );

			return( -1 );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( scan_tree_node->default_scan_object != NULL )
	{
		libcnotify_printf(
		 "%s: default: ",
		 function );

		if( libsigscan_scan_object_printf(
		     scan_tree_node->default_scan_object,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print default scan object.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "\n" );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

