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

#ifndef PARTICLES_HH
#define PARTICLES_HH

#include <sensors.hh>
#include <iaudio-producer.hh>
#include <iparticles-master.hh>
#include <particles-worker.hh>
#include <iconfig.hh>
#include <config-image.hh>
#include <audio-player.hh>
#include <cpu-status.hh>
#include <iparticles-change-listener.hh>

class Particles : public IAudio_producer, public IParticles_master
{
public:
  Particles(const uint16_t width,
	    const uint16_t height,
	    const IConfig *config,
            const Sensors *sensors,
            const Cpu_status *cpu_status,
	    const uint16_t num_threads);
  virtual ~Particles();
  void set_change_listener(IParticles_change_listener *change_listener);
  const Sweep_inertia *get_sweep_inertia() const;
  void init_image();
  QImage *get_image() const;
  const double get_pitch() const;
  const double get_roll() const;
  const double get_ax() const;
  const double get_ay() const;
  void await_flush_completed(const int worker_id);
  void await_work_completed(const int worker_id);
  void load_image(const Config_image *image);
  void reset();
  void update();
  virtual void produce(Audio_slice *audio_slice);
  void handle_sweep(const QPointF pos0, const QPointF pos1,
		    const time_t delta_sec, const suseconds_t delta_usec);
  void sweep_fade_step();
private:
  const IConfig *_config;
  uint16_t _width;
  uint16_t _height;
  const Sensors *_sensors;
  const Cpu_status *_cpu_status;
  uint16_t _num_threads;
  IParticles_change_listener *_particles_change_listener;
  Audio_player *_audio_player;
  const QPixmap *_pixmap;
  QImage *_image;
  double *_inertia;
  Sweep_inertia *_sweep_inertia;
  double *_remaining_momentum_x;
  double *_remaining_momentum_y;
  uint32_t _moved_count;
  Particles_worker **_worker;
  pthread_barrier_t _flush_completed_barrier;
  pthread_barrier_t _work_completed_barrier;
  void swap_pixels_x(const int32_t x1, const int32_t y1, const int32_t dx,
		     const int32_t index1, const int32_t index2);
  void swap_pixels_y(const int32_t x1, const int32_t y1, const int32_t dy,
		     const int32_t index1, const int32_t index2);
  void spawn_workers();
  void setup_process_affinity();
  void update_row_block_borders();
  void update_column_block_borders();
  void single_threaded_init_particles();
  void single_threaded_update_moved_count(const uint32_t moved_count);
  void single_threaded_update();
  void multi_threaded_init_particles();
  void multi_threaded_update_moved_count();
  void multi_threaded_update();
  void frame_usleep() const;
};

#endif /* PARTICLES_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
