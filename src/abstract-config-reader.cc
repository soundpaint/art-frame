/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2015, 2016, 2019 Jürgen Reuter
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * As a special exception to the GNU General Public License, if you
 * distribute this file as part of a program that contains a
 * configuration script generated by Autoconf, you may include it
 * under the same distribution terms that you use for the rest of that
 * program.
 *
 * For updates and more info or contacting the author, visit:
 * <https://github.com/soundpaint/art-frame>
 *
 * Author's web site: www.juergen-reuter.de
 */

#include <abstract-config-reader.hh>
#include <cerrno>
#include <log.hh>
#include <xml-node-list.hh>
#include <xml-parser-error-handler.hh>
#include <xercesc/parsers/XercesDOMParser.hpp>

const locale_t
Abstract_config_reader::LOCALE_C = create_locale_c();

const locale_t
Abstract_config_reader::create_locale_c()
{
  return newlocale(LC_NUMERIC, "C", 0);
}

Abstract_config_reader::Abstract_config_reader()
{
  try {
    xercesc::XMLPlatformUtils::Initialize();
  } catch (const xercesc::XMLException &exc) {
    std::stringstream full_msg;
    char *message = xercesc::XMLString::transcode(exc.getMessage());
    full_msg << "Abstract_config_reader::Abstract_config_reader(): "
      "failed parsing XML config file: " << message;
    xercesc::XMLString::release(&message);
    fatal(full_msg.str());
  }
}

Abstract_config_reader::~Abstract_config_reader()
{
  xercesc::XMLPlatformUtils::Terminate();
}

void
Abstract_config_reader::fatal(const char *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::fatal(): "
    "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Abstract_config_reader::fatal(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::fatal(): "
    "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Abstract_config_reader::fatal(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::fatal(): "
    "failed parsing XML config file: " << msg;
  Log::fatal(full_msg.str());
}

void
Abstract_config_reader::error(const char *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::error(): "
    "failed parsing XML config file: " << msg;
  Log::error(full_msg.str());
}

void
Abstract_config_reader::error(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::error(): "
    "failed parsing XML config file: " << msg;
  Log::error(full_msg.str());
}

void
Abstract_config_reader::error(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::error(): "
    "failed parsing XML config file: " << msg;
  Log::error(full_msg.str());
}

void
Abstract_config_reader::warn(const char *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::warn(): "
    "while parsing XML config file: " << msg;
  Log::warn(full_msg.str());
}

void
Abstract_config_reader::warn(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::warn(): "
    "while parsing XML config file: " << msg;
  Log::warn(full_msg.str());
}

void
Abstract_config_reader::warn(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::warn(): "
    "while parsing XML config file: " << msg;
  Log::warn(full_msg.str());
}

void
Abstract_config_reader::info(const char *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::info(): "
    "while parsing XML config file: " << msg;
  Log::info(full_msg.str());
}

void
Abstract_config_reader::info(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::info(): "
    "while parsing XML config file: " << msg;
  Log::info(full_msg.str());
}

void
Abstract_config_reader::info(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::info(): "
    "while parsing XML config file: " << msg;
  Log::info(full_msg.str());
}

void
Abstract_config_reader::debug(const char *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::debug(): "
    "while parsing XML config file: " << msg;
  Log::debug(full_msg.str());
}

void
Abstract_config_reader::debug(const std::string msg)
{
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::debug(): "
    "while parsing XML config file: " << msg;
  Log::debug(full_msg.str());
}

void
Abstract_config_reader::debug(const XMLCh *msg) {
  std::stringstream full_msg;
  full_msg << "Abstract_config_reader::debug(): "
    "while parsing XML config file: " << msg;
  Log::debug(full_msg.str());
}

const bool
Abstract_config_reader::parse_bool(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  bool value;
  if (xercesc::XMLString::equals(token_as_c_star, "true")) {
    value = true;
  } else if (xercesc::XMLString::equals(token_as_c_star, "false")) {
    value = false;
  } else {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_bool(): "
      "value is not a Boolean: " << token_as_c_star;
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const int32_t
Abstract_config_reader::str_to_subint32(const char *nptr, char **endptr,
                                        const int base,
                                        const long min, const long max)
{
  int32_t value = strtol_l(nptr, endptr, base, LOCALE_C);
  if (value > max || (errno == ERANGE && value == LONG_MAX)) {
    value = max;
    errno = ERANGE;
  } else if (value < min || (errno == ERANGE && value == LONG_MIN)) {
    value = min;
    errno = ERANGE;
  }
  return value;
}

const int8_t
Abstract_config_reader::str_to_int8(const char *nptr, char **endptr,
                                    const int base)
{
  return str_to_subint32(nptr, endptr, base, -128, 127);
}

const uint8_t
Abstract_config_reader::str_to_uint8(const char *nptr, char **endptr,
                                     const int base)
{
  return str_to_subint32(nptr, endptr, base, 0, 255);
}

const int16_t
Abstract_config_reader::str_to_int16(const char *nptr, char **endptr,
                                     const int base)
{
  return str_to_subint32(nptr, endptr, base, -32768, 32767);
}

const uint16_t
Abstract_config_reader::str_to_uint16(const char *nptr, char **endptr,
                                      const int base)
{
  return str_to_subint32(nptr, endptr, base, 0, 65535);
}

const int8_t
Abstract_config_reader::parse_decimal_int8(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const int8_t value = str_to_int8(token_as_c_star, &err_pos, 10);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_int8(): "
      "value is not a byte, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const uint8_t
Abstract_config_reader::parse_decimal_uint8(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const uint16_t value = str_to_uint8(token_as_c_star, &err_pos, 10);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_uint8(): "
      "value is not an unsigned byte, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const int16_t
Abstract_config_reader::parse_decimal_int16(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const int16_t value = str_to_int16(token_as_c_star, &err_pos, 10);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_int16(): "
      "value is not a short, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const uint16_t
Abstract_config_reader::parse_decimal_uint16(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const uint16_t value = str_to_uint16(token_as_c_star, &err_pos, 10);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_uint16(): "
      "value is not an unsigned short, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const int32_t
Abstract_config_reader::parse_decimal_int32(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const int32_t value = strtol_l(token_as_c_star, &err_pos, 10, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_int32(): "
      "value is not a long, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const uint32_t
Abstract_config_reader::parse_decimal_uint32(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const uint32_t value = strtoul_l(token_as_c_star, &err_pos, 10, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_uint32(): "
      "value is not an unsigned long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const uint32_t
Abstract_config_reader::parse_hex_or_dec_or_oct_int32(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const uint32_t value = strtoul_l(token_as_c_star, &err_pos, 0, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_hex_or_decimal_int32(): "
      "value is not an unsigned long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const int64_t
Abstract_config_reader::parse_decimal_int64(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const int64_t value = strtoll_l(token_as_c_star, &err_pos, 10, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_int64(): "
      "value is not a long long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const uint64_t
Abstract_config_reader::parse_decimal_uint64(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const uint64_t value = strtoull_l(token_as_c_star, &err_pos, 10, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_decimal_uint64(): "
      "value is not an unsigned long long, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const float
Abstract_config_reader::parse_float(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const float value = strtof_l(token_as_c_star, &err_pos, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_float(): "
      "value is not a double, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const double
Abstract_config_reader::parse_double(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const double value = strtod_l(token_as_c_star, &err_pos, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_double(): "
      "value is not a double, error position: " << (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

const long double
Abstract_config_reader::parse_long_double(const XMLCh *token)
{
  char *token_as_c_star = xercesc::XMLString::transcode(token);
  char *err_pos;
  const double value = strtold_l(token_as_c_star, &err_pos, LOCALE_C);
  if (err_pos != token_as_c_star + strlen(token_as_c_star)) {
    std::stringstream full_msg;
    full_msg << "Abstract_config_reader::parse_long_double(): "
      "value is not a long double, error position: " <<
      (int)(*err_pos);
    fatal(full_msg.str());
  }
  xercesc::XMLString::release(&token_as_c_star);
  return value;
}

/*
 * Main difference of this method compared to method
 * DOMElement::getElementsByTagName() is that this method does not
 * return all descendants of any depth, but only direct child
 * elements.
 *
 * Note that this method does not support wildcard characters like "*"
 * for pattern matching, but matches only exact node names.  For
 * matching all direct child elements with whatever node name, use
 * NULL as tag name.
 */
const xercesc::DOMNodeList *
Abstract_config_reader::get_children_by_tag_name(const xercesc::DOMElement *parent,
                                                 const XMLCh *tag_name)
{
  const xercesc::DOMNodeList *node_list =
    parent->getChildNodes();
  if (node_list) {
    Xml_node_list *nodes = new Xml_node_list();
    if (!nodes) {
      Log::fatal("Abstract_config_reader::get_children_by_tag_name(): "
                 "not enough memory");
    }
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      xercesc::DOMNode *node = node_list->item(node_index);
      xercesc::DOMElement *elem =
        dynamic_cast<xercesc::DOMElement *>(node);
      if (!elem) {
        // not an element, but e.g. text node => skip
        continue;
      }
      const XMLCh *elem_name = elem->getLocalName();
      if ((!tag_name) || xercesc::XMLString::equals(elem_name, tag_name)) {
        nodes->add(elem);
      }
    }
    return nodes;
  } else {
    return 0;
  }
}

const xercesc::DOMElement *
Abstract_config_reader::get_single_child_element(const xercesc::DOMElement *parent,
                                                 const char *name,
                                                 const bool required)
{
  XMLCh *utf8_name = xercesc::XMLString::transcode(name);
  if (!utf8_name) {
    fatal("Abstract_config_reader::get_single_child_element(): "
          "not enough memory");
  }
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(parent, utf8_name);
  xercesc::XMLString::release(&utf8_name);
  utf8_name = 0;
  if (!node_list) {
    fatal("Abstract_config_reader::get_single_child_element(): "
          "node list is NULL");
  }
  const XMLSize_t length = node_list->getLength();
  if (length > 1) {
    std::stringstream msg;
    msg << "Abstract_config_reader::get_single_child_element(): "
      "expected single node '" << name << "', got " << length <<
      " nodes instead";
    fatal(msg.str());
  }
  if (required) {
    if (length < 1) {
      std::stringstream msg;
      msg << "Abstract_config_reader::get_single_child_element(): "
        "missing node '" << name << "'";
      fatal(msg.str());
    }
  }
  const xercesc::DOMElement *element;
  if (length > 0) {
    const xercesc::DOMNode *node = node_list->item(0);
    if (!node) {
      fatal("Abstract_config_reader::get_single_child_element(): "
            "node is NULL");
    }
    element = dynamic_cast<const xercesc::DOMElement *>(node);
    if (!element) {
      fatal("Abstract_config_reader::get_single_child_element(): "
            "element is NULL");
    }
  } else {
    element = 0;
  }
  return element;
}

void
Abstract_config_reader::parse(const char *path)
{
  if (!path) {
    fatal("Abstract_config_reader::parse(): path is NULL");
  }
  xercesc::XercesDOMParser *parser = new xercesc::XercesDOMParser();
  if (!parser) {
    fatal("Abstract_config_reader::parse(): failed creating DOM parser");
  }

  parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
  parser->setDoNamespaces(true);
  parser->setDoSchema(true);
  parser->setValidationConstraintFatal(true);
  parser->setValidationSchemaFullChecking(true);
  Xml_parser_error_handler *error_handler = new Xml_parser_error_handler();
  if (!error_handler) {
    fatal("Abstract_config_reader::parse(): failed creating error handler");
  }
  parser->setErrorHandler(error_handler);

  xercesc::DOMDocument *doc;
  {
    std::stringstream message;
    message << "Abstract_config_reader::parse(): "
      "reading XML config from file " << path;
    Log::info(message.str());
  }
  try {
    parser->parse(path);
    const XMLSize_t error_count = parser->getErrorCount();
    if (error_count > 0) {
      std::stringstream message;
      message << "Abstract_config_reader::parse(): " <<
        error_count << " error(s) occurred while parsing";
      Log::warn(message.str());
    }
    doc = parser->getDocument();
  } catch (const xercesc::XMLException &exc) {
    std::stringstream full_msg;
    char *message = xercesc::XMLString::transcode(exc.getMessage());
    full_msg << "Abstract_config_reader::Config(): "
      "failed parsing XML config file: " << message;
    xercesc::XMLString::release(&message);
    fatal(full_msg.str());
  } catch (const xercesc::DOMException &exc) {
    std::stringstream full_msg;
    char *message = xercesc::XMLString::transcode(exc.getMessage());
    full_msg << "Abstract_config_reader::Config(): "
      "failed parsing XML config file: " << message;
    xercesc::XMLString::release(&message);
    fatal(full_msg.str());
  } catch (const xercesc::SAXParseException &exc) {
    std::stringstream full_msg;
    char *message = xercesc::XMLString::transcode(exc.getMessage());
    full_msg << "Abstract_config_reader::parse(): "
      "SAXParseException: " << message;
    xercesc::XMLString::release(&message);
    fatal(full_msg.str());
  } catch (const xercesc::SAXException &exc) {
    std::stringstream full_msg;
    char *message = xercesc::XMLString::transcode(exc.getMessage());
    full_msg << "Abstract_config_reader::parse(): SAXException: " << message;
    xercesc::XMLString::release(&message);
    fatal(full_msg.str());
  } catch (...) {
    fatal("Abstract_config_reader::parse(): "
          "unknown exception while reading XML file");
  }
  if (doc) {
    info("Abstract_config_reader::parse(): XSD successfully read");
  } else {
    fatal("Abstract_config_reader::parse(): "
          "document is NULL (maybe XML file does not exist?)");
  }
  const xercesc::DOMElement *elem_config = doc->getDocumentElement();
  if (!elem_config) {
    fatal("Abstract_config_reader::parse(): document element is NULL");
  }
  parse_document(elem_config);
  print_config();

  elem_config = 0;

  // Do *not* call "doc->release()", since the document is
  // automatically released by xercesc when releasing the parser.
  doc = 0;

  delete parser;
  parser = 0;

  delete error_handler;
  error_handler = 0;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
