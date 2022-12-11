--echo # ----- TEST 1: Check if valid wait point is set
eval set global rpl_semi_sync_master_wait_point= $my_wait_point;
select @@rpl_semi_sync_master_wait_point;

--echo # ----- TEST 2: No semi-sync and GTID on master
connection master;
SHOW REPLICA HOSTS;

--echo # ----- TEST 3: Yes semi-sync and no GTID on master
set global rpl_semi_sync_master_enabled = 1;
show variables like 'rpl_semi_sync_master_enabled';
show status like 'Rpl_semi_sync_master_status';
SHOW REPLICA HOSTS;
reset master;
SHOW REPLICA HOSTS;

--echo # ----- TEST 4: No semi-sync on replica - empty result

connection slave;
SHOW REPLICA HOSTS;

--echo # ----- TEST 5: Enable semi-sync on replica - empty result
set global rpl_semi_sync_slave_enabled = 1;
show variables like 'rpl_semi_sync_slave_enabled';
# No ACK yet 
SHOW REPLICA HOSTS;

# Verify there is connection on master
--connection master
show variables like 'rpl_semi_sync_master_enabled';
let $status_var= Rpl_semi_sync_master_clients;
let $status_var_value= 1;
source include/wait_for_status_var.inc;

connection slave;
# Here (on replica) should show same list like on primary (or? @buggy1)
SHOW REPLICA HOSTS;

--echo # ----- Test 6: Create table on primary when semi-sync is enabled
--connection master
CREATE TABLE t1 (a INT);
INSERT INTO t1 SET a=1;
--replace_column 3 'REPLICA_PORT'
--replace_result $SLAVE_MYPORT REPLICA_PORT $DEFAULT_MASTER_PORT DEFAULT_PORT
show status like 'Rpl_semi_sync_master_status';
show status like 'Rpl_semi_sync_master_no_tx';
show status like 'Rpl_semi_sync_master_yes_tx';
SHOW REPLICA HOSTS;
INSERT INTO t1 SET a=2;
--replace_column 3 'REPLICA_PORT'
--replace_result $SLAVE_MYPORT REPLICA_PORT $DEFAULT_MASTER_PORT DEFAULT_PORT
SHOW REPLICA HOSTS;

--echo # ----- Test 7: Test the result on primary when semi-sync is disabled
SET GLOBAL rpl_semi_sync_master_enabled = OFF;
DROP TABLE t1;
SHOW SLAVE HOSTS;

sync_slave_with_master;
show status like 'Rpl_semi_sync_slave_status';
