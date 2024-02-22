# Check all fields for show slave status and IS.replica_status
# Parameters
# $is_replica_status_fields - comma separated fields
# $num_skip - field in the list  that has NULL value from IS.replica_status

let $num_rows= 1;
let $num= 1;
let $handle_null= 0;
let $show_query= SHOW ALL SLAVES STATUS;

while ($is_replica_status_fields)
{
  let $sss_field= `SELECT SUBSTRING_INDEX('$is_replica_status_fields', ',', 1)`;
  let $is_replica_status_fields= `SELECT LTRIM(SUBSTRING('$is_replica_status_fields', LENGTH('$sss_field') + 2))`;

  --echo # Check $sss_field
  let $sss_field_value= query_get_value($show_query, $sss_field, $num_rows);
  let $is_field_value= `select $sss_field from information_schema.replica_status`;
  if (`SELECT strcmp("$sss_field_value", "$is_field_value") != 0`)
  {
    # Handle NULL of SQL_Remaining_Delay
    --echo # Ovo je $num, a ovo je $num_skip, a ovo je $num==$num_skip
    if ($num == $num_skip)
    {
      --echo # usao sam ovdje hejjjj
      # `mtr` interprets NULL as "", but returns valid for query_get_value()
      --echo $(`SELECT strcmp("$sss_field_value", 1)`)
      #       die("Empty variable"); mtr cannot handle ^
      if (`SELECT strcmp("$sss_field_value", 1) == NULL`) # < this is not possible
      {
        inc $handle_null;
        --echo # usao sam i ovdje hejjjj
          --echo # ovo json_on_empty_response
          SELECT $is_field_value;
        if (`SELECT strcmp("$is_field_value", "") != 0`)
        {
          die SHOW SLAVE STATUS $sss_field value ($sss_field_value) does not match value of IS.replica_status ($is_field_value);
        }
      }
    }
    if ($handle_null == 0)
    {
       die AfdA $handle_null SHOW SLAVE STATUS $sss_field value ($sss_field_value) does not match value of IS.replica_status ($is_field_value);
    }
    let $handle_null= 0;
  }
  inc $num;
}