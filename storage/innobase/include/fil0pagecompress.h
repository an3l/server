/*****************************************************************************

Copyright (C) 2013, 2018 MariaDB Corporation. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

*****************************************************************************/

#ifndef fil0pagecompress_h
#define fil0pagecompress_h

#ifndef UNIV_INNOCHECKSUM

#include "fsp0fsp.h"
#include "fsp0pagecompress.h"

/******************************************************************//**
@file include/fil0pagecompress.h
Helper functions for extracting/storing page compression and
atomic writes information to table space.

Created 11/12/2013 Jan Lindström jan.lindstrom@skysql.com
***********************************************************************/

/****************************************************************//**
For page compressed pages compress the page before actual write
operation.
@return compressed page to be written*/
UNIV_INTERN
byte*
fil_compress_page(
/*==============*/
	fil_space_t*	space,	/*!< in,out: tablespace (NULL during IMPORT) */
	byte*	buf,		/*!< in: buffer from which to write; in aio
				this must be appropriately aligned */
	byte*	out_buf,	/*!< out: compressed buffer */
	ulint	len,		/*!< in: length of input buffer.*/
	ulint	level,		/* in: compression level */
	ulint	block_size,	/*!< in: block size */
	bool	encrypted,	/*!< in: is page also encrypted */
	ulint*	out_len);	/*!< out: actual length of compressed
				page */

/**
For page compressed pages decompress the page after actual read
operation.
@param[in]	page_buf	Preallocated temporal buffer where
				compression is done and then copied
				to the buffer.
@param[in,out]	buf		Compressed page and after suggesful
				decompresses operation uncompressed page.
@param[in]	len		Length of output buffer.
@param[out]	write_size	Actual payload size of the compressed data.
@return true when operation succeeded or false when failed */
UNIV_INTERN
bool
fil_decompress_page(
	byte*	page_buf,
	byte*	buf,
	ulong	len,
	ulint*	write_size)
	MY_ATTRIBUTE((warn_unused_result));

#endif /* !UNIV_INNOCHECKSUM */

/**
Verify that stored post compression checksum matches calculated
checksum. Note that old format did not have a checksum and
in that case either original pre-compression page checksum will
fail after decompression or page decompression fails.

@param[in,out]	page		page frame
@param[in]	space_id	Tablespace identifier
@param[in]	offset		Page offset
@return true if post compression checksum matches, false otherwise */
UNIV_INTERN
bool
fil_verify_compression_checksum(
	const byte*		page,
	ulint			space_id,
	ulint			offset)
	MY_ATTRIBUTE((warn_unused_result));

#endif /* fil0pagecompress_h */
