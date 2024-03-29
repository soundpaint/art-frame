/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2016, 2019 Jürgen Reuter
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

#ifndef SWEEP_INERTIA_HH
#define SWEEP_INERTIA_HH

#include <time.h>
#include <QtCore/QPointF>
#include <iconfig.hh>

class Sweep_inertia
{
public:
  Sweep_inertia(const uint16_t width, const uint16_t height,
                const IConfig *config);
  virtual ~Sweep_inertia();
  void add_sweep(const QPointF pos0, const QPointF pos1, const double time);
  void fade_step();
  const double get_inertia_x(const uint32_t pos_x, const uint32_t pos_y) const;
  const double get_inertia_y(const uint32_t pos_x, const uint32_t pos_y) const;
private:
  uint16_t _width;
  uint16_t _height;
  const IConfig *_config;
  struct inertia_t {
    double x, y;
  };
  struct inertia_t *_inertia;
  static double *envelope;
  const double get_envelope(const int16_t thickness_index,
                            const double peak_sensitivity);
  void add_horizontal_sweep(const uint32_t x0,
                            const uint32_t y0,
                            const uint32_t x1,
                            const uint32_t y1,
                            const struct inertia_t inertia);
  void add_vertical_sweep(const uint32_t x0,
                          const uint32_t y0,
                          const uint32_t x1,
                          const uint32_t y1,
                          const struct inertia_t inertia);
};

#endif /* SWEEP_INERTIA_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
