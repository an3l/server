#ifndef CLIENT_MY_READLINE_INCLUDED
#define CLIENT_MY_READLINE_INCLUDED

/*
 * Copyright (c) 2000, 2011, Oracle and/or its affiliates
 * SPDX-FileCopyrightText: 2025 MariaDB Foundation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* readline for batch mode */

typedef struct st_line_buffer
{
  File file;
  char *buffer;			/* The buffer itself, grown as needed. */
  char *end;			/* Pointer at buffer end */
  char *start_of_line,*end_of_line;
  uint bufread;			/* Number of bytes to get with each read(). */
  uint eof;
  ulong max_size;
  ulong read_length;		/* Length of last read string */
  int error;
  bool truncated;
} LINE_BUFFER;

extern LINE_BUFFER *batch_readline_init(ulong max_size,FILE *file);
extern LINE_BUFFER *batch_readline_command(LINE_BUFFER *buffer, char * str);
extern char *batch_readline(LINE_BUFFER *buffer, bool binary_mode);
extern void batch_readline_end(LINE_BUFFER *buffer);

#endif /* CLIENT_MY_READLINE_INCLUDED */
