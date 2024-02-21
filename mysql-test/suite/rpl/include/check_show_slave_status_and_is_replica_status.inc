# All fields for show slave status and IS.replica_status

let $sss_field= query_get_value(SHOW SLAVE STATUS, Master_Host, 1);
let $is_field= `select master_host from information_schema.replica_status`;
if (`SELECT strcmp("$is_field", "$sss_field") != 0`)
{
    die SHOW SLAVE STATUS Master_Host value ($sss_field) does not match value of IS.replica_status ($is_field);
}
