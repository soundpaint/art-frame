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

#ifndef PARTICLES_WORKER_HH
#define PARTICLES_WORKER_HH

#include <iparticles-master.hh>
#include <inttypes.h>

class Particles_worker
{
public:
  Particles_worker(const uint16_t id,
		   IParticles_master *master,
		   const uint16_t row_block_x0,
		   const uint16_t row_block_width,
		   const uint16_t row_block_y0,
		   const uint16_t row_block_height,
		   const uint16_t column_block_x0,
		   const uint16_t column_block_width,
		   const uint16_t column_block_y0,
		   const uint16_t column_block_height);
  virtual ~Particles_worker();
  const pid_t get_pid() const;
  const uint16_t get_id() const;
  const uint16_t get_row_block_x0() const;
  const uint16_t get_row_block_width() const;
  const uint16_t get_row_block_y0() const;
  const uint16_t get_row_block_height() const;
  const uint16_t get_column_block_x0() const;
  const uint16_t get_column_block_width() const;
  const uint16_t get_column_block_y0() const;
  const uint16_t get_column_block_height() const;
  const double get_remaining_momentum_x_leftmost_x(const uint32_t y) const;
  const double get_remaining_momentum_y_leftmost_x(const uint32_t y) const;
  const double get_remaining_momentum_x_rightmost_x(const uint32_t y) const;
  const double get_remaining_momentum_y_rightmost_x(const uint32_t y) const;
  const double get_inertia_rightmost_x(const uint32_t y) const;
  const double get_inertia_leftmost_x(const uint32_t y) const;
  void set_inertia_rightmost_x(const uint32_t y, const double inertia);
  void set_inertia_leftmost_x(const uint32_t y, const double inertia);
  const double get_remaining_momentum_x_uppermost_y(const uint32_t x) const;
  const double get_remaining_momentum_y_uppermost_y(const uint32_t x) const;
  const double get_remaining_momentum_x_lowermost_y(const uint32_t x) const;
  const double get_remaining_momentum_y_lowermost_y(const uint32_t x) const;
  const double get_inertia_lowermost_y(const uint32_t x) const;
  const double get_inertia_uppermost_y(const uint32_t x) const;
  void set_inertia_lowermost_y(const uint32_t x, const double inertia);
  void set_inertia_uppermost_y(const uint32_t x, const double inertia);
  void init_particles();
  void update();
  void create_thread();
  void worker_loop();
  const uint32_t get_moved_count() const;
private:
  pthread_t _worker_thread;
  pid_t _pid;
  uint16_t _id;
  IParticles_master *_master;
  uint16_t _row_block_x0;
  uint16_t _row_block_width;
  uint16_t _row_block_y0;
  uint16_t _row_block_height;
  uint16_t _column_block_x0;
  uint16_t _column_block_width;
  uint16_t _column_block_y0;
  uint16_t _column_block_height;
  double *_row_block_inertia;
  double *_row_block_remaining_momentum_x;
  double *_row_block_remaining_momentum_y;
  double *_column_block_inertia;
  double *_column_block_remaining_momentum_x;
  double *_column_block_remaining_momentum_y;
  uint32_t _moved_count;
  static void *worker_thread(void *arg);
  void row_block_swap_pixels_x(const int32_t x1, const int32_t y1,
			       const int32_t dx,
			       const int32_t index1, const int32_t index2);
  void row_block_swap_pixels_y(const int32_t x1, const int32_t y1,
			       const int32_t dy,
			       const int32_t index1, const int32_t index2);
  void column_block_swap_pixels_x(const int32_t x1, const int32_t y1,
				  const int32_t dx,
				  const int32_t index1, const int32_t index2);
  void column_block_swap_pixels_y(const int32_t x1, const int32_t y1,
				  const int32_t dy,
				  const int32_t index1, const int32_t index2);
  void check_range_x(const uint32_t x, const char *function_id) const;
  void check_range_y(const uint32_t y, const char *function_id) const;
  void row_block_init_particles();
  void column_block_init_particles();
  void do_work();
  void combined_block_update();
  void row_block_update();
  void column_block_update();
  void row_block_update_fast();
  void column_block_update_fast();
};

#endif /* PARTICLES_WORKER_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
