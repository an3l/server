/*
 * Copyright (c) 2000, 2007 MySQL AB
 * SPDX-FileCopyrightText: 2025 MariaDB Foundation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
  echo is a replacement for the "echo" command builtin to cmd.exe
  on Windows, to get a Unix equivalent behaviour when running commands
  like:
    $> echo "hello" | mysql

  The windows "echo" would have sent "hello" to mysql while
  Unix echo will send hello without the enclosing hyphens

  This is a very advanced high tech program so take care when
  you change it and remember to valgrind it before production
  use.

*/

#include <stdio.h>

int main(int argc, char **argv)
{
  int i;
  for (i= 1; i < argc; i++)
  {
    fprintf(stdout, "%s", argv[i]);
    if (i < argc - 1)
      fprintf(stdout, " ");
  }
  fprintf(stdout, "\n");
  return 0;
}
