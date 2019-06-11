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

#include <sweep-inertia.hh>
#include <string.h>
#include <cmath>
#include <log.hh>

Sweep_inertia::Sweep_inertia(const uint16_t width, const uint16_t height)
{
  _width = width;
  _height = height;
  _inertia = new inertia_t[_width * _height];
}

Sweep_inertia::~Sweep_inertia()
{
  _width = 0;
  _height = 0;
  memset(_inertia, 0, _width * _height * sizeof(inertia_t));
  delete[] _inertia;
  _inertia = 0;
}

const int16_t
Sweep_inertia::MIN_THICKNESS_INDEX = -5;

const int16_t
Sweep_inertia::MAX_THICKNESS_INDEX = +5;

double *
Sweep_inertia::envelope = 0;

const double
Sweep_inertia::get_envelope(int16_t thickness_index)
{
  // TODO: Performance: Precalculate values in array.

  // TODO: Berechnung gemäß Normalverteilung
  //return 1.0 / exp(-thickness_index * thickness_index);

  if (!envelope) {
    envelope = new double[MAX_THICKNESS_INDEX - MIN_THICKNESS_INDEX + 1];
    if (!envelope) {
      Log::fatal("Sweep_inertia::get_envelope(): not enough memory");
    }
    for (uint16_t index = 0;
	 index < MAX_THICKNESS_INDEX - MIN_THICKNESS_INDEX + 1;
	 index++) {
      envelope[index] = 1.0 / (1.0 + index * index);
    }
  }
  if (thickness_index < MIN_THICKNESS_INDEX) {
    Log::fatal("Sweep_inertia::get_envelope(): "
	       "thickness_index < MIN_THICKNESS_INDEX");
  }
  if (thickness_index > MAX_THICKNESS_INDEX) {
    Log::fatal("Sweep_inertia::get_envelope(): "
	       "thickness_index > MAX_THICKNESS_INDEX");
  }
  return envelope[thickness_index - MIN_THICKNESS_INDEX];
}

void
Sweep_inertia::add_horizontal_sweep(const uint32_t x0,
				    const uint32_t y0,
				    const uint32_t x1,
				    const uint32_t y1,
				    const struct inertia_t inertia)
{
  /*
  {
    std::stringstream msg;
    msg << "[add horizontal sweep: "
      "ix=" << inertia.x << ", iy=" << inertia.y << "]";
    Log::info(msg.str());
  }
  */
  const int32_t dx = x1 - x0;
  const int32_t dy = y1 - y0;
  for (uint32_t x = x0; x < x1; x++) {
    const uint32_t y = y0 + round(((double)(x - x0)) / dx * dy);
    if (y >= _height) {
      std::stringstream msg;
      msg << "Sweep_inertia::add_horizontal_sweep(): "
	"y >= height: " << y << " >= " << _height <<
	"(x0=" << x0 << ", y0=" << y0 << ", x=" << x <<
	", dx=" << dx << ", dy=" << dy << ")";
      Log::fatal(msg.str());
    }
    for (int16_t thickness_index = MIN_THICKNESS_INDEX;
	 thickness_index <= MAX_THICKNESS_INDEX; thickness_index++) {
      const int32_t ty = y + thickness_index;
      if ((ty >= 0) && (ty < _height)) { 
	const double envelope = get_envelope(thickness_index);
	_inertia[ty * _width + x].x += inertia.x * envelope;
	_inertia[ty * _width + x].y += inertia.y * envelope;
      }
    }
  }
}

void
Sweep_inertia::add_vertical_sweep(const uint32_t x0,
				  const uint32_t y0,
				  const uint32_t x1,
				  const uint32_t y1,
				  const struct inertia_t inertia)
{
  /*
  {
    std::stringstream msg;
    msg << "[add vertical sweep: "
      "ix=" << inertia.x << ", iy=" << inertia.y << "]";
    Log::info(msg.str());
  }
  */
  const int32_t dx = x1 - x0;
  const int32_t dy = y1 - y0;
  for (uint32_t y = y0; y < y1; y++) {
    const uint32_t x = x0 + round(((double)(y - y0)) / dy * dx);
    if (x >= _width) {
      std::stringstream msg;
      msg << "Sweep_inertia::add_vertical_sweep(): "
	"x >= width: " << x << " >= " << _width <<
	"(x0=" << x0 << ", y0=" << y0 << ", y=" << y <<
	", dx=" << dx << ", dy=" << dy << ")";
      Log::fatal(msg.str());
    }
    for (int16_t thickness_index = MIN_THICKNESS_INDEX;
	 thickness_index <= MAX_THICKNESS_INDEX; thickness_index++) {
      const int32_t tx = x + thickness_index;
      if ((tx >= 0) && (tx < _width)) { 
	const double envelope = get_envelope(thickness_index);
	_inertia[y * _width + tx].x += inertia.x * envelope;
	_inertia[y * _width + tx].y += inertia.y * envelope;
      }
    }
  }
}

void
Sweep_inertia::add_sweep(const QPointF pos0,
			 const QPointF pos1,
			 const time_t delta_sec,
			 const suseconds_t delta_usec)
{
  const double time = (uint32_t)delta_sec + 0.000001 * (uint32_t)delta_usec;
  double clipped_time;
  if (time < 0.01) {
    clipped_time = 0.01;
  } else if (time > 10.0) {
    clipped_time = 10.0;
  } else {
    clipped_time = time;
  }
  const double speed = 10.0 / clipped_time;
  const int32_t sx0 = round(pos0.x());
  const int32_t sy0 = round(pos0.y());
  const int32_t sx1 = round(pos1.x());
  const int32_t sy1 = round(pos1.y());

  // clip coordinates to rectangle (0, 0, _width, _height)
  const uint32_t x0 = (sx0 > 0) ? (sx0 < _width ? sx0 : _width - 1) : 0;
  const uint32_t y0 = (sy0 > 0) ? (sy0 < _height ? sy0 : _height - 1) : 0;
  const uint32_t x1 = (sx1 > 0) ? (sx1 < _width ? sx1 : _width - 1) : 0;
  const uint32_t y1 = (sy1 > 0) ? (sy1 < _height ? sy1 : _height - 1) : 0;
  if (x0 >= _width) {
    std::stringstream msg;
    msg << "Sweep_inertia::add_sweep(): "
      "x0 >= width: " << x0 << " >= " << _width;
    Log::fatal(msg.str());
  }
  if (x1 >= _width) {
    std::stringstream msg;
    msg << "Sweep_inertia::add_sweep(): "
      "x1 >= width: " << x1 << " >= " << _width;
    Log::fatal(msg.str());
  }
  if (y0 >= _height) {
    std::stringstream msg;
    msg << "Sweep_inertia::add_sweep(): "
      "y0 >= height: " << y0 << " >= " << _height;
    Log::fatal(msg.str());
  }
  if (y1 >= _height) {
    std::stringstream msg;
    msg << "Sweep_inertia::add_sweep(): "
      "y1 >= height: " << y1 << " >= " << _height;
    Log::fatal(msg.str());
  }
  const int32_t dx = x1 - x0;
  const int32_t dy = y1 - y0;
  const double sweep_length = sqrt((double)(dx * dx + dy * dy));
  if (sweep_length > 0.0) {
    struct inertia_t inertia;
    inertia.x = speed * dx / sweep_length;
    inertia.y = speed * dy / sweep_length;
    if (abs(dx) > abs(dy)) {
      add_horizontal_sweep(x0, y0, x1, y1, inertia);
    } else {
      add_vertical_sweep(x0, y0, x1, y1, inertia);
    }
  } else {
    // zero length move => ignore
  }
}

const double
Sweep_inertia::FADE_FACTOR = 0.8;

void
Sweep_inertia::fade_step()
{
  //Log::debug("fade");
  // TODO
  //std::cout << "[sweep_fade]\n";
  //memset(_inertia, 0, _width * _height * sizeof(inertia_t));
  struct inertia_t *ptr_inertia = _inertia;
  for (int y = 0; y < _height; y++) {
    for (int x = 0; x < _width; x++) {
      ptr_inertia->x *= FADE_FACTOR;
      ptr_inertia->y *= FADE_FACTOR;
      ptr_inertia++;
    }
  }
}

const double
Sweep_inertia::get_inertia_x(const uint32_t pos_x, const uint32_t pos_y) const
{
  const double inertia = _inertia[pos_y * _width + pos_x].x;
  //_inertia[pos_y * _width + pos_x].x = 0.0;
  return inertia;
}

const double
Sweep_inertia::get_inertia_y(const uint32_t pos_x, const uint32_t pos_y) const
{
  const double inertia = _inertia[pos_y * _width + pos_x].y;
  //_inertia[pos_y * _width + pos_x].y = 0.0;
  return inertia;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */