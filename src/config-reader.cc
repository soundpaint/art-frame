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

#include <config-reader.hh>
#include <cerrno>
#include <unistd.h>
#include <log.hh>

#define CONFIG_SCHEMA_LOCATION "http://soundpaint.org/schema/soundcolumn/config.xsd"

Config_reader::Config_reader(Config *config)
  : Abstract_config_reader()
{
  if (!config) {
    Log::fatal("Config_reader::Config_reader(): config is null");
  }
  _config = config;
}

Config_reader::~Config_reader()
{
  // Q objects will be deleted by Qt, just set them to 0

  _config = 0;
}

void
Config_reader::parse_document(const xercesc::DOMElement *elem_config)
{
  const XMLCh *node_name = elem_config->getNodeName();

  char *node_name_as_c_star = xercesc::XMLString::transcode(node_name);
  if (!node_name_as_c_star) {
    Log::fatal("Config_reader::parse_document(): not enough memory");
  }

  if (strcmp(node_name_as_c_star, "config")) {
    Log::fatal(node_name_as_c_star);
  }

  xercesc::XMLString::release(&node_name_as_c_star);
  node_name_as_c_star = 0;

  const xercesc::DOMElement *elem_power_save =
    get_single_child_element(elem_config, "power-save");
  if (elem_power_save) {
    parse_power_save(elem_power_save);
  }

  const xercesc::DOMElement *elem_images =
    get_single_child_element(elem_config, "images");
  if (elem_images) {
    parse_images(elem_images);
  } else {
    fatal("missing \"images\" element");
  }
}

void
Config_reader::parse_power_save(const xercesc::DOMElement *elem_power_save)
{
  // start_fan_temperature
  const xercesc::DOMElement *elem_start_fan_temperature =
    get_single_child_element(elem_power_save, "start_fan_temperature");
  if (elem_start_fan_temperature) {
    const XMLCh *start_fan_temperature =
      elem_start_fan_temperature->getTextContent();
    const double start_fan_temperature_value =
      parse_double(start_fan_temperature);
    _config->set_start_fan_temperature(start_fan_temperature_value);
  }

  // stop_fan_temperature
  const xercesc::DOMElement *elem_stop_fan_temperature =
    get_single_child_element(elem_power_save, "stop_fan_temperature");
  if (elem_stop_fan_temperature) {
    const XMLCh *stop_fan_temperature =
      elem_stop_fan_temperature->getTextContent();
    const double stop_fan_temperature_value =
      parse_double(stop_fan_temperature);
    _config->set_stop_fan_temperature(stop_fan_temperature_value);
  }

  // start_cooling_break_temperature
  const xercesc::DOMElement *elem_start_cooling_break_temperature =
    get_single_child_element(elem_power_save,
                             "start_cooling_break_temperature");
  if (elem_start_cooling_break_temperature) {
    const XMLCh *start_cooling_break_temperature =
      elem_start_cooling_break_temperature->getTextContent();
    const double start_cooling_break_temperature_value =
      parse_double(start_cooling_break_temperature);
    _config->set_start_cooling_break_temperature(start_cooling_break_temperature_value);
  }

  // stop_cooling_break_temperature
  const xercesc::DOMElement *elem_stop_cooling_break_temperature =
    get_single_child_element(elem_power_save,
                             "stop_cooling_break_temperature");
  if (elem_stop_cooling_break_temperature) {
    const XMLCh *stop_cooling_break_temperature =
      elem_stop_cooling_break_temperature->getTextContent();
    const double stop_cooling_break_temperature_value =
      parse_double(stop_cooling_break_temperature);
    _config->set_stop_cooling_break_temperature(stop_cooling_break_temperature_value);
  }

  // frame-usleep-min
  const xercesc::DOMElement *elem_frame_usleep_min =
    get_single_child_element(elem_power_save, "frame-usleep-min");
  if (elem_frame_usleep_min) {
    const XMLCh *frame_usleep_min = elem_frame_usleep_min->getTextContent();
    const uint32_t frame_usleep_min_value =
      parse_decimal_uint32(frame_usleep_min);
    _config->set_frame_usleep_min(frame_usleep_min_value);
  }

  // frame-usleep-max
  const xercesc::DOMElement *elem_frame_usleep_max =
    get_single_child_element(elem_power_save, "frame-usleep-max");
  if (elem_frame_usleep_max) {
    const XMLCh *frame_usleep_max = elem_frame_usleep_max->getTextContent();
    const uint32_t frame_usleep_max_value =
      parse_decimal_uint32(frame_usleep_max);
    _config->set_frame_usleep_max(frame_usleep_max_value);
  }
}

void
Config_reader::parse_images(const xercesc::DOMElement *elem_config)
{
  XMLCh *node_name_image = xercesc::XMLString::transcode("image");
  if (!node_name_image) {
    Log::fatal("Config_reader::parse_images(): not enough memory");
  }
  const xercesc::DOMNodeList *node_list =
    elem_config->getElementsByTagName(node_name_image);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      const xercesc::DOMNode *node = node_list->item(node_index);
      const xercesc::DOMElement *image =
	dynamic_cast<const xercesc::DOMElement *>(node);
      if (!image) {
	fatal("Config_reader::parse_images(): unexpected null element");
      }
      const Config_image *config_image = parse_image(image);
      if (config_image) {
        _config->add_image(config_image);
      } else {
        Log::warn("Config_reader::parse_images(): skipping image");
      }
    }
  } else {
    // no images => nothing to parse
  }
  if (!_config->get_images_count()) {
    Log::fatal("Config_reader::parse_images(): no image loaded");
  }
  xercesc::XMLString::release(&node_name_image);
}

const Config_image *
Config_reader::parse_image(const xercesc::DOMElement *elem_image)
{
  const Config_image *config_image;
  const xercesc::DOMElement *elem_from_file =
    get_single_child_element(elem_image, "from-file");
  if (elem_from_file) {
    config_image = parse_from_file(elem_from_file);
  } else {
    fatal("for now, image definition must contain from-file definition");
  }
  return config_image;
}

const Config_image *
Config_reader::parse_from_file(const xercesc::DOMElement *elem_from_file)
{
  const XMLCh *path;
  const xercesc::DOMElement *elem_path =
    get_single_child_element(elem_from_file, "path");
  if (elem_path) {
    path = parse_path(elem_path);
  } else {
    fatal("for now, from-file definition must contain path definition");
  }
  char *str_path = xercesc::XMLString::transcode(path);
  const Config_image *config_image = Config_image::create(str_path);
  xercesc::XMLString::release(&str_path);
  str_path = 0;
  if (!config_image) {
    Log::warn("Config_reader::parse_document(): failed accessing image");
  }
  return config_image;
}

const XMLCh *
Config_reader::parse_path(const xercesc::DOMElement *elem_path)
{
  const XMLCh *node_value_path = elem_path->getTextContent();
  return node_value_path;
}

void
Config_reader::print_config() const
{
  std::stringstream msg;
  msg << "\r\n";
  msg << "######## CONFIG ########\r\n";
  // TODO
  msg << "[TODO]\r\n";
  msg << "########################";
  Log::info(msg.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
