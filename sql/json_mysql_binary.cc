/*
   Copyright (c) 2000, 2017, Oracle and/or its affiliates.
   Copyright (c) 2008, 2019, MariaDB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/
#include "json_mysql_binary.h"
#include <algorithm>            // std::min

#define JSONB_TYPE_SMALL_OBJECT   0x0
#define JSONB_TYPE_LARGE_OBJECT   0x1
#define JSONB_TYPE_SMALL_ARRAY    0x2
#define JSONB_TYPE_LARGE_ARRAY    0x3
#define JSONB_TYPE_LITERAL        0x4
#define JSONB_TYPE_INT16          0x5
#define JSONB_TYPE_UINT16         0x6
#define JSONB_TYPE_INT32          0x7
#define JSONB_TYPE_UINT32         0x8
#define JSONB_TYPE_INT64          0x9
#define JSONB_TYPE_UINT64         0xA
#define JSONB_TYPE_DOUBLE         0xB
#define JSONB_TYPE_STRING         0xC
#define JSONB_TYPE_OPAQUE         0xF

#define JSONB_NULL_LITERAL        '\x00'
#define JSONB_TRUE_LITERAL        '\x01'
#define JSONB_FALSE_LITERAL       '\x02'
/*
  The size of offset or size fields in the small and the large storage
  format for JSON objects and JSON arrays.
*/
#define SMALL_OFFSET_SIZE         2
#define LARGE_OFFSET_SIZE         4

/*
  The size of key entries for objects when using the small storage
  format or the large storage format. In the small format it is 4
  bytes (2 bytes for key length and 2 bytes for key offset). In the
  large format it is 6 (2 bytes for length, 4 bytes for offset).
*/
#define KEY_ENTRY_SIZE_SMALL      (2 + SMALL_OFFSET_SIZE)
#define KEY_ENTRY_SIZE_LARGE      (2 + LARGE_OFFSET_SIZE)
/*
  The size of value entries for objects or arrays. When using the
  small storage format, the entry size is 3 (1 byte for type, 2 bytes
  for offset). When using the large storage format, it is 5 (1 byte
  for type, 4 bytes for offset).
*/
#define VALUE_ENTRY_SIZE_SMALL    (1 + SMALL_OFFSET_SIZE)
#define VALUE_ENTRY_SIZE_LARGE    (1 + LARGE_OFFSET_SIZE)

namespace json_mysql_binary
{

// Constructor for literals and errors.
Value::Value(mysql_value_enum_type t)
  : m_type(t), m_data(), m_element_count(), m_length(),
    m_int_value(), m_double_value(), m_large()
{
  DBUG_ASSERT(t == LITERAL_NULL || t == LITERAL_TRUE || t == LITERAL_FALSE ||
              t == ERROR);
}


// Constructor for int and uint.
Value::Value(mysql_value_enum_type t, int64 val)
  : m_type(t), m_data(), m_element_count(), m_length(),
    m_int_value(val), m_double_value(), m_large()
{
  DBUG_ASSERT(t == INT || t == UINT);
}


// Constructor for double.
Value::Value(double d)
  : m_type(DOUBLE), m_data(), m_element_count(), m_length(),
    m_int_value(), m_double_value(d), m_large()
{}


// Constructor for string.
Value::Value(const char *data, size_t len)
  : m_type(STRING), m_data(data), m_element_count(),
    m_length(len), m_int_value(), m_double_value(), m_large()
{}


// Constructor for arrays and objects.
Value::Value(mysql_value_enum_type t, const char *data, size_t bytes,
             size_t element_count, bool large)
  : m_type(t), m_data(data), m_element_count(element_count),
    m_length(bytes), m_int_value(), m_double_value(), m_large(large)
{
  DBUG_ASSERT(t == ARRAY || t == OBJECT);
}

static Value err() { return Value(Value::ERROR); } // assert in explicit constructor to error

/**
  Parse a JSON scalar value.

  @param type   the binary type of the scalar
  @param data   pointer to the start of the binary representation of the scalar
  @param len    the maximum number of bytes to read from data
  @return  an object that represents the scalar value
*/

/**
  Read a variable length written by append_variable_length().

  @param[in] data  the buffer to read from
  @param[in] data_length  the maximum number of bytes to read from data
  @param[out] length  the length that was read
  @param[out] num  the number of bytes needed to represent the length
  @return  false on success, true on error
*/
static bool read_variable_length(const char *data, size_t data_length,
                                 size_t *length, size_t *num)
{
  /*
    It takes five bytes to represent UINT_MAX32, which is the largest
    supported length, so don't look any further.
  */
  const size_t max_bytes= std::min(data_length, static_cast<size_t>(5));

  size_t len= 0;
  for (size_t i= 0; i < max_bytes; i++)
  {
    // Get the next 7 bits of the length.
    len|= (data[i] & 0x7f) << (7 * i);
    if ((data[i] & 0x80) == 0)
    {
      // The length shouldn't exceed 32 bits.
      if (len > UINT_MAX32)
        return true;                          /* purecov: inspected */

      // This was the last byte. Return successfully.
      *num= i + 1;
      *length= len;
      return false;
    }
  }

  // No more available bytes. Return true to signal error.
  return true;                                /* purecov: inspected */
}

/**
  Parse a JSON scalar value.

  @param type   the binary type of the scalar
  @param data   pointer to the start of the binary representation of the scalar
  @param len    the maximum number of bytes to read from data
  @return  an object that represents the scalar value
*/
static Value parse_scalar(uint8 type, const char *data, size_t len)
{
  switch (type)
  {
  case JSONB_TYPE_LITERAL:
    if (len < 1)
      return err();                           /* purecov: inspected */
    switch (static_cast<uint8>(*data))
    {
    case JSONB_NULL_LITERAL:
      return Value(Value::LITERAL_NULL);
    case JSONB_TRUE_LITERAL:
      return Value(Value::LITERAL_TRUE);
    case JSONB_FALSE_LITERAL:
      return Value(Value::LITERAL_FALSE);
    default:
      return err();                           /* purecov: inspected */
    }
  case JSONB_TYPE_INT16:
    if (len < 2)
      return err();                           /* purecov: inspected */
    return Value(Value::INT, sint2korr(data));
  case JSONB_TYPE_INT32:
    if (len < 4)
      return err();                           /* purecov: inspected */
    return Value(Value::INT, sint4korr(data));
  case JSONB_TYPE_INT64:
    if (len < 8)
      return err();                           /* purecov: inspected */
    return Value(Value::INT, sint8korr(data));
  case JSONB_TYPE_UINT16:
    if (len < 2)
      return err();                           /* purecov: inspected */
    return Value(Value::UINT, uint2korr(data));
  case JSONB_TYPE_UINT32:
    if (len < 4)
      return err();                           /* purecov: inspected */
    return Value(Value::UINT, uint4korr(data));
  case JSONB_TYPE_UINT64:
    if (len < 8)
      return err();                           /* purecov: inspected */
    return Value(Value::UINT, uint8korr(data));
  case JSONB_TYPE_DOUBLE:
    {
      if (len < 8)
        return err();                         /* purecov: inspected */
      double d;
      float8get(d, data);
      return Value(d);
    }
  case JSONB_TYPE_STRING:
    {
      size_t str_len;
      size_t n;
      if (read_variable_length(data, len, &str_len, &n))
        return err();                         /* purecov: inspected */
      if (len < n + str_len)
        return err();                         /* purecov: inspected */
      return Value(data + n, str_len);
    }
  
  //case JSONB_TYPE_OPAQUE:
  //  {
      /*
        There should always be at least one byte, which tells the field
        type of the opaque value.
      */
  //    if (len < 1)
  //      return err();                         /* purecov: inspected */

      // The type is encoded as a uint8 that maps to an enum_field_types.
  //    uint8 type_byte= static_cast<uint8>(*data);
  //    enum_field_types field_type= static_cast<enum_field_types>(type_byte);

      // Then there's the length of the value.
  //    size_t val_len;
  //   size_t n;
  //    if (read_variable_length(data + 1, len - 1, &val_len, &n))
  //      return err();                         /* purecov: inspected */
  //    if (len < 1 + n + val_len)
  //      return err();                         /* purecov: inspected */
  //    return Value(field_type, data + 1 + n, val_len);
  //  }
    
  default:
    // Not a valid scalar type.
    return err();
  }
}
/**
  Read an offset or size field from a buffer. The offset could be either
  a two byte unsigned integer or a four byte unsigned integer.

  @param data  the buffer to read from
  @param large tells if the large or small storage format is used; true
               means read four bytes, false means read two bytes
*/
static size_t read_offset_or_size(const char *data, bool large)
{
  return large ? uint4korr(data) : uint2korr(data);
}

/**
  Parse a JSON array or object.

  @param t      type (either ARRAY or OBJECT)
  @param data   pointer to the start of the array or object
  @param len    the maximum number of bytes to read from data
  @param large  if true, the array or object is stored using the large
                storage format; otherwise, it is stored using the small
                storage format
  @return  an object that allows access to the array or object
*/
static Value parse_array_or_object(Value::mysql_value_enum_type t, const char *data,
                                   size_t len, bool large)
{
  DBUG_ASSERT(t == Value::ARRAY || t == Value::OBJECT);

  /*
    Make sure the document is long enough to contain the two length fields
    (both number of elements or members, and number of bytes).
  */
  const size_t offset_size= large ? LARGE_OFFSET_SIZE : SMALL_OFFSET_SIZE;
  if (len < 2 * offset_size)
    return err();
  const size_t element_count= read_offset_or_size(data, large);
  const size_t bytes= read_offset_or_size(data + offset_size, large);

  // The value can't have more bytes than what's available in the data buffer.
  if (bytes > len)
    return err();

  /*
    Calculate the size of the header. It consists of:
    - two length fields
    - if it is a JSON object, key entries with pointers to where the keys
      are stored
    - value entries with pointers to where the actual values are stored
  */
  size_t header_size= 2 * offset_size;
  if (t == Value::OBJECT)
    header_size+= element_count *
      (large ? KEY_ENTRY_SIZE_LARGE : KEY_ENTRY_SIZE_SMALL);
  header_size+= element_count *
    (large ? VALUE_ENTRY_SIZE_LARGE : VALUE_ENTRY_SIZE_SMALL);

  // The header should not be larger than the full size of the value.
  if (header_size > bytes)
    return err();                             /* purecov: inspected */

  return Value(t, data, bytes, element_count, large);
}

/**
  Parse a JSON value within a larger JSON document.

  @param type   the binary type of the value to parse
  @param data   pointer to the start of the binary representation of the value
  @param len    the maximum number of bytes to read from data
  @return  an object that allows access to the value
*/
static Value parse_value(uint8 type, const char *data, size_t len)
{

  // Parse further => type = data[0] , data+1 , len=len-1
  switch (type)
  {
    case JSONB_TYPE_SMALL_OBJECT:
      //return parse_scalar(type, data, len);
      return parse_array_or_object(Value::OBJECT, data, len, false);
    case JSONB_TYPE_LARGE_OBJECT:
      return parse_scalar(type, data, len);
      return parse_array_or_object(Value::OBJECT, data, len, false);
    case JSONB_TYPE_SMALL_ARRAY:
      return parse_array_or_object(Value::OBJECT, data, len, false);
    case JSONB_TYPE_LARGE_ARRAY:
      return parse_array_or_object(Value::OBJECT, data, len, false);
    default:
      return parse_scalar(type, data, len);

  }
}

Value parse_binary(const char *data, size_t len)
{
  if (len<1)
    return err(); // error
  uint8 type= data[0];
  return parse_value(type, data+1, len-1);
}


}//end of namespace json_mysql_binary