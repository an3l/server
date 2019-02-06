#include "json_dom.h"
#include "sql_class.h"          // THD
#include "sql_time.h"
#include "sql_const.h"

/*
Json_dom::enum_json_type Json_wrapper::type() const
{
  if (empty())
  {
    return Json_dom::J_ERROR;
  }

  if (m_is_dom)
  {
    return m_dom_value->json_type();
  }

  json_mysql_binary::Value::enum_type typ= m_value.type();

  if (typ == json_mysql_binary::Value::OPAQUE)
  {
    const enum_field_types ftyp= m_value.field_type();

    switch (ftyp)
    {
    case MYSQL_TYPE_NEWDECIMAL:
      return Json_dom::J_DECIMAL;
    case MYSQL_TYPE_DATETIME:
      return Json_dom::J_DATETIME;
    case MYSQL_TYPE_DATE:
      return Json_dom::J_DATE;
    case MYSQL_TYPE_TIME:
      return Json_dom::J_TIME;
    case MYSQL_TYPE_TIMESTAMP:
      return Json_dom::J_TIMESTAMP;
    default: ;
      // ok, fall through
    }
  }

  return bjson2json(typ);
}
*/


void Json_wrapper::steal(Json_wrapper *old)
{
  if (old->m_is_dom)
  {
    bool old_is_aliased= old->m_dom_alias;
    old->m_dom_alias= true; // we want no deep copy now, or later
    *this= *old;
    this->m_dom_alias= old_is_aliased; // set it back
    // old is now marked as aliased, so any ownership is effectively
    // transferred to this.
  }
  else
  {
    *this= *old;
  }
}
  Json_wrapper::Json_wrapper(const json_mysql_binary::Value &value)
    : m_is_dom(false), m_dom_alias(false), m_value(value), m_dom_value(NULL)
  {}


Json_dom *Json_wrapper::clone_dom()
{
  // If we already have a DOM, return a clone of it.
  if (m_is_dom)
    return m_dom_value ? m_dom_value->clone() : NULL;

  // Otherwise, produce a new DOM tree from the binary representation.
  return Json_dom::parse(m_value);
}

Json_wrapper &Json_wrapper::operator=(const Json_wrapper& from)
{
  if (this == &from)
  {
    return *this;   // self assignment: no-op
  }

  if (m_is_dom && !m_dom_alias &&!empty())
  {
    // we own our own copy, so we are responsible for deallocation
    delete m_dom_value;
  }

  m_is_dom= from.m_is_dom;

  if (from.m_is_dom)
  {
    if (from.m_dom_alias)
    {
      m_dom_value= from.m_dom_value;
    }
    else
    {
      m_dom_value= from.m_dom_value->clone();
    }

    m_dom_alias= from.m_dom_alias;
  }
  else
  {
    m_dom_value= NULL;
    m_value= from.m_value;
  }

  return *this;
}