#ifndef JSON_DOM_INCLUDED
#define JSON_DOM_INCLUDED
/*
#include "my_global.h"
#include "malloc_allocator.h"   // Malloc_allocator
#include "my_decimal.h"         // my_decimal
#include "binary_log_types.h"   // enum_field_types
#include "my_time.h"            // my_time_flags_t
#include "mysql_time.h"         // MYSQL_TIME

#include "sql_alloc.h"          // Sql_alloc
#include "sql_error.h"          // Sql_condition
#include "prealloced_array.h"   // Prealloced_array

#include <map>
#include <string>
*/
#include "my_global.h"
#include "sql_alloc.h"          // Sql_alloc
#include "sql_error.h"          // Sql_condition
//#include "prealloced_array.h"   // Prealloced_array
#include "json_mysql_binary.h"        // json_mysql_binary::Value
/**
  @class
  @brief JSON DOM classes: Abstract base class is Json_dom -
  MySQL representation of in-memory JSON objects used by the JSON type
  Supports access, deep cloning, and updates. See also Json_wrapper and
  json_mysql_binary::Value.
  Uses heap for space allocation for now. FIXME.

  Class hierarchy:
  <code><pre>
      Json_dom (abstract)
       Json_scalar (abstract)
         Json_string
         Json_number (abstract)
           Json_decimal
           Json_int
           Json_uint
           Json_double
         Json_boolean
         Json_null
         Json_datetime
         Json_opaque
       Json_object
       Json_array
  </pre></code>
  At the outset, object and array add/insert/append operations takes
  a clone unless specified in the method, e.g. add_alias hands the
  responsibility for the passed in object over to the object.
*/


/**
  @class
  Abstraction for accessing JSON values irrespective of whether they
  are (started out as) binary JSON values or JSON DOM values. The
  purpose of this is to allow uniform access for callers. It allows us
  to access binary JSON values without necessarily building a DOM (and
  thus having to read the entire value unless necessary, e.g. for
  accessing only a single array slot or object field).

  Instances of this class are usually created on the stack. In some
  cases instances are cached in an Item and reused, in which case they
  are allocated from query-duration memory (which is why the class
  inherits from Sql_alloc).
*/
class Json_dom
{
  // so that these classes can call set_parent()
  //friend class Json_object;
  //friend class Json_array;
public:
  /**
    Json values in MySQL comprises the stand set of JSON values plus a
    MySQL specific set. A Json _number_ type is subdivided into _int_,
    _uint_, _double_ and _decimal_.

    MySQL also adds four built-in date/time values: _date_, _time_,
    _datetime_ and _timestamp_.  An additional _opaque_ value can
    store any other MySQL type.

    The enumeration is common to Json_dom and Json_wrapper.

    The enumeration is also used by Json_wrapper::compare() to
    determine the ordering when comparing values of different types,
    so the order in which the values are defined in the enumeration,
    is significant. The expected order is null < number < string <
    object < array < boolean < date < time < datetime/timestamp <
    opaque.
  */
  enum enum_json_type {
    J_NULL,
    J_DECIMAL,
    J_INT,
    J_UINT,
    J_DOUBLE,
    J_STRING,
    J_OBJECT,
    J_ARRAY,
    J_BOOLEAN,
    J_DATE,
    J_TIME,
    J_DATETIME,
    J_TIMESTAMP,
    J_OPAQUE,
    J_ERROR
  };

  /**
    Extended type ids so that JSON_TYPE() can give useful type
    names to certain sub-types of J_OPAQUE.
  */
  enum enum_json_opaque_type {
    J_OPAQUE_BLOB,
    J_OPAQUE_BIT,
    J_OPAQUE_GEOMETRY
  };

protected:
  Json_dom() : m_parent(NULL) {}

  /**
    Set the parent dom to which this dom is attached.

    @param[in] parent the parent we're being attached to
  */
  void set_parent(Json_dom *parent) { m_parent= parent; }

public:
  virtual ~Json_dom() {}
  /**
    Make a deep clone. The ownership of the returned object is
    henceforth with the caller.

    @return a cloned Json_dom object.
  */
  virtual Json_dom *clone() const= 0;

private:

  /** Parent pointer */
  Json_dom *m_parent;

}; // end of Json_dom

class Json_wrapper : Sql_alloc
{
private:
  bool m_is_dom;      //!< Wraps a DOM iff true
  bool m_dom_alias;   //!< If true, don't deallocate in destructor
  json_mysql_binary::Value m_value;
  Json_dom *m_dom_value;

  /**
    Get the wrapped datetime value in the packed format.

    @param[in,out] buffer a char buffer with space for at least
    Json_datetime::PACKED_SIZE characters
    @return a char buffer that contains the packed representation of the
    datetime (may or may not be the same as buffer)
  */
  const char *get_datetime_packed(char *buffer) const;

public:
  /**
    Create an empty wrapper. Cf ::empty().
  */
  Json_wrapper()
    : m_is_dom(true), m_dom_alias(true), m_value(), m_dom_value(NULL)
  {}

  void steal(Json_wrapper *old);

  /**
    Wrap a binary value. Does not copy the underlying buffer, so
    lifetime is limited the that of the supplied value.

    @param[in] value  the binary value
  */
  explicit Json_wrapper(const json_mysql_binary::Value &value);
  /**
    Copy constructor. Does a deep copy of any owned DOM. If a DOM
    os not owned (aliased), the copy will also be aliased.
  */
  //Json_wrapper(const Json_wrapper &old);

  /**
    Assignment operator. Does a deep copy of any owned DOM. If a DOM
    os not owned (aliased), the copy will also be aliased. Any owned
    DOM in the left side will be deallocated.
  */
  Json_wrapper &operator=(const Json_wrapper &old);

  //~Json_wrapper();

  /**
    A Wrapper is defined to be empty if it is passed a NULL value with the
    constructor for JSON dom, or if the default constructor is used.

    @return true if the wrapper is empty.
  */
  bool empty() const { return m_is_dom && !m_dom_value; }

  /**
    Format the JSON value to an external JSON string in buffer in
    the format of ISO/IEC 10646.

    @param[in,out] buffer      the formatted string is appended, so make sure
                               the length is set correctly before calling
    @param[in]     json_quoted if the JSON value is a string and json_quoted
                               is false, don't perform quoting on the string.
                               This is only used by JSON_UNQUOTE.
    @param[in]     func_name   The name of the function that called to_string().

    @return false formatting went well, else true
  */
  bool to_string(String *buffer, bool json_quoted, const char *func_name) const;

  // Accessors

  /**
    Return the type of the wrapped JSON value

    @return the type
  */
  Json_dom::enum_json_type type() const;

  /**
    Return the MYSQL type of the opaque value, see ::type(). Valid for
    J_OPAQUE.  Calling this method if the type is not J_OPAQUE will give
    undefined results.

    @return the type
  */
  enum_field_types field_type() const;


  /**
    Get the wrapped contents in DOM form. Same as to_dom(), except it returns
    a clone of the original DOM instead of the actual, internal DOM tree.

    @return pointer to a DOM object, or NULL if the DOM could not be allocated
  */
  Json_dom *clone_dom();

}; // end of Json_wrapper class

#endif /* JSON_DOM_INCLUDED */