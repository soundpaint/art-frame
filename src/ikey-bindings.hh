/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2019 Jürgen Reuter
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

#ifndef IKEY_BINDINGS_HH
#define IKEY_BINDINGS_HH

#include <iostream>
#include <string.h>

class IKey_bindings
{
public:
  enum Action
    {
      None,
      Menu,
      Quit,
      About,
      License,
      Simulation_start_stop,
      Simulation_decrement_gravity,
      Simulation_increment_gravity,
      Image_previous,
      Image_reset,
      Image_next,
      Image_capture,
      Audio_decrement_volume,
      Audio_increment_volume,
      Audio_mute_unmute
    };
  static Action action_from_string(const char *str);
  virtual ~IKey_bindings() = 0;
  virtual const Action get_action_for_key(const int key) const = 0;
};

inline IKey_bindings::~IKey_bindings() {}

inline IKey_bindings::Action
IKey_bindings::action_from_string(const char *str) {
  if (!strcmp(str, "Menu"))
    return Menu;
  else if (!strcmp(str, "Quit"))
    return Quit;
  else if (!strcmp(str, "About"))
    return About;
  else if (!strcmp(str, "License"))
    return License;
  else if (!strcmp(str, "Simulation_start_stop"))
    return Simulation_start_stop;
  else if (!strcmp(str, "Simulation_decrement_gravity"))
    return Simulation_decrement_gravity;
  else if (!strcmp(str, "Simulation_increment_gravity"))
    return Simulation_increment_gravity;
  else if (!strcmp(str, "Image_previous"))
    return Image_previous;
  else if (!strcmp(str, "Image_reset"))
    return Image_reset;
  else if (!strcmp(str, "Image_next"))
    return Image_next;
  else if (!strcmp(str, "Image_capture"))
    return Image_capture;
  else if (!strcmp(str, "Audio_decrement_volume"))
    return Audio_decrement_volume;
  else if (!strcmp(str, "Audio_increment_volume"))
    return Audio_increment_volume;
  else if (!strcmp(str, "Audio_mute_unmute"))
    return Audio_mute_unmute;
  else
    return None;
}

inline std::ostream &operator<<(std::ostream &str,
                                const IKey_bindings &key_bindings)
{
  return str << "IKey_bindings()"; // TODO
}

#endif /* IKEY_BINDINGS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
