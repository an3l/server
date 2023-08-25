# ==== Purpose ====
#
# Waits until both the IO and SQL threads of the current connection
# have stopped gracefully.
#
# Note: this script will fail if one of the threads stops with an
# error.  If you expect an error in one of the threads, use
# include/wait_for_slave_io_error.inc or
# include/wait_for_slave_sql_error.inc instead.
#
# This script also fails if a timeout is reached (default 300
# seconds).
#
#
# ==== Usage ====
#
# [--let $slave_timeout= NUMBER]
# [--let $rpl_debug= 1]
# --source include/wait_for_slave_to_stop.inc
#
# Parameters:
#   $slave_timeout
#     See include/wait_for_slave_param.inc
#
#   $rpl_debug
#     See include/rpl_init.inc


--let $include_filename= wait_for_slave_to_stop.inc
--source include/begin_include_file.inc

# Persist slave_error_param in case of stopping the slave
# see `include/stop_slave.inc`.
--let $old_slave_error_param= $slave_error_param
--source include/wait_for_slave_sql_to_stop.inc
--let $slave_error_param= $old_slave_error_param
--source include/wait_for_slave_io_to_stop.inc

--let $include_filename= wait_for_slave_to_stop.inc
--source include/end_include_file.inc
