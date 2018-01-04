--disable_warnings
set global innodb_file_format = `Barracuda`;
set global innodb_file_per_table = on;
--enable_warnings

call mtr.add_suppression("mysqld: Index for table .* is corrupt; try to repair it");
call mtr.add_suppression("InnoDB: Page \[page id: space=[0-9]+, page number= [0-9]+\] in file .* may be corrupted. .*");
call mtr.add_suppression("InnoDB: Database page corruption on disk or a failed file read of tablespace .*");
call mtr.add_suppression("InnoDB: We detected index corruption in an InnoDB type table. .*");

create database comp_test;
use comp_test;
create table innodb_normal (c1 int not null auto_increment primary key, b char(200)) engine=innodb;
create table innodb_page_compressed1 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=1;
create table innodb_page_compressed2 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=2;
create table innodb_page_compressed3 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=3;
create table innodb_page_compressed4 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=4;
create table innodb_page_compressed5 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=5;
create table innodb_page_compressed6 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=6;
create table innodb_page_compressed7 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=7;
create table innodb_page_compressed8 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=8;
create table innodb_page_compressed9 (c1 int not null auto_increment primary key, b char(200)) engine=innodb page_compressed=1 page_compression_level=9;

--disable_query_log
begin;
let $i = 2000;
while ($i)
{
    insert into innodb_normal(b) values(REPEAT('Aa',50));
    insert into innodb_normal(b) values(REPEAT('a',100));
    insert into innodb_normal(b) values(REPEAT('b',100));
    insert into innodb_normal(b) values(REPEAT('0',100));
    insert into innodb_normal(b) values(REPEAT('1',100));
    dec $i;
}

insert into innodb_page_compressed1 select * from innodb_normal;
insert into innodb_page_compressed2 select * from innodb_normal;
insert into innodb_page_compressed3 select * from innodb_normal;
insert into innodb_page_compressed4 select * from innodb_normal;
insert into innodb_page_compressed5 select * from innodb_normal;
insert into innodb_page_compressed6 select * from innodb_normal;
insert into innodb_page_compressed7 select * from innodb_normal;
insert into innodb_page_compressed8 select * from innodb_normal;
insert into innodb_page_compressed9 select * from innodb_normal;
commit;
--enable_query_log

select count(*) from innodb_page_compressed1;
select count(*) from innodb_page_compressed3;
select count(*) from innodb_page_compressed4;
select count(*) from innodb_page_compressed5;
select count(*) from innodb_page_compressed6;
select count(*) from innodb_page_compressed6;
select count(*) from innodb_page_compressed7;
select count(*) from innodb_page_compressed8;
select count(*) from innodb_page_compressed9;

#
# Wait until pages are really compressed
#
let $wait_condition= select variable_value > 0 from information_schema.global_status where variable_name = 'INNODB_NUM_PAGES_PAGE_COMPRESSED';
--source include/wait_condition.inc

--let $MYSQLD_DATADIR=`select @@datadir`

# shutdown before grep

--source include/shutdown_mysqld.inc

--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_normal.ibd
--let SEARCH_RANGE = 10000000
--let SEARCH_PATTERN=AaAaAaAa
--echo # innodb_normal expected FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed1.ibd
--echo # innodb_page_compressed1 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed2.ibd
--echo # innodb_page_compressed2 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed3.ibd
--echo # innodb_page_compressed3 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed4.ibd
--echo # innodb_page_compressed4 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed5.ibd
--echo # innodb_page_compressed5 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed6.ibd
--echo # innodb_page_compressed6 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed7.ibd
--echo # innodb_page_compressed7 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed8.ibd
--echo # innodb_page_compressed8 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc
--let t1_IBD = $MYSQLD_DATADIR/comp_test/innodb_page_compressed9.ibd
--echo # innodb_page_compressed9 page compressed expected NOT FOUND
-- let SEARCH_FILE=$t1_IBD
-- source include/search_pattern_in_file.inc

-- source include/start_mysqld.inc

use comp_test;
select count(*) from innodb_page_compressed1;
select count(*) from innodb_page_compressed3;
select count(*) from innodb_page_compressed4;
select count(*) from innodb_page_compressed5;
select count(*) from innodb_page_compressed6;
select count(*) from innodb_page_compressed6;
select count(*) from innodb_page_compressed7;
select count(*) from innodb_page_compressed8;
select count(*) from innodb_page_compressed9;

let $wait_condition= select variable_value > 0 from information_schema.global_status where variable_name = 'INNODB_NUM_PAGES_PAGE_DECOMPRESSED';
--source include/wait_condition.inc

let INNODB_PAGE_SIZE=`select @@innodb_page_size`;
let MYSQLD_DATADIR=`select @@datadir`;

--echo # Restart server with 30s timeout to avoid recovery
--let $shutdown_timeout= 30
--source include/shutdown_mysqld.inc

--echo # Corrupting tablespaces...
#
# Now we corrupt page compressed pages as follows:
# (1) compression method
# (2) payload size
# (3) data
# (4) checksum
#
# We corrupt page 4 as page 0 is not compressed and we do
# not want to mess up with IBUF (page 2) or INODE (page 3) pages.
#
perl;
open(FILE, "+<", "$ENV{MYSQLD_DATADIR}/comp_test/innodb_page_compressed1.ibd") or die "open";
binmode FILE;
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 26, SEEK_SET) or die "seek";
sysread(FILE, $_, 8) == 8 || die "unable to read1\n";
# Corrupt compression method by decreasing one and if 0, reset to 6 (snappy)
my($ignore, $cmethod)=unpack("NN", $_);
$cmethod--;
if ($cmethod == 0) {
  $cmethod = 6;
}
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 26, SEEK_SET) or die "seek";
substr($_, 4,4) = pack("N",$cmethod);
syswrite(FILE, $_, 8) == 8 || die "unable to write\n";
close FILE or die "close";

# (2) corrupt payload size by decreasing size by 50 and if 0 set it to 20
open(FILE, "+<", "$ENV{MYSQLD_DATADIR}/comp_test/innodb_page_compressed2.ibd") or die "open";
binmode FILE;
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 40, SEEK_SET) or die "seek";
sysread(FILE, $_, 2) == 2 || die "unable to read2\n";
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 40, SEEK_SET) or die "seek";
my($size)=unpack("n", $_);
$size -= 50;
if ($size <= 0) {
  $size = 20;
}
substr($_, 0,2) = pack("n",$size);
syswrite(FILE, $_, 2) == 2 || die "unable to write\n";
close FILE or die "close";

# (3) Corrupt data
open(FILE, "+<", "$ENV{MYSQLD_DATADIR}/comp_test/innodb_page_compressed3.ibd") or die "open";
binmode FILE;
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 42, SEEK_SET) or die "seek";
sysread(FILE, $_, 16) == 16 || die "unable to read3\n";
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4 + 42, SEEK_SET) or die "seek";
substr($_,0,16) = pack("NNNN",0xdeadbeef,0xdeadbeef,0xdeadbeef,0xdeadbeef);
syswrite(FILE, $_, 16) == 16 || die "unable to write\n";
close FILE or die "close";

# (4) Corrupt checksum
open(FILE, "+<", "$ENV{MYSQLD_DATADIR}/comp_test/innodb_page_compressed4.ibd") or die "open";
binmode FILE;
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4, SEEK_SET) or die "seek";
sysread(FILE, $_, 4) == 4 || die "unable to read4\n";
seek(FILE, $ENV{'INNODB_PAGE_SIZE'} * 4, SEEK_SET) or die "seek";
substr($_, 0,4) = pack("N",0xbeedead);
syswrite(FILE, $_, 4) == 4 || die "unable to write\n";
close FILE or die "close";
EOF

--echo # Corruption done

--echo # Start server again
--source include/start_mysqld.inc

use comp_test;

--error ER_NOT_KEYFILE
select * from innodb_page_compressed1;
--error ER_NOT_KEYFILE
select * from innodb_page_compressed2;
--error ER_NOT_KEYFILE
select * from innodb_page_compressed3;
--error ER_NOT_KEYFILE
select * from innodb_page_compressed4;

use test;
drop database comp_test;
