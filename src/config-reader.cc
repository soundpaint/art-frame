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
    Log::fatal("Config_reader::Config_reader(): config is NULL");
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

  const xercesc::DOMElement *elem_kiosk_mode =
    get_single_child_element(elem_config, "kiosk-mode");
  if (elem_kiosk_mode) {
    parse_kiosk_mode(elem_kiosk_mode);
  }

  const xercesc::DOMElement *elem_simulation =
    get_single_child_element(elem_config, "simulation");
  if (elem_simulation) {
    parse_simulation(elem_simulation);
  }

  const xercesc::DOMElement *elem_audio =
    get_single_child_element(elem_config, "audio");
  if (elem_audio) {
    parse_audio(elem_audio);
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
    if (start_fan_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "start fan temperature value < 0");
    }
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
    if (stop_fan_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "stop fan temperature value < 0");
    }
    if (_config->get_start_fan_temperature() <
        stop_fan_temperature_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "fan temperature: start value < stop value");
    }
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
    if (start_cooling_break_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "start cooling break temperature value < 0");
    }
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
    if (stop_cooling_break_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "stop cooling break temperature value < 0");
    }
    if (_config->get_start_cooling_break_temperature() <
        stop_cooling_break_temperature_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "cooling break temperature: start value < stop value");
    }
    _config->set_stop_cooling_break_temperature(stop_cooling_break_temperature_value);
  }

  // frame-usleep-min
  const xercesc::DOMElement *elem_frame_usleep_min =
    get_single_child_element(elem_power_save, "frame-usleep-min");
  if (elem_frame_usleep_min) {
    const XMLCh *frame_usleep_min = elem_frame_usleep_min->getTextContent();
    const uint32_t frame_usleep_min_value =
      parse_decimal_uint32(frame_usleep_min);
    if (frame_usleep_min_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep min value < 0");
    }
    _config->set_frame_usleep_min(frame_usleep_min_value);
  }

  // frame-usleep-max
  const xercesc::DOMElement *elem_frame_usleep_max =
    get_single_child_element(elem_power_save, "frame-usleep-max");
  if (elem_frame_usleep_max) {
    const XMLCh *frame_usleep_max = elem_frame_usleep_max->getTextContent();
    const uint32_t frame_usleep_max_value =
      parse_decimal_uint32(frame_usleep_max);
    if (frame_usleep_max_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep max value < 0");
    }
    if (_config->get_frame_usleep_min() > frame_usleep_max_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep: min value < max value");
    }
    _config->set_frame_usleep_max(frame_usleep_max_value);
  }
}

void
Config_reader::parse_kiosk_mode(const xercesc::DOMElement *elem_kiosk_mode)
{
  // control_autohide_after
  const xercesc::DOMElement *elem_control_autohide_after =
    get_single_child_element(elem_kiosk_mode, "control-autohide-after");
  if (elem_control_autohide_after) {
    const XMLCh *control_autohide_after =
      elem_control_autohide_after->getTextContent();
    const uint16_t control_autohide_after_value =
      parse_decimal_uint16(control_autohide_after);
    if (control_autohide_after_value < 0) {
      Log::fatal("Config_reader::parse_kiosk_mode(): control autohide after < 0");
    }
    _config->set_control_autohide_after(control_autohide_after_value);
  }

  // enable_cursor
  const xercesc::DOMElement *elem_enable_cursor =
    get_single_child_element(elem_kiosk_mode, "enable-cursor");
  if (elem_enable_cursor) {
    const XMLCh *enable_cursor =
      elem_enable_cursor->getTextContent();
    const bool enable_cursor_value =
      parse_bool(enable_cursor);
    _config->set_enable_cursor(enable_cursor_value);
  }

  // enable_button_quit
  const xercesc::DOMElement *elem_enable_button_quit =
    get_single_child_element(elem_kiosk_mode, "enable-button-quit");
  if (elem_enable_button_quit) {
    const XMLCh *enable_button_quit =
      elem_enable_button_quit->getTextContent();
    const bool enable_button_quit_value =
      parse_bool(enable_button_quit);
    _config->set_enable_button_quit(enable_button_quit_value);
  }

  // enable_key_quit
  const xercesc::DOMElement *elem_enable_key_quit =
    get_single_child_element(elem_kiosk_mode, "enable-key-quit");
  if (elem_enable_key_quit) {
    const XMLCh *enable_key_quit =
      elem_enable_key_quit->getTextContent();
    const bool enable_key_quit_value =
      parse_bool(enable_key_quit);
    _config->set_enable_key_quit(enable_key_quit_value);
  }
}

void
Config_reader::parse_simulation(const xercesc::DOMElement *elem_simulation)
{
  // simulation_start_on_application_start
  const xercesc::DOMElement *elem_start_on_application_start =
    get_single_child_element(elem_simulation,
                             "start-on-application-start");
  if (elem_start_on_application_start) {
    const XMLCh *start_on_application_start =
      elem_start_on_application_start->getTextContent();
    const bool start_on_application_start_value =
      parse_bool(start_on_application_start);
    _config->set_simulation_start_on_application_start(start_on_application_start_value);
  }

  // initial_speed
  const xercesc::DOMElement *elem_initial_speed =
    get_single_child_element(elem_simulation, "initial-speed");
  if (elem_initial_speed) {
    const XMLCh *initial_speed = elem_initial_speed->getTextContent();
    const double initial_speed_value = parse_double(initial_speed);
    if (initial_speed_value < 0.0) {
      Log::fatal("Config_reader::parse_simulation(): initial speed < 0.0");
    }
    if (initial_speed_value > 1.0) {
      Log::fatal("Config_reader::parse_simulation(): initial speed > 1.0");
    }
    _config->set_simulation_initial_speed(initial_speed_value);
  }
}

void
Config_reader::parse_audio(const xercesc::DOMElement *elem_audio)
{
  // enable_audio
  const xercesc::DOMElement *elem_enable_audio =
    get_single_child_element(elem_audio, "enable-audio");
  if (elem_enable_audio) {
    const XMLCh *enable_audio = elem_enable_audio->getTextContent();
    const bool enable_audio_value = parse_bool(enable_audio);
    _config->set_enable_audio(enable_audio_value);
  }

  // initial_volume
  const xercesc::DOMElement *elem_initial_volume =
    get_single_child_element(elem_audio, "initial-volume");
  if (elem_initial_volume) {
    const XMLCh *initial_volume = elem_initial_volume->getTextContent();
    const double initial_volume_value = parse_double(initial_volume);
    if (initial_volume_value < 0.0) {
      Log::fatal("Config_reader::parse_audio(): initial volume < 0.0");
    }
    if (initial_volume_value > 1.0) {
      Log::fatal("Config_reader::parse_audio(): initial volume > 1.0");
    }
    _config->set_audio_initial_volume(initial_volume_value);
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
        fatal("Config_reader::parse_images(): image is NULL");
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
