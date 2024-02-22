# Check alll fields for show slave status and IS.replica_status

--echo # Check Connection_name

let $tmp= Connection_name;
let $sss_field= query_get_value(SHOW ALL SLAVES STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Connection_name value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_SQL_State

let $tmp= Slave_SQL_State;
let $sss_field= query_get_value(SHOW ALL SLAVES STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_SQL_State value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_IO_State

let $tmp= Slave_IO_State;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_IO_State value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_Host

let $tmp= Master_Host;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_Host value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_User

let $tmp= Master_User;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_User value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_Port

let $tmp= Master_Port;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_Port value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Connect_Retry

let $tmp= Connect_Retry;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Connect_Retry value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_Log_File

let $tmp= Master_Log_File;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_Log_File value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Read_Master_Log_Pos

let $tmp= Read_Master_Log_Pos;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Read_Master_Log_Pos value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Relay_Log_File

let $tmp= Relay_Log_File;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Relay_Log_File value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Relay_Log_Pos

let $tmp= Relay_Log_Pos;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Relay_Log_Pos value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Relay_Master_Log_File

let $tmp= Relay_Master_Log_File;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Relay_Master_Log_File value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Slave_IO_Running

let $tmp= Slave_IO_Running;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_IO_Running value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_SQL_Running

let $tmp= Slave_SQL_Running;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_SQL_Running value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Replicate_Rewrite_DB

let $tmp= Replicate_Rewrite_DB;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Rewrite_DB value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Replicate_Do_DB

let $tmp= Replicate_Do_DB;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Do_DB value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Replicate_Ignore_DB

let $tmp= Replicate_Ignore_DB;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Ignore_DB value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Replicate_Do_Table

let $tmp= Replicate_Do_Table;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Do_Table value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Replicate_Ignore_Table

let $tmp= Replicate_Ignore_Table;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Ignore_Table value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Replicate_Wild_Do_Table

let $tmp= Replicate_Wild_Do_Table;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Wild_Do_Table value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Replicate_Wild_Ignore_Table

let $tmp= Replicate_Wild_Ignore_Table;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Wild_Ignore_Table value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Last_Errno

let $tmp= Last_Errno;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_Errno value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Last_Error

let $tmp= Last_Error;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_Error value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Skip_Counter

let $tmp= Skip_Counter;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Skip_Counter value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Exec_Master_Log_Pos

let $tmp= Exec_Master_Log_Pos;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Exec_Master_Log_Pos value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Relay_Log_Space

let $tmp= Relay_Log_Space;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Relay_Log_Space value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Until_Condition

let $tmp= Until_Condition;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Until_Condition value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Until_Log_File

let $tmp= Until_Log_File;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Until_Log_File value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Until_Log_Pos

let $tmp= Until_Log_Pos;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Until_Log_Pos value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Allowed
let $tmp= Master_SSL_Allowed;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Allowed value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_CA_File

let $tmp= Master_SSL_CA_File;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_CA_File value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_CA_Path

let $tmp= Master_SSL_CA_Path;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_CA_Path value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Cert

let $tmp= Master_SSL_Cert;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Cert value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Cipher

let $tmp= Master_SSL_Cipher;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Cipher value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Key

let $tmp= Master_SSL_Key;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Key value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Seconds_Behind_Master

let $tmp= Seconds_Behind_Master;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Seconds_Behind_Master value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Verify_Server_Cert
# mysqltest: At line 17: query 'select Master_SSL_Verify_Server_Cert from information_schema.replica_status' failed: ER_PARSE_ERROR (1064): You have an error in your SQL syntax; check the manual that corresponds to your MariaDB server version for the right syntax to use near 'Master_SSL_Verify_Server_Cert from information_schema.replica_status' at line 1

#let $tmp= Master_SSL_Verify_Server_Cert;
#let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
#--echo Field $tmp
#let $is_field= `select $tmp from information_schema.replica_status`;
#if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
#{
#    die SHOW SLAVE STATUS Master_SSL_Verify_Server_Cert value ($sss_field) does not match value of IS.replica_status ($is_field);
#}


--echo # Check Last_IO_Errno

let $tmp= Last_IO_Errno;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_IO_Errno value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Last_IO_Error

let $tmp= Last_IO_Error;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_IO_Error value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Last_SQL_Errno

let $tmp= Last_SQL_Errno;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_SQL_Errno value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Last_SQL_Error

let $tmp= Last_SQL_Error;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Last_SQL_Error value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Replicate_Ignore_Server_Ids

let $tmp= Replicate_Ignore_Server_Ids;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Ignore_Server_Ids value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_Server_Id

let $tmp= Master_Server_Id;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_Server_Id value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Crl

let $tmp= Master_SSL_Crl;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Crl value ($sss_field) does not match value of IS.replica_status ($is_field);
}


--echo # Check Master_SSL_Crlpath

let $tmp= Master_SSL_Crlpath;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_SSL_Crlpath value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Using_Gtid

let $tmp= Using_Gtid;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Using_Gtid value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Gtid_IO_Pos

let $tmp= Gtid_IO_Pos;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Gtid_IO_Pos value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Replicate_Ignore_Domain_Ids

let $tmp= Replicate_Ignore_Domain_Ids;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Replicate_Ignore_Domain_Ids value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Parallel_Mode

let $tmp= Parallel_Mode;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Parallel_Mode value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check SQL_Delay

let $tmp= SQL_Delay;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS SQL_Delay value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check SQL_Remaining_Delay

let $tmp= SQL_Remaining_Delay;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
# Cannot compare NULL and ""
let $is_field= query_get_value(select * from information_schema.replica_status, $tmp, 1); 
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS SQL_Remaining_Delay value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_SQL_Running_State

let $tmp= Slave_SQL_Running_State;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_SQL_Running_State value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_DDL_Groups

let $tmp= Slave_DDL_Groups;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_DDL_Groups value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_Non_Transactional_Groups

let $tmp= Slave_Non_Transactional_Groups;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_Non_Transactional_Groups value ($sss_field) does not match value of IS.replica_status ($is_field);
}

--echo # Check Slave_Transactional_Groups

let $tmp= Slave_Transactional_Groups;
let $sss_field= query_get_value(SHOW SLAVE STATUS, $tmp, 1);
let $is_field= `select $tmp from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Slave_Transactional_Groups value ($sss_field) does not match value of IS.replica_status ($is_field);
}