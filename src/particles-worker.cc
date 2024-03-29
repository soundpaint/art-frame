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

#include <particles-worker.hh>
#include <cmath>
#include <unistd.h>
#include <QtGui/QImage>
#include <log.hh>
#include <sweep-inertia.hh>

Particles_worker::Particles_worker(const uint16_t id,
                                   IParticles_master *master,
                                   const uint16_t row_block_x0,
                                   const uint16_t row_block_width,
                                   const uint16_t row_block_y0,
                                   const uint16_t row_block_height,
                                   const uint16_t column_block_x0,
                                   const uint16_t column_block_width,
                                   const uint16_t column_block_y0,
                                   const uint16_t column_block_height)
{
  _pid = 0;
  _id = id;

  if (!master) {
    Log::fatal("Particles_worker::Particles_worker(): "
               "master is NULL");
  }
  _master = master;

  if (row_block_x0 < 0) {
    Log::fatal("Particles_worker::Particles_worker(): row_block_x0 < 0");
  }
  _row_block_x0 = row_block_x0;

  if (row_block_width <= 0) {
    Log::fatal("Particles_worker::Particles_worker(): row_block_width <= 0");
  }
  _row_block_width = row_block_width;

  if (row_block_y0 < 0) {
    Log::fatal("Particles_worker::Particles_worker(): row_block_y0 < 0");
  }
  _row_block_y0 = row_block_y0;

  if (row_block_height <= 0) {
    Log::fatal("Particles_worker::Particles_worker(): row_block_height <= 0");
  }
  _row_block_height = row_block_height;

  if (column_block_x0 < 0) {
    Log::fatal("Particles_worker::Particles_worker(): column_block_x0 < 0");
  }
  _column_block_x0 = column_block_x0;

  if (column_block_width <= 0) {
    Log::fatal("Particles_worker::Particles_worker(): column_block_width <= 0");
  }
  _column_block_width = column_block_width;

  if (column_block_y0 < 0) {
    Log::fatal("Particles_worker::Particles_worker(): column_block_y0 < 0");
  }
  _column_block_y0 = column_block_y0;

  if (column_block_height <= 0) {
    Log::fatal("Particles_worker::Particles_worker(): column_block_height <= 0");
  }
  _column_block_height = column_block_height;

  _row_block_inertia = new double[_row_block_width * _row_block_height];
  if (!_row_block_inertia) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _row_block_remaining_momentum_x =
    new double[_row_block_width * _row_block_height];
  if (!_row_block_remaining_momentum_x) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _row_block_remaining_momentum_y =
    new double[_row_block_width * _row_block_height];
  if (!_row_block_remaining_momentum_y) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _column_block_inertia =
    new double[_column_block_width * _column_block_height];
  if (!_column_block_inertia) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _column_block_remaining_momentum_x =
    new double[_column_block_width * _column_block_height];
  if (!_column_block_remaining_momentum_x) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _column_block_remaining_momentum_y =
    new double[_column_block_width * _column_block_height];
  if (!_column_block_remaining_momentum_y) {
    Log::fatal("Particles_worker::Particles_worker(): not enough memory");
  }

  _moved_count = 0;

#if 0
  {
    std::stringstream info;
    info << "created worker[row_block_x0=" << _row_block_x0 <<
      ", row_block_width=" << _row_block_width <<
      ", row_block_y0=" << _row_block_y0 <<
      ", row_block_height=" << _row_block_height <<
      ", column_block_x0=" << _column_block_x0 <<
      ", column_block_width=" << _column_block_width <<
      ", column_block_y0=" << _column_block_y0 <<
      ", column_block_height=" << _column_block_height <<
      "]";
    Log::info(info.str());
  }
#endif
}

Particles_worker::~Particles_worker()
{
  for (uint16_t y = 0; y < _row_block_height; y++) {
    for (uint16_t x = 0; x < _row_block_width; x++) {
      const uint32_t index = y * _row_block_width + x;
      _row_block_inertia[index] = 0.0;
      _row_block_remaining_momentum_x[index] = 0.0;
      _row_block_remaining_momentum_y[index] = 0.0;
    }
  }

  delete [] _row_block_inertia;
  _row_block_inertia = 0;

  delete [] _row_block_remaining_momentum_x;
  _row_block_remaining_momentum_x = 0;

  delete [] _row_block_remaining_momentum_y;
  _row_block_remaining_momentum_y = 0;

  _row_block_x0 = 0;
  _row_block_width = 0;
  _row_block_y0 = 0;
  _row_block_height = 0;

  for (uint16_t y = 0; y < _column_block_height; y++) {
    for (uint16_t x = 0; x < _column_block_width; x++) {
      const uint32_t index = y * _column_block_width + x;
      _column_block_inertia[index] = 0.0;
      _column_block_remaining_momentum_x[index] = 0.0;
      _column_block_remaining_momentum_y[index] = 0.0;
    }
  }

  delete [] _column_block_inertia;
  _column_block_inertia = 0;

  delete [] _column_block_remaining_momentum_x;
  _column_block_remaining_momentum_x = 0;

  delete [] _column_block_remaining_momentum_y;
  _column_block_remaining_momentum_y = 0;

  _column_block_x0 = 0;
  _column_block_width = 0;
  _column_block_y0 = 0;
  _column_block_height = 0;

  _moved_count = 0;
  _id = 0;
  _master = 0;
}

void
Particles_worker::check_range_x(const uint32_t x, const char *function_id) const
{
  if (x < 0) {
    std::stringstream msg;
    msg << "Particles_worker::" << function_id << "(): "
      "x < 0";
    Log::fatal(msg.str());
  }
  if (x >= _column_block_width) {
    std::stringstream msg;
    msg << "Particles_worker::" << function_id << "(): "
      "x >= _column_block_width";
    Log::fatal(msg.str());
  }
}

void
Particles_worker::check_range_y(const uint32_t y, const char *function_id) const
{
  if (y < 0) {
    std::stringstream msg;
    msg << "Particles_worker::" << function_id << "(): "
      "y < 0";
    Log::fatal(msg.str());
  }
  if (y >= _row_block_height) {
    std::stringstream msg;
    msg << "Particles_worker::" << function_id << "(): "
      "y >= _row_block_height";
    Log::fatal(msg.str());
  }
}

const double
Particles_worker::get_remaining_momentum_x_leftmost_x(const uint32_t y) const
{
  check_range_y(y, "get_remaining_momentum_x_leftmost_x");
  return _row_block_remaining_momentum_x[y * _row_block_width];
}

const double
Particles_worker::get_remaining_momentum_y_leftmost_x(const uint32_t y) const
{
  check_range_y(y, "get_remaining_momentum_y_leftmost_x");
  return _row_block_remaining_momentum_y[y * _row_block_width];
}

const double
Particles_worker::get_remaining_momentum_x_rightmost_x(const uint32_t y) const
{
  check_range_y(y, "get_remaining_momentum_x_rightmost_x");
  return _row_block_remaining_momentum_x[(y + 1) * _row_block_width - 1];
}

const double
Particles_worker::get_remaining_momentum_y_rightmost_x(const uint32_t y) const
{
  check_range_y(y, "get_remaining_momentum_y_rightmost_x");
  return _row_block_remaining_momentum_y[(y + 1) * _row_block_width - 1];
}

const double
Particles_worker::get_remaining_momentum_x_uppermost_y(const uint32_t x) const
{
  check_range_x(x, "get_remaining_momentum_x_uppermost_y");
  return _column_block_remaining_momentum_x[x * _column_block_height];
}

const double
Particles_worker::get_remaining_momentum_y_uppermost_y(const uint32_t x) const
{
  check_range_x(x, "get_remaining_momentum_y_uppermost_y");
  return _column_block_remaining_momentum_y[x * _column_block_height];
}

const double
Particles_worker::get_remaining_momentum_x_lowermost_y(const uint32_t x) const
{
  check_range_x(x, "get_remaining_momentum_x_lowermost_y");
  return _column_block_remaining_momentum_x[(x + 1) * _column_block_height - 1];
}

const double
Particles_worker::get_remaining_momentum_y_lowermost_y(const uint32_t x) const
{
  check_range_x(x, "get_remaining_momentum_y_lowermost_y");
  return _column_block_remaining_momentum_y[(x + 1) * _column_block_height - 1];
}

const double
Particles_worker::get_inertia_leftmost_x(const uint32_t y) const
{
  check_range_y(y, "get_inertia_leftmost_x");
  return _row_block_inertia[y * _row_block_width];
}

const double
Particles_worker::get_inertia_rightmost_x(const uint32_t y) const
{
  check_range_y(y, "get_inertia_rightmost_x");
  return _row_block_inertia[(y + 1) * _row_block_width - 1];
}

void
Particles_worker::set_inertia_leftmost_x(const uint32_t y,
                                         const double inertia)
{
  check_range_y(y, "set_inertia_leftmost_x");
  _row_block_inertia[y * _row_block_width] = inertia;
}

void
Particles_worker::set_inertia_rightmost_x(const uint32_t y,
                                          const double inertia)
{
  check_range_y(y, "set_inertia_rightmost_x");
  _row_block_inertia[(y + 1) * _row_block_width - 1] = inertia;
}

const double
Particles_worker::get_inertia_uppermost_y(const uint32_t x) const
{
  check_range_x(x, "get_inertia_uppermost_y");
  return _column_block_inertia[x * _column_block_height];
}

const double
Particles_worker::get_inertia_lowermost_y(const uint32_t x) const
{
  check_range_x(x, "get_inertia_lowermost_y");
  return _column_block_inertia[(x + 1) * _column_block_height - 1];
}

void
Particles_worker::set_inertia_uppermost_y(const uint32_t x,
                                          const double inertia)
{
  check_range_x(x, "set_inertia_uppermost_y");
  _column_block_inertia[x * _column_block_height] = inertia;
}

void
Particles_worker::set_inertia_lowermost_y(const uint32_t x,
                                          const double inertia)
{
  check_range_x(x, "set_inertia_lowermost_y");
  _column_block_inertia[(x + 1) * _column_block_height - 1] = inertia;
}

const uint16_t
Particles_worker::get_id() const
{
  return _id;
}

const uint16_t
Particles_worker::get_row_block_x0() const
{
  return _row_block_x0;
}

const uint16_t
Particles_worker::get_row_block_width() const
{
  return _row_block_width;
}

const uint16_t
Particles_worker::get_row_block_y0() const
{
  return _row_block_y0;
}

const uint16_t
Particles_worker::get_row_block_height() const
{
  return _row_block_height;
}

const uint16_t
Particles_worker::get_column_block_x0() const
{
  return _column_block_x0;
}

const uint16_t
Particles_worker::get_column_block_width() const
{
  return _column_block_width;
}

const uint16_t
Particles_worker::get_column_block_y0() const
{
  return _column_block_y0;
}

const uint16_t
Particles_worker::get_column_block_height() const
{
  return _column_block_height;
}

void
Particles_worker::worker_loop()
{
  std::stringstream msg;
  msg << "worker " << _id << ": deploy columns " <<
    _row_block_x0 << ".." <<
    (_row_block_x0 + _row_block_width - 1) << ", lines " <<
    _row_block_y0 << ".." <<
    (_row_block_y0 + _row_block_height - 1) << " and columns " <<
    _column_block_x0 << ".." <<
    (_column_block_x0 + _column_block_width - 1) << ", lines " <<
    _column_block_y0 << ".." <<
    (_column_block_y0 + _column_block_height - 1);
  Log::info(msg.str());

  for (;;) {
    // wait until image has been flushed to video device
    _master->await_flush_completed(_id);

    do_work();

    // wait until all work is done
    _master->await_work_completed(_id);
  }
}

void *
Particles_worker::worker_thread(void *arg)
{
  Particles_worker *this_worker = (Particles_worker *)arg;
  this_worker->_pid = getpid();
  this_worker->worker_loop();
  return 0;
}

void
Particles_worker::create_thread()
{
  const int result =
    pthread_create(&(_worker_thread),
                   NULL,
                   (void * (*)(void *))worker_thread,
                   this);
  if (result) {
    std::stringstream msg;
    msg << "Particles_worker::create_thread(): "
      "worker #" << _id << ": failed creating thread: " << result;
    Log::fatal(msg.str());
  }
}

const pid_t
Particles_worker::get_pid() const
{
  return _pid;
}

void
Particles_worker::row_block_init_particles()
{
  const QImage *image = _master->get_image();
  const Sweep_inertia *sweep_inertia = _master->get_sweep_inertia();
  if (image) {
    for (uint16_t y = 0; y < _row_block_height; y++) {
      const uint16_t image_y = _row_block_y0 + y;
      for (uint16_t x = 0; x < _row_block_width; x++) {
        const uint16_t image_x = _row_block_x0 + x;
        const QRgb pixel = image->pixel(image_x, image_y);
        const double inertium = 1.0 + ((double)qGray(pixel)) / 256.0;
        const double sweep_x = sweep_inertia->get_inertia_x(image_x, image_y);
        const double sweep_y = sweep_inertia->get_inertia_y(image_x, image_y);
        const uint32_t index = y * _row_block_width + x;
        _row_block_inertia[index] = inertium;
        _row_block_remaining_momentum_x[index] = sweep_x;
        _row_block_remaining_momentum_y[index] = sweep_y;
      }
    }
  } else {
    // no pixmap loaded so far => nothing to initialize
  }
}

void
Particles_worker::column_block_init_particles()
{
  const QImage *image = _master->get_image();
  const Sweep_inertia *sweep_inertia = _master->get_sweep_inertia();
  if (image) {
    for (uint16_t y = 0; y < _column_block_height; y++) {
      const uint16_t image_y = _column_block_y0 + y;
      for (uint16_t x = 0; x < _column_block_width; x++) {
        const uint16_t image_x = _column_block_x0 + x;
        const QRgb pixel = image->pixel(image_x, image_y);
        const double inertium = 1.0 + ((double)qGray(pixel)) / 256.0;
        const double sweep_x = sweep_inertia->get_inertia_x(image_x, image_y);
        const double sweep_y = sweep_inertia->get_inertia_y(image_x, image_y);
        const uint32_t index = y * _column_block_width + x;
        _column_block_inertia[index] = inertium;
        _column_block_remaining_momentum_x[index] = sweep_x;
        _column_block_remaining_momentum_y[index] = sweep_y;
      }
    }
  } else {
    // no pixmap loaded so far => nothing to initialize
  }
}

void
Particles_worker::init_particles()
{
  row_block_init_particles();
  column_block_init_particles();
}

inline void
Particles_worker::row_block_swap_pixels_x(const int32_t x1, const int32_t y1,
                                          const int32_t dx,
                                          const int32_t index1,
                                          const int32_t index2)
{
  const int32_t x2 = std::max(0, std::min(x1 + dx, _row_block_width - 1));
  const double swap_remaining_momentum_x =
    _row_block_remaining_momentum_x[index2] - dx;
  _row_block_remaining_momentum_x[index2] =
    _row_block_remaining_momentum_x[index1] - dx;
  _row_block_remaining_momentum_x[index1] = swap_remaining_momentum_x;
  QImage *image = _master->get_image();
  const QRgb swap_pixel = image->pixel(x1, y1);
  image->setPixel(x1, y1, image->pixel(x2, y1));
  image->setPixel(x2, y1, swap_pixel);
  const double swap_inertia = _row_block_inertia[index1];
  _row_block_inertia[index1] = _row_block_inertia[index2];
  _row_block_inertia[index2] = swap_inertia;
}

inline void
Particles_worker::row_block_swap_pixels_y(const int32_t x1, const int32_t y1,
                                          const int32_t dy,
                                          const int32_t index1,
                                          const int32_t index2)
{
  const int32_t y2 = std::max(0, std::min(y1 + dy, _row_block_height - 1));
  const double swap_remaining_momentum_y =
    _row_block_remaining_momentum_y[index2] - dy;
  _row_block_remaining_momentum_y[index2] =
    _row_block_remaining_momentum_y[index1] - dy;
  _row_block_remaining_momentum_y[index1] = swap_remaining_momentum_y;
  QImage *image = _master->get_image();
  const QRgb swap_pixel = image->pixel(x1, y1);
  image->setPixel(x1, y1, image->pixel(x1, y2));
  image->setPixel(x1, y2, swap_pixel);
  const double swap_inertia = _row_block_inertia[index1];
  _row_block_inertia[index1] = _row_block_inertia[index2];
  _row_block_inertia[index2] = swap_inertia;
}

inline void
Particles_worker::column_block_swap_pixels_x(const int32_t x1, const int32_t y1,
                                             const int32_t dx,
                                             const int32_t index1,
                                             const int32_t index2)
{
  const int32_t x2 = std::max(0, std::min(x1 + dx, _column_block_width - 1));
  const double swap_remaining_momentum_x =
    _column_block_remaining_momentum_x[index2] - dx;
  _column_block_remaining_momentum_x[index2] =
    _column_block_remaining_momentum_x[index1] - dx;
  _column_block_remaining_momentum_x[index1] = swap_remaining_momentum_x;
  QImage *image = _master->get_image();
  const QRgb swap_pixel = image->pixel(x1, y1);
  image->setPixel(x1, y1, image->pixel(x2, y1));
  image->setPixel(x2, y1, swap_pixel);
  const double swap_inertia = _column_block_inertia[index1];
  _column_block_inertia[index1] = _column_block_inertia[index2];
  _column_block_inertia[index2] = swap_inertia;
}

inline void
Particles_worker::column_block_swap_pixels_y(const int32_t x1, const int32_t y1,
                                             const int32_t dy,
                                             const int32_t index1,
                                             const int32_t index2)
{
  const int32_t y2 = std::max(0, std::min(y1 + dy, _column_block_height - 1));
  const double swap_remaining_momentum_y =
    _column_block_remaining_momentum_y[index2] - dy;
  _column_block_remaining_momentum_y[index2] =
    _column_block_remaining_momentum_y[index1] - dy;
  _column_block_remaining_momentum_y[index1] = swap_remaining_momentum_y;
  QImage *image = _master->get_image();
  const QRgb swap_pixel = image->pixel(x1, y1);
  image->setPixel(x1, y1, image->pixel(x1, y2));
  image->setPixel(x1, y2, swap_pixel);
  const double swap_inertia = _column_block_inertia[index1];
  _column_block_inertia[index1] = _column_block_inertia[index2];
  _column_block_inertia[index2] = swap_inertia;
}

void
Particles_worker::update()
{
  // combined_block_update();
  row_block_update();
  column_block_update();
  // row_block_update_fast();
  // column_block_update_fast();
}

void
Particles_worker::row_block_update()
{
  row_block_init_particles();
  const double dx = /* 1.2598e-322 * */ sin(+1.0 * _master->get_ax());

  for (uint16_t y = 0; y < _row_block_height; y++) {
    int32_t index1 = y * _row_block_width;
    for (uint16_t x = 0; x < _row_block_width; x++) {
      _row_block_remaining_momentum_x[index1] +=
        _row_block_inertia[index1] * dx;
      index1++;
    }
  }

  uint32_t moved_count = 0;
  for (int32_t y1 = 0; y1 < _row_block_height; y1++) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0;
    int32_t index2 = index1 + 1;
    int32_t index3 = index1 + _row_block_width;
    for (int32_t x1 = 0; x1 < _row_block_width - 1; x1++) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      if ((_row_block_remaining_momentum_x[index2] > remaining_momentum_x1) &&
          (remaining_momentum_x1 > 0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                _gravity, index1, index2);
        moved_count++;
      }
      index1++;
      index2++;
      index3++;
    }
  }
  for (int32_t y1 = _row_block_height - 1; y1 >= 0; y1--) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0 + _row_block_width - 1;
    int32_t index2 = index1 - 1;
    int32_t index3 = index1 - _row_block_width;
    for (int32_t x1 = _row_block_width - 1; x1 > 0; x1--) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      if ((_row_block_remaining_momentum_x[index2] < remaining_momentum_x1) &&
          (remaining_momentum_x1 < -0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                -_gravity, index1, index2);
        moved_count++;
      }
      index1--;
      index2--;
      index3--;
    }
  }
  _moved_count = 0.2 * moved_count + 0.8 * _moved_count;
}

void
Particles_worker::column_block_update()
{
  column_block_init_particles();
  const double dy = sin(-1.0 * _master->get_ay());

  for (uint16_t y = 0; y < _column_block_height; y++) {
    int32_t index1 = y * _column_block_width;
    for (uint16_t x = 0; x < _column_block_width; x++) {
      _column_block_remaining_momentum_y[index1] +=
        _column_block_inertia[index1] * dy;
      index1++;
    }
  }

  uint32_t moved_count = 0;
  for (int32_t y1 = 0; y1 < _column_block_height - 1; y1++) {
    const int32_t index0 = y1 * _column_block_width;
    int32_t index1 = index0;
    int32_t index2 = index1 + _column_block_width;
    int32_t index3 = index1 + 1;
    for (int32_t x1 = 0; x1 < _column_block_width; x1++) {
      const double remaining_momentum_y1 =
        _column_block_remaining_momentum_y[index1];
      if ((_column_block_remaining_momentum_y[index2] >
           remaining_momentum_y1) &&
          (remaining_momentum_y1 > 0.0)) {
        column_block_swap_pixels_y(_column_block_x0 + x1, _column_block_y0 + y1,
                                   _gravity, index1, index2);
        moved_count++;
      }
      index1++;
      index2++;
      index3++;
    }
  }
  for (int32_t y1 = _column_block_height - 1; y1 > 0; y1--) {
    const int32_t index0 = y1 * _column_block_width;
    int32_t index1 = index0 + _column_block_width - 1;
    int32_t index2 = index1 - _column_block_width;
    int32_t index3 = index1 - 1;
    for (int32_t x1 = _column_block_width - 1; x1 >= 0; x1--) {
      const double remaining_momentum_y1 =
        _column_block_remaining_momentum_y[index1];
      if ((_column_block_remaining_momentum_y[index2] <
           remaining_momentum_y1) &&
          (remaining_momentum_y1 < -0.0)) {
        column_block_swap_pixels_y(_column_block_x0 + x1, _column_block_y0 + y1,
                                   -_gravity, index1, index2);
        moved_count++;
      }
      index1--;
      index2--;
      index3--;
    }
  }
  _moved_count = 0.2 * moved_count + 0.8 * _moved_count;
}

void
Particles_worker::row_block_update_fast()
{
  const QImage *image = _master->get_image();
  if (!image) {
    return;
  }

  uint32_t moved_count = 0;
  const double dx = tan(+0.75 * _master->get_ax());

  for (uint16_t y = 0; y < _row_block_height; y++) {
    int32_t index = y * _row_block_width;
    for (uint16_t x = 0; x < _row_block_width; x++) {
      const QRgb pixel = image->pixel(_row_block_x0 + x, _row_block_y0 + y);
      const double inertium = 1.0 + ((double)qGray(pixel)) / 256.0;
      _row_block_remaining_momentum_x[index] = inertium * dx;
      index++;
    }
  }

  // TODO

  /*
  for (int32_t y1 = 0; y1 < _row_block_height; y1++) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0;
    int32_t index2 = index1 + 1;
    int32_t index3 = index1 + _row_block_width;
    for (int32_t x1 = 0; x1 < _row_block_width - 1; x1++) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      if ((_row_block_remaining_momentum_x[index2] > remaining_momentum_x1) &&
          (remaining_momentum_x1 > 0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                _gravity, index1, index2);
        moved_count++;
      }
      index1++;
      index2++;
      index3++;
    }
  }
  for (int32_t y1 = _row_block_height - 1; y1 >= 0; y1--) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0 + _row_block_width - 1;
    int32_t index2 = index1 - 1;
    int32_t index3 = index1 - _row_block_width;
    for (int32_t x1 = _row_block_width - 1; x1 > 0; x1--) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      if ((_row_block_remaining_momentum_x[index2] < remaining_momentum_x1) &&
          (remaining_momentum_x1 < -0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                -_gravity, index1, index2);
        moved_count++;
      }
      index1--;
      index2--;
      index3--;
    }
  }
  */
  _moved_count = 0.2 * moved_count + 0.8 * _moved_count;
}

void
Particles_worker::column_block_update_fast()
{
  // TODO
}

void
Particles_worker::combined_block_update()
{
  const double dx = tan(-0.75 * _master->get_ax());
  const double dy = tan(0.75 * _master->get_ay());

  for (uint16_t y = 0; y < _row_block_height; y++) {
    int32_t index1 = y * _row_block_width;
    for (uint16_t x = 0; x < _row_block_width; x++) {
      _row_block_remaining_momentum_x[index1] +=
        _row_block_inertia[index1] * dx;
      _row_block_remaining_momentum_y[index1] +=
        _row_block_inertia[index1] * dy;
      index1++;
    }
  }

  uint32_t moved_count = 0;
  for (int32_t y1 = 0; y1 < _row_block_height - 1; y1++) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0;
    int32_t index2 = index1 + 1;
    int32_t index3 = index1 + _row_block_width;
    for (int32_t x1 = 0; x1 < _row_block_width - 1; x1++) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      const double remaining_momentum_y1 =
        _row_block_remaining_momentum_y[index1];
      if ((_row_block_remaining_momentum_x[index2] > remaining_momentum_x1) &&
          (remaining_momentum_x1 > 0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                _gravity, index1, index2);
        moved_count++;
      }
      if ((_row_block_remaining_momentum_y[index3] > remaining_momentum_y1) &&
          (remaining_momentum_y1 > 0.0)) {
        row_block_swap_pixels_y(_row_block_x0 + x1, _row_block_y0 + y1,
                                _gravity, index1, index3);
        moved_count++;
      }
      index1++;
      index2++;
      index3++;
    }
  }
  for (int32_t y1 = _row_block_height - 1; y1 > 0; y1--) {
    const int32_t index0 = y1 * _row_block_width;
    int32_t index1 = index0 + _row_block_width - 1;
    int32_t index2 = index1 - 1;
    int32_t index3 = index1 - _row_block_width;
    for (int32_t x1 = _row_block_width - 1; x1 > 0; x1--) {
      const double remaining_momentum_x1 =
        _row_block_remaining_momentum_x[index1];
      const double remaining_momentum_y1 =
        _row_block_remaining_momentum_y[index1];
      if ((_row_block_remaining_momentum_x[index2] < remaining_momentum_x1) &&
          (remaining_momentum_x1 < -0.0)) {
        row_block_swap_pixels_x(_row_block_x0 + x1, _row_block_y0 + y1,
                                -_gravity, index1, index2);
        moved_count++;
      } else if ((_row_block_remaining_momentum_y[index3] <
                  remaining_momentum_y1) &&
          (remaining_momentum_y1 < -0.0)) {
        row_block_swap_pixels_y(_row_block_x0 + x1, _row_block_y0 + y1,
                                -_gravity, index1, index3);
        moved_count++;
      }
      index1--;
      index2--;
      index3--;
    }
  }
#if 0
  {
    std::stringstream msg;
    msg << "worker #" << _id << ": moved " << moved_count << " pixels";
    Log::info(msg.str());
  }
#endif
  _moved_count = 0.2 * moved_count + 0.8 * _moved_count;
}

const uint32_t
Particles_worker::get_moved_count() const
{
  return _moved_count;
}

void
Particles_worker::do_work()
{
  update();
}

const int8_t
Particles_worker::get_gravity() const
{
  return _gravity;
}

void
Particles_worker::set_gravity(const int8_t gravity)
{
  _gravity = gravity;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
