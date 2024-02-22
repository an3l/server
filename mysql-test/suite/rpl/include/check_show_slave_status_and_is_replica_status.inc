# Check all fields for show slave status and IS.replica_status

--echo # Check Connection_name

let $is_replica_status_fields= Connection_name, Slave_SQL_State, Slave_IO_State;

while ($is_replica_status_fields)
{
    let $sss_field= `SELECT SUBSTRING_INDEX('$is_replica_status_fields', ',', 1)`;
    let $is_replica_status_fields= `SELECT LTRIM(SUBSTRING('$is_replica_status_fields', LENGTH('$sss_field') + 2))`;
    let $num_rows= 1;
    let $num_fields= 1;
    let $show_query= SHOW ALL SLAVES STATUS;

    --echo $sss_field
    let $sss_field_value= query_get_value($show_query, $sss_field, $num_rows);
    let $is_field_value= `select $sss_field from information_schema.replica_status`;

    if (`SELECT strcmp("$sss_field_value", "$is_field_value") != 0`)
    {
        die SHOW SLAVE STATUS Master_Host value ($sss_field_value) does not match value of IS.replica_status ($is_field_value);
    }
    --echo $sss_field_value = $is_field_value
    inc $num_fields;
}