#ifndef JSON_MYSQL_BINARY_INCLUDED
#define JSON_MYSQL_BINARY_INCLUDED
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

 /*
@verbatim
  doc ::= type value
   type ::=
      0x00 |       // small JSON object
      0x01 |       // large JSON object
      0x02 |       // small JSON array
      0x03 |       // large JSON array
      0x04 |       // literal (true/false/null)
      0x05 |       // int16
      0x06 |       // uint16
      0x07 |       // int32
      0x08 |       // uint32
      0x09 |       // int64
      0x0a |       // uint64
      0x0b |       // double
      0x0c |       // utf8mb4 string
      0x0f         // custom data (any MySQL data type)
   value ::=
      object  |
      array   |
      literal |
      number  |
      string  |
      custom-data
   object ::= element-count size key-entry* value-entry* key* value*
   array ::= element-count size value-entry* value*
   // number of members in object or number of elements in array
  element-count ::=
      uint16 |  // if used in small JSON object/array
      uint32    // if used in large JSON object/array
   // number of bytes in the binary representation of the object or array
  size ::=
      uint16 |  // if used in small JSON object/array
      uint32    // if used in large JSON object/array
   key-entry ::= key-offset key-length
   key-offset ::=
      uint16 |  // if used in small JSON object
      uint32    // if used in large JSON object
   key-length ::= uint16    // key length must be less than 64KB
   value-entry ::= type offset-or-inlined-value
   // This field holds either the offset to where the value is stored,
  // or the value itself if it is small enough to be inlined (that is,
  // if it is a JSON literal or a small enough [u]int).
  offset-or-inlined-value ::=
      uint16 |   // if used in small JSON object/array
      uint32     // if used in large JSON object/array
   key ::= utf8mb4-data
   literal ::=
      0x00 |   // JSON null literal
      0x01 |   // JSON true literal
      0x02 |   // JSON false literal
   number ::=  ....  // little-endian format for [u]int(16|32|64), whereas
                    // double is stored in a platform-independent, eight-byte
                    // format using float8store()
   string ::= data-length utf8mb4-data
   custom-data ::= custom-type data-length binary-data
   custom-type ::= uint8   // type identifier that matches the
                          // internal enum_field_types enum
   data-length ::= uint8*  // If the high bit of a byte is 1, the length
                          // field is continued in the next byte,
                          // otherwise it is the last byte of the length
                          // field. So we need 1 byte to represent
                          // lengths up to 127, 2 bytes to represent
                          // lengths up to 16383, and so on...
  @endverbatim
 */
#include "my_global.h"
#include "sql_string.h"                         /* String */

 namespace json_mysql_binary
{
/*
  Class used for reading JSON values that are stored in the binary
  format. Values are parsed lazily, so that only the parts of the
  value that are interesting to the caller, are read. Array elements
  can be looked up in constant time using the element() function.
  Object members can be looked up in O(log n) time using the lookup()
  function.
*/
class Value
{
  public:
    enum enum_type
    {
      OBJECT, ARRAY, STRING, INT, UINT, DOUBLE,
      LITERAL_NULL, LITERAL_TRUE, LITERAL_FALSE,
      OPAQUE,
      ERROR /* Not really a type. Used to signal that an
              error was detected. */
    };

    enum_type type() const { return m_type; }

    static Value err() { return Value(ERROR); }
    /* Constructor for values that represent literals or errors. */
    explicit Value(enum_type t);
    /* Constructor for values that represent ints or uints. */
    explicit Value(enum_type t, int64 val);
    /* Constructor for values that represent doubles. */
    explicit Value(double val);
    /** Constructor for values that represent strings. */
    Value(const char *data, size_t len);
    /*
      Constructor for values that represent arrays or objects.
      @param t type
      @param data pointer to the start of the binary representation
      @param bytes the number of bytes in the binary representation of the value
      @param element_count the number of elements or members in the value
      @param large true if the value should be stored in the large
      storage format with 4 byte offsets instead of 2 byte offsets
    */
    Value(enum_type t, const char *data, size_t bytes, size_t element_count,
          bool large);
    /** Constructor for values that represent opaque data. */
    //Value(enum_field_types ft, const char *data, size_t len);
    
    /** Copy constructor. */
    Value(const Value &old)
        : m_type(old.m_type), m_data(old.m_data),
        m_element_count(old.m_element_count), m_length(old.m_length),
        m_int_value(old.m_int_value), m_double_value(old.m_double_value),
        m_large(old.m_large)
    {}

    /** Empty constructor. Produces a value that represents an error condition. */
    Value()
        : m_type(ERROR), m_data(NULL), 
        m_element_count(-1), m_length(-1), m_int_value(-1),
        m_double_value(0.0), m_large(false)
    {}
// MYSQL_TYPE_NULL
    /** Assignment operator. */
    Value &operator=(const Value &from)
    {
        if (this != &from)
        {
        // Copy the entire from value into this.
        new (this) Value(from);
        }
        return *this;
    }

   private:
    /* The type of the value. */
    const enum_type m_type;
    /**
    The MySQL field type of the value, in case the type of the value is
    OPAQUE. Otherwise, it is unused.
    */
    //const enum_field_types m_field_type ;
    /*
    Pointer to the start of the binary representation of the value. Only
    used by STRING, OBJECT and ARRAY.
    The memory pointed to by this member is not owned by this Value
    object. Callers that create Value objects must make sure that the
    memory is not freed as long as the Value object is alive.
    */
    const char *m_data;
    /*
    Element count for arrays and objects. Unused for other types.
    */
    const size_t m_element_count;
    /*
    The full length (in bytes) of the binary representation of an array or
    object, or the length of a string or opaque value. Unused for other types.
    */
    const size_t m_length;
    /* The value if the type is INT or UINT. */
    const int64 m_int_value;
    /** The value if the type is DOUBLE. */
    const double m_double_value;
    /*
    True if an array or an object uses the large storage format with 4
    byte offsets instead of 2 byte offsets.
    */
    const bool m_large;
};

 /*
  Parse a JSON binary document
  @param[in] data   a pointer to the binary data
  @param[in] len    the size of the binary document in bytes
  @return an object that allows access to the contents of the document
*/
Value parse_binary(const char *data, size_t len);

 }
 #endif  /* JSON_BINARY_INCLUDED */ 