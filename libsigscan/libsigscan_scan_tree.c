/*
 * Scan tree functions
 *
 * Copyright (c) 2014, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libsigscan_byte_value_group.h"
#include "libsigscan_libcdata.h"
#include "libsigscan_libcerror.h"
#include "libsigscan_libcnotify.h"
#include "libsigscan_offset_group.h"
#include "libsigscan_pattern_weights.h"
#include "libsigscan_scan_tree.h"
#include "libsigscan_scan_tree_node.h"
#include "libsigscan_signature_group.h"
#include "libsigscan_signature_table.h"

uint8_t libsigscan_common_byte_values[ 256 ] = {
/*                           \a \b \t \n \v \f \r      */
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*         !  "  #  $  %  &  '  (  )  *  +  ,  -  .  / */
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*      0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ? */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
/*      @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O */
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*      P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _ */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
/*      `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o */
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*      p  q  r  s  t  y  v  w  x  y  z  {  |  }  ~    */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

/* Creates scan tree
 * Make sure the value scan_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_initialize(
     libsigscan_scan_tree_t **scan_tree,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_initialize";

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( *scan_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid scan tree value already set.",
		 function );

		return( -1 );
	}
	*scan_tree = memory_allocate_structure(
	              libsigscan_scan_tree_t );

	if( *scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create scan tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *scan_tree,
	     0,
	     sizeof( libsigscan_scan_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear scan tree.",
		 function );

		memory_free(
		 *scan_tree );

		*scan_tree = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *scan_tree != NULL )
	{
		memory_free(
		 *scan_tree );

		*scan_tree = NULL;
	}
	return( -1 );
}

/* Frees scan tree
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_free(
     libsigscan_scan_tree_t **scan_tree,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_scan_tree_free";

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( *scan_tree != NULL )
	{
		memory_free(
		 *scan_tree );

		*scan_tree = NULL;
	}
	return( 1 );
}

/* Determines the (most significant) pattern offset based on the similarity weights
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_scan_tree_get_pattern_offset_by_similarity_weights(
     libsigscan_scan_tree_t *scan_tree,
     libsigscan_pattern_weights_t *similarity_weights,
     libsigscan_pattern_weights_t *occurrence_weights,
     libsigscan_pattern_weights_t *byte_value_weights,
     off64_t *pattern_offset,
     libcerror_error_t **error )
{
	libsigscan_offset_group_t *offset_group = NULL;
	libsigscan_weight_group_t *weight_group = NULL;
	static char *function                   = "libsigscan_scan_tree_get_pattern_offset_by_similarity_weights";
	off64_t similarity_offset               = 0;
	int byte_value_weight                   = 0;
	int largest_byte_value_weight           = 0;
	int largest_occurrence_weight           = 0;
	int largest_weight                      = 0;
	int number_of_offsets                   = 0;
	int occurrence_weight                   = 0;
	int offset_index                        = 0;
	int result                              = 0;

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( pattern_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pattern offset.",
		 function );

		return( -1 );
	}
	result = libsigscan_pattern_weights_get_largest_weight(
	          similarity_weights,
	          &largest_weight,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve largest similarity weight.",
		 function );

		return( -1 );
	}
/* TODO
	else if( result == 0 )
	{
		return( 0 );
	}
*/
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: largest similarity weight: %d\n",
		 function,
		 largest_weight );
	}
#endif
	if( largest_weight > 0 )
	{
		if( libsigscan_pattern_weights_get_offset_group(
		     similarity_weights,
		     largest_weight,
		     &offset_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
		if( libsigscan_offset_group_get_number_of_offsets(
		     offset_group,
		     &number_of_offsets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of offsets in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of offsets: %d\n",
		 function,
		 number_of_offsets );
	}
#endif
	if( number_of_offsets == 0 )
	{
		/* No similarity offset fall back on the occurence weights.
		 */
		result = libsigscan_scan_tree_get_pattern_offset_by_occurrence_weights(
		          scan_tree,
		          occurrence_weights,
		          byte_value_weights,
		          pattern_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve pattern offset based on occurrence weights.",
			 function );

			return( -1 );
		}
	}
	else if( number_of_offsets == 1 )
	{
		result = libsigscan_offset_group_get_offset_by_index(
		          offset_group,
		          0,
		          pattern_offset,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve pattern offset: 0 in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
	else
	{
		for( offset_index = 0;
		     offset_index < number_of_offsets;
		     offset_index++ )
		{
			if( libsigscan_offset_group_get_offset_by_index(
			     offset_group,
			     offset_index,
			     &similarity_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve pattern offset: %d in offsets group for weight: %d.",
				 function,
				 offset_index,
				 largest_weight );

				return( -1 );
			}
			if( libsigscan_pattern_weights_get_weight_group(
			     occurrence_weights,
			     similarity_offset,
			     &weight_group,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve weight group for offset: %" PRIi64 ".",
				 function,
				 similarity_offset );

				return( -1 );
			}
			if( libsigscan_weight_group_get_weight(
			     weight_group,
			     &occurrence_weight,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve weight of weight group for offset: %" PRIi64 ".",
				 function,
				 similarity_offset );

				return( -1 );
			}
			if( ( largest_occurrence_weight > 0 )
			 && ( occurrence_weight == largest_occurrence_weight ) )
			{
				if( libsigscan_pattern_weights_get_weight_group(
				     byte_value_weights,
				     similarity_offset,
				     &weight_group,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve weight group for offset: %" PRIi64 ".",
					 function,
					 similarity_offset );

					return( -1 );
				}
				if( libsigscan_weight_group_get_weight(
				     weight_group,
				     &byte_value_weight,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve weight of weight group for offset: %" PRIi64 ".",
					 function,
					 similarity_offset );

					return( -1 );
				}
				if( byte_value_weight > largest_byte_value_weight )
				{
					largest_occurrence_weight = 0;
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				byte_value_weight = 0;
			}
#endif
			if( ( offset_index == 0 )
			 || ( occurrence_weight > largest_occurrence_weight ) )
			{
				largest_occurrence_weight = occurrence_weight;
				*pattern_offset           = similarity_offset;

				if( libsigscan_pattern_weights_get_weight_group(
				     byte_value_weights,
				     similarity_offset,
				     &weight_group,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve weight group for offset: %" PRIi64 ".",
					 function,
					 similarity_offset );

					return( -1 );
				}
				if( libsigscan_weight_group_get_weight(
				     weight_group,
				     &largest_byte_value_weight,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve weight of weight group for offset: %" PRIi64 ".",
					 function,
					 similarity_offset );

					return( -1 );
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: similarity offset: %" PRIi64 " occurrence weight: %d, byte value weight: %d (largest occurrence weight: %d, largest byte value weight: %d)\n",
				 function,
				 similarity_offset,
				 occurrence_weight,
				 byte_value_weight,
				 largest_occurrence_weight,
				 largest_byte_value_weight );
			}
#endif
		}
		result = 1;
	}
	return( result );
}

/* Determines the (most significant) pattern offset based on the occurrence weights
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_scan_tree_get_pattern_offset_by_occurrence_weights(
     libsigscan_scan_tree_t *scan_tree,
     libsigscan_pattern_weights_t *occurrence_weights,
     libsigscan_pattern_weights_t *byte_value_weights,
     off64_t *pattern_offset,
     libcerror_error_t **error )
{
	libsigscan_offset_group_t *offset_group = NULL;
	libsigscan_weight_group_t *weight_group = NULL;
	static char *function                   = "libsigscan_scan_tree_get_pattern_offset_by_occurrence_weights";
	off64_t occurrence_offset               = 0;
	int byte_value_weight                   = 0;
	int largest_byte_value_weight           = 0;
	int largest_weight                      = 0;
	int number_of_offsets                   = 0;
	int offset_index                        = 0;
	int result                              = 0;

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( pattern_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pattern offset.",
		 function );

		return( -1 );
	}
	result = libsigscan_pattern_weights_get_largest_weight(
	          occurrence_weights,
	          &largest_weight,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve largest occurrence weight.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: largest occurrence weight: %d\n",
		 function,
		 largest_weight );
	}
#endif
	if( largest_weight > 0 )
	{
		if( libsigscan_pattern_weights_get_offset_group(
		     occurrence_weights,
		     largest_weight,
		     &offset_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
		if( libsigscan_offset_group_get_number_of_offsets(
		     offset_group,
		     &number_of_offsets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of offsets in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of offsets: %d\n",
		 function,
		 number_of_offsets );
	}
#endif
	if( number_of_offsets == 0 )
	{
		/* No occurrnece offset fall back on the byte value weights.
		 */
		result = libsigscan_scan_tree_get_pattern_offset_by_byte_value_weights(
		          scan_tree,
		          byte_value_weights,
		          pattern_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve pattern offset based on byte value weights.",
			 function );

			return( -1 );
		}
	}
	else if( number_of_offsets == 1 )
	{
		if( libsigscan_offset_group_get_offset_by_index(
		     offset_group,
		     0,
		     pattern_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve pattern offset: 0 in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
	else
	{
		for( offset_index = 0;
		     offset_index < number_of_offsets;
		     offset_index++ )
		{
			if( libsigscan_offset_group_get_offset_by_index(
			     offset_group,
			     offset_index,
			     &occurrence_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve pattern offset: %d in offsets group for weight: %d.",
				 function,
				 offset_index,
				 largest_weight );

				return( -1 );
			}
			if( libsigscan_pattern_weights_get_weight_group(
			     byte_value_weights,
			     occurrence_offset,
			     &weight_group,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve weight group for offset: %" PRIi64 ".",
				 function,
				 occurrence_offset );

				return( -1 );
			}
			if( libsigscan_weight_group_get_weight(
			     weight_group,
			     &byte_value_weight,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve weight of weight group for offset: %" PRIi64 ".",
				 function,
				 occurrence_offset );

				return( -1 );
			}
			if( ( offset_index == 0 )
			 || ( byte_value_weight > largest_byte_value_weight ) )
			{
				largest_byte_value_weight = byte_value_weight;
				*pattern_offset           = occurrence_offset;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: occurrence offset: %" PRIi64 " byte value weight: %d (largest byte value weight: %d)\n",
				 function,
				 occurrence_offset,
				 byte_value_weight,
				 largest_byte_value_weight );
			}
#endif
		}
	}
	return( result );
}

/* Determines the (most significant) pattern offset based on the byte value weights
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_scan_tree_get_pattern_offset_by_byte_value_weights(
     libsigscan_scan_tree_t *scan_tree,
     libsigscan_pattern_weights_t *byte_value_weights,
     off64_t *pattern_offset,
     libcerror_error_t **error )
{
	libsigscan_offset_group_t *offset_group = NULL;
	static char *function                   = "libsigscan_scan_tree_get_pattern_offset_by_byte_value_weights";
	int largest_weight                      = 0;
	int number_of_offsets                   = 0;
	int result                              = 0;

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( pattern_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pattern offset.",
		 function );

		return( -1 );
	}
	result = libsigscan_pattern_weights_get_largest_weight(
	          byte_value_weights,
	          &largest_weight,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve largest byte value weight.",
		 function );

		return( -1 );
	}
/* TODO
	else if( result == 0 )
	{
		return( 0 );
	}
*/
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: largest byte value weight: %d\n",
		 function,
		 largest_weight );
	}
#endif
	if( largest_weight > 0 )
	{
		if( libsigscan_pattern_weights_get_offset_group(
		     byte_value_weights,
		     largest_weight,
		     &offset_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
		if( libsigscan_offset_group_get_number_of_offsets(
		     offset_group,
		     &number_of_offsets,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of offsets in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of offsets: %d\n",
		 function,
		 number_of_offsets );
	}
#endif
	if( number_of_offsets > 0 )
	{
		result = libsigscan_offset_group_get_offset_by_index(
		          offset_group,
		          0,
		          pattern_offset,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve pattern offset: 0 in offsets group for weight: %d.",
			 function,
			 largest_weight );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: no byte value offsets found.\n",
		 function );
	}
#endif
	return( result );
}

/* Determines the most significant pattern offset
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_scan_tree_get_most_significant_pattern_offset(
     libsigscan_scan_tree_t *scan_tree,
     libcdata_array_t *signatures_array,
     libsigscan_pattern_weights_t *similarity_weights,
     libsigscan_pattern_weights_t *occurrence_weights,
     libsigscan_pattern_weights_t *byte_value_weights,
     off64_t *pattern_offset,
     libcerror_error_t **error )
{
	static char *function    = "libsigscan_scan_tree_get_most_significant_pattern_offset";
	int number_of_signatures = 0;
	int result               = 0;

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     signatures_array,
	     &number_of_signatures,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of signatures.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of signatures: %d\n",
		 function,
		 number_of_signatures );
	}
#endif
	switch( number_of_signatures )
	{
		case 0:
			break;

		case 1:
			result = libsigscan_scan_tree_get_pattern_offset_by_byte_value_weights(
			          scan_tree,
			          byte_value_weights,
			          pattern_offset,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve most significant pattern offset based on byte value weights.",
				 function );

				return( -1 );
			}
			break;

		case 2:
			result = libsigscan_scan_tree_get_pattern_offset_by_occurrence_weights(
			          scan_tree,
			          occurrence_weights,
			          byte_value_weights,
			          pattern_offset,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve most significant pattern offset based on occurrence weights.",
				 function );

				return( -1 );
			}
			break;

		default:
			result = libsigscan_scan_tree_get_pattern_offset_by_similarity_weights(
			          scan_tree,
			          similarity_weights,
			          occurrence_weights,
			          byte_value_weights,
			          pattern_offset,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve most significant pattern offset based on similarity weights.",
				 function );

				return( -1 );
			}
			break;
	}
	return( result );
}

/* Builds a scan tree node
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_build_node(
     libsigscan_scan_tree_t *scan_tree,
     libcdata_array_t *signatures_array,
     libsigscan_signature_table_t *signature_table,
     libsigscan_scan_tree_node_t **node,
     libcerror_error_t **error )
{
	libsigscan_byte_value_group_t *byte_value_group  = NULL;
	libsigscan_pattern_weights_t *byte_value_weights = NULL;
	libsigscan_pattern_weights_t *occurrence_weights = NULL;
	libsigscan_pattern_weights_t *similarity_weights = NULL;
	libsigscan_scan_object_t *scan_object            = NULL;
	libsigscan_scan_tree_node_t *scan_tree_node      = NULL;
	libsigscan_signature_group_t *signature_group    = NULL;
	static char *function                            = "libsigscan_scan_tree_build_node";
	off64_t pattern_offset                           = -1;
	uint8_t byte_value                               = 0;
	int byte_value_group_index                       = 0;
	int signature_group_index                        = 0;
	int number_of_byte_value_groups                  = 0;
	int number_of_signature_groups                   = 0;
	int number_of_signatures                         = 0;

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( libsigscan_pattern_weights_initialize(
	     &occurrence_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create occurrence weights.",
		 function );

		goto on_error;
	}
	if( libsigscan_pattern_weights_initialize(
	     &similarity_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create similarity weights.",
		 function );

		goto on_error;
	}
	if( libsigscan_pattern_weights_initialize(
	     &byte_value_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create byte value weights.",
		 function );

		goto on_error;
	}
	if( libsigscan_signature_table_get_number_of_byte_value_groups(
	     signature_table,
	     &number_of_byte_value_groups,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of byte value groups.",
		 function );

		goto on_error;
	}
	for( byte_value_group_index = 0;
	     byte_value_group_index < number_of_byte_value_groups;
	     byte_value_group_index++ )
	{
		if( libsigscan_signature_table_get_byte_value_group_by_index(
		     signature_table,
		     byte_value_group_index,
		     &byte_value_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve byte value group: %d.",
			 function,
			 byte_value_group_index );

			goto on_error;
		}
		if( byte_value_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing byte value group: %d.",
			 function,
			 byte_value_group_index );

			goto on_error;
		}
		if( libsigscan_byte_value_group_get_number_of_signature_groups(
		     byte_value_group,
		     &number_of_signature_groups,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: invalid byte value: %d - unable to retrieve number of signature groups.",
			 function,
			 byte_value_group_index );

			goto on_error;
		}
		if( number_of_signature_groups > 1 )
		{
			if( libsigscan_pattern_weights_set_weight(
			     occurrence_weights,
			     byte_value_group->pattern_offset,
			     number_of_signature_groups,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set occurrence weight.",
				 function );

				goto on_error;
			}
		}
		for( signature_group_index = 0;
		     signature_group_index < number_of_signature_groups;
		     signature_group_index++ )
		{
			if( libsigscan_byte_value_group_get_signature_group_by_index(
			     byte_value_group,
			     signature_group_index,
			     &signature_group,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: invalid byte value group: %d - unable to retrieve signature group: %d.",
				 function,
				 byte_value_group_index,
				 signature_group_index );

				goto on_error;
			}
			if( signature_group == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid byte value group: %d - missing signature group: %d.",
				 function,
				 byte_value_group_index,
				 signature_group_index );

				goto on_error;
			}
			if( libsigscan_signature_group_get_number_of_signatures(
			     signature_group,
			     &number_of_signatures,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: invalid byte value group: %d - invalid signature group: %d - unable to retrieve number of signatures.",
				 function,
				 byte_value_group_index,
				 signature_group_index );

				goto on_error;
			}
			if( number_of_signatures > 1 )
			{
				if( libsigscan_pattern_weights_add_weight(
				     similarity_weights,
				     byte_value_group->pattern_offset,
				     number_of_signatures,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to add similarity weight.",
					 function );

					goto on_error;
				}
			}
			if( libsigscan_common_byte_values[ byte_value ] == 0 )
			{
				if( libsigscan_pattern_weights_add_weight(
				     byte_value_weights,
				     byte_value_group->pattern_offset,
				     1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to add byte value weight.",
					 function );

					goto on_error;
				}
			}
		}
	}
	if( libsigscan_scan_tree_get_most_significant_pattern_offset(
	     scan_tree,
	     signatures_array,
	     similarity_weights,
	     occurrence_weights,
	     byte_value_weights,
	     &pattern_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve most significant pattern offset.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: most significant pattern offset: %" PRIi64 "\n",
		 function,
		 pattern_offset );
	}
#endif
	if( libsigscan_pattern_weights_free(
	     &byte_value_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value weights.",
		 function );

		goto on_error;
	}
	if( libsigscan_pattern_weights_free(
	     &similarity_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free similarity weights.",
		 function );

		goto on_error;
	}
	if( libsigscan_pattern_weights_free(
	     &occurrence_weights,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free occurrence weights.",
		 function );

		goto on_error;
	}
/* TODO: ignore list add pattern offset */

	if( libsigscan_scan_tree_node_initialize(
	     &scan_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create scan tree node.",
		 function );

		goto on_error;
	}
#ifdef TODO
/* TODO:
 * create scan tree node
 * retrieve the byte values for the pattern offset from the signature table
 */

	for( byte_value_index = 0;
	     byte_value_index < number_of_byte_values;
	     byte_value_index++ )
	{
/* TODO: get number of signatures for the byte value */

		if( number_of_signatures == 0 )
		{
/* TODO: return error */
		}
		if( number_of_signatures == 1 )
		{
/* TODO: create signature scan object */
		}
		else
		{
/* TODO: create new pattern table */
/* TODO: create scan tree sub node */
/* TODO: create scan tree node scan object */
		}
/* TODO: add scan object */
		if( libsigscan_scan_tree_node_set_byte_value(
		     scan_tree_node,
		     0,
		     scan_object,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set occurrence weight.",
			 function );

			goto on_error;
		}
	}
/* TODO
 * determine remaining patterns
 */
	if( number_of_remaining_patterns == 1 )
	{
/* TODO set default pattern */
	}
	else if( number_of_remaining_patterns > 1 )
	{
/* TODO build a default scan tree sub node */
	}
#endif

/* TODO: add scan tree node to node */

	return( 1 );

on_error:
	if( scan_tree_node != NULL )
	{
		libsigscan_scan_tree_node_free(
		 &scan_tree_node,
		 NULL );
	}
	if( byte_value_weights != NULL )
	{
		libsigscan_pattern_weights_free(
		 &byte_value_weights,
		 NULL );
	}
	if( similarity_weights != NULL )
	{
		libsigscan_pattern_weights_free(
		 &similarity_weights,
		 NULL );
	}
	if( occurrence_weights != NULL )
	{
		libsigscan_pattern_weights_free(
		 &occurrence_weights,
		 NULL );
	}
	return( -1 );
}

/* Builds the scan tree
 * Returns 1 if successful or -1 on error
 */
int libsigscan_scan_tree_build(
     libsigscan_scan_tree_t *scan_tree,
     libcdata_array_t *signatures_array,
     libcerror_error_t **error )
{
	libsigscan_signature_table_t *signature_table = NULL;
	static char *function                         = "libsigscan_scan_tree_build";

	if( scan_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid scan tree.",
		 function );

		return( -1 );
	}
	if( libsigscan_signature_table_initialize(
	     &signature_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create signature table.",
		 function );

		goto on_error;
	}
	if( libsigscan_signature_table_fill(
	     signature_table,
	     signatures_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to fill signature table.",
		 function );

		goto on_error;
	}
	if( libsigscan_scan_tree_build_node(
	     scan_tree,
	     signatures_array,
	     signature_table,
	     &( scan_tree->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to build scan tree root node.",
		 function );

		goto on_error;
	}
/* TODO create skip table */
/* TODO determine largest pattern size */

	if( libsigscan_signature_table_free(
	     &signature_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free signature table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( signature_table != NULL )
	{
		libsigscan_signature_table_free(
		 &signature_table,
		 NULL );
	}
	return( -1 );
}

