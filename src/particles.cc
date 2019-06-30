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

#include <particles.hh>
#include <QtGui/QPainter>
#include <log.hh>

#define MULTI_THREADED 1

Particles::Particles(const uint16_t width,
                     const uint16_t height,
                     const IConfig *config,
                     const Sensors *sensors,
                     const Cpu_status *cpu_status,
                     const uint16_t num_threads)
{
  if (width <= 0) {
    Log::fatal("Particles::Particles(): width <= 0");
  }
  _width = width;

  if (height <= 0) {
    Log::fatal("Particles::Particles(): height <= 0");
  }
  _height = height;

  if (!config) {
    Log::fatal("Particles::Particles(): config is NULL");
  }
  _config = config;

  if (!sensors) {
    Log::fatal("Particles::Particles(): sensors is NULL");
  }
  _sensors = sensors;

  if (!cpu_status) {
    Log::fatal("Particles::Particles(): cpu_status is NULL");
  }
  _cpu_status = cpu_status;

  _particles_change_listener = 0;

  if (num_threads <= 0) {
    Log::fatal("Particles::Particles(): num_threads <= 0");
  }
  _num_threads = num_threads;

  _pixmap = 0;

  _image = new QImage(_width, _height, QImage::Format_RGB32);
  if (!_image) {
    Log::fatal("Particles::Particles(): not enough memory");
  }

  _inertia = new double[_width * _height];
  if (!_inertia) {
    Log::fatal("Particles::Particles(): not enough memory");
  }

  _sweep_inertia = new Sweep_inertia(_width, _height, _config);
  if (!_sweep_inertia) {
    Log::fatal("Particles::Particles(): not enough memory");
  }

  _remaining_momentum_x = new double[_width * _height];
  if (!_remaining_momentum_x) {
    Log::fatal("Particles::Particles(): not enough memory");
  }

  _remaining_momentum_y = new double[_width * _height];
  if (!_remaining_momentum_y) {
    Log::fatal("Particles::Particles(): not enough memory");
  }

  _moved_count = 0;

  pthread_barrier_init(&_flush_completed_barrier, NULL,
                       _num_threads + 1);
  pthread_barrier_init(&_work_completed_barrier, NULL,
                       _num_threads + 1);

  _worker = (Particles_worker **)calloc(sizeof(Particles_worker), _num_threads);
  if (!_worker) {
    Log::fatal("not enough memory");
  }
  spawn_workers();
  setup_process_affinity();
}

Particles::~Particles()
{
  pthread_barrier_destroy(&_flush_completed_barrier);
  pthread_barrier_destroy(&_work_completed_barrier);

  // Q objects will be deleted by Qt, just set them to 0
  _pixmap = 0;
  _image = 0;

  for (uint16_t y = 0; y < _height; y++) {
    for (uint16_t x = 0; x < _width; x++) {
      _inertia[y * _width + x] = 0.0;
      _remaining_momentum_x[y * _width + x] = 0.0;
      _remaining_momentum_y[y * _width + x] = 0.0;
    }
  }

  delete _sweep_inertia;
  _sweep_inertia = 0;

  delete [] _inertia;
  _inertia = 0;

  delete [] _remaining_momentum_x;
  _remaining_momentum_x = 0;

  delete [] _remaining_momentum_y;
  _remaining_momentum_y = 0;

  _moved_count = 0;

  for (uint16_t index = 0; index < _num_threads; index++) {
    delete _worker[index];
    _worker[index] = 0;
  }
  free(_worker);
  _worker = 0;

  _particles_change_listener = 0;
  _cpu_status = 0;
  _sensors = 0;
  _config = 0;
  _width = 0;
  _height = 0;
}

void
Particles::set_change_listener(IParticles_change_listener *change_listener)
{
  if (!change_listener) {
    Log::fatal("Particles::set_change_listener(): change_listener is NULL");
  }
  _particles_change_listener = change_listener;
}

const Sweep_inertia *
Particles::get_sweep_inertia() const
{
  return _sweep_inertia;
}

void
Particles::spawn_workers()
{
  Log::info("spawning worker threads...");
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  for (uint16_t index = 0; index < _num_threads; index++) {
    const uint16_t x0 = x1;
    const uint16_t y0 = y1;
    x1 = (index * _width + _width) / _num_threads;
    y1 = (index * _height + _height) / _num_threads;
    uint16_t column_block_width = x1 - x0;
    uint16_t row_block_height = y1 - y0;
    Particles_worker *worker =
      new Particles_worker(index, this,
                           0, _width,
                           y0, row_block_height,
                           x0, column_block_width,
                           0, _height);
    if (!worker) {
      Log::fatal("Particles::spawn_workers(): not enough memory");
    }
    worker->create_thread();
    _worker[index] = worker;
  }
}

void
Particles::setup_process_affinity()
{
  Log::info("setting process affinities...");
  for (uint16_t index = 0; index < _num_threads; index++) {
    const Particles_worker *worker = _worker[index];
    while (!worker->get_pid()) {
      std::stringstream msg;
      msg << "worker #" << worker->get_id() <<
        ": not yet running => sleeping for a short while...";
      Log::info(msg.str());
      sleep(1); // ensure thread has started
    }
    const pid_t pid = worker->get_pid();

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(index, &mask);
    const int result = sched_setaffinity(pid, sizeof(cpu_set_t), &mask);
    if (result) {
      const int errsv = errno;
      std::stringstream msg;
      msg << "worker #" << worker->get_id() <<
        ": failed setting process affinity: " << strerror(errsv);
      Log::fatal(msg.str());
    }
  }
  Log::info("process affinities now set for all threads");
}

void
Particles::await_flush_completed(const int worker_id)
{
#if DEBUG_BARRIER
  {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " waiting for flush phase to complete";
    Log::info(msg.str());
  }
#endif

  const int retval =
    pthread_barrier_wait(&_flush_completed_barrier);
  if ((retval != 0) && (retval != PTHREAD_BARRIER_SERIAL_THREAD)) {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " failed to wait for flush phase to complete";
    Log::fatal(msg.str());
  }

#if DEBUG_BARRIER
  {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " entering work phase";
    Log::info(msg.str());
  }
#endif

  if (worker_id < 0) {
    // control thread: re-new barrier for next cycle
    pthread_barrier_destroy(&_flush_completed_barrier);
    pthread_barrier_init(&_flush_completed_barrier, NULL,
                         _num_threads + 1);
  }
}

void
Particles::await_work_completed(const int worker_id)
{
#if DEBUG_BARRIER
  {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " waiting for work phase to complete";
    Log::info(msg.str());
  }
#endif

  const int retval =
    pthread_barrier_wait(&_work_completed_barrier);
  if ((retval != 0) && (retval != PTHREAD_BARRIER_SERIAL_THREAD)) {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " failed to wait for work phase to complete";
    Log::fatal(msg.str());
  }

#if DEBUG_BARRIER
  {
    std::stringstream msg;
    if (worker_id >= 0) {
      msg << "worker #" << worker_id;
    } else {
      msg << "control thread";
    }
    msg << " entering flush phase";
    Log::info(msg.str());
  }
#endif

  if (worker_id < 0) {
    // control thread: re-new barrier for next cycle
    pthread_barrier_destroy(&_work_completed_barrier);
    pthread_barrier_init(&_work_completed_barrier, NULL,
                         _num_threads + 1);
  }
}

void
Particles::single_threaded_init_particles()
{
  for (uint16_t y = 0; y < _height; y++) {
    for (uint16_t x = 0; x < _width; x++) {
      const QRgb pixel = _image->pixel(x, y);
      const double inertium = 1.0 + ((double)qGray(pixel)) / 256.0;
      _inertia[y * _width + x] = inertium;
      _remaining_momentum_x[y * _width + x] = 0.0;
      _remaining_momentum_y[y * _width + x] = 0.0;
    }
  }
}

void
Particles::multi_threaded_init_particles()
{
  for (uint16_t index = 0; index < _num_threads; index++) {
    _worker[index]->init_particles();
  }
}

void
Particles::load_image(const Config_image *image)
{
  if (!image) {
    Log::fatal("Particles::load_image(): image is NULL");
  }
  if (_pixmap) {
    delete _pixmap;
  }
  _pixmap = image->create_pixmap();
  reset();
}

QImage *
Particles::get_image() const
{
  return _image;
}

void
Particles::reset()
{
  if (_pixmap) {
    QPainter painter(_image);
    painter.fillRect(0, 0, _width, _height, Qt::white);
    painter.drawPixmap(0, 0, _width, _height, *_pixmap);
    painter.end();
#if MULTI_THREADED
  multi_threaded_init_particles();
#else
  single_threaded_init_particles();
#endif
  } else {
    // no pixmap loaded so far => nothing to reset
  }
}

inline void
Particles::swap_pixels_x(const int32_t x1, const int32_t y1, const int32_t dx,
                         const int32_t index1, const int32_t index2)
{
  const double swap_remaining_momentum_x = _remaining_momentum_x[index2] - dx;
  _remaining_momentum_x[index2] = _remaining_momentum_x[index1] - dx;
  _remaining_momentum_x[index1] = swap_remaining_momentum_x;
  const QRgb swap_pixel = _image->pixel(x1, y1);
  _image->setPixel(x1, y1, _image->pixel(x1 + dx, y1));
  _image->setPixel(x1 + dx, y1, swap_pixel);
  const double swap_inertia = _inertia[index1];
  _inertia[index1] = _inertia[index2];
  _inertia[index2] = swap_inertia;
}

inline void
Particles::swap_pixels_y(const int32_t x1, const int32_t y1, const int32_t dy,
                         const int32_t index1, const int32_t index2)
{
  const double swap_remaining_momentum_y = _remaining_momentum_y[index2] - dy;
  _remaining_momentum_y[index2] = _remaining_momentum_y[index1] - dy;
  _remaining_momentum_y[index1] = swap_remaining_momentum_y;
  const QRgb swap_pixel = _image->pixel(x1, y1);
  _image->setPixel(x1, y1, _image->pixel(x1, y1 + dy));
  _image->setPixel(x1, y1 + dy, swap_pixel);
  const double swap_inertia = _inertia[index1];
  _inertia[index1] = _inertia[index2];
  _inertia[index2] = swap_inertia;
}

void
Particles::multi_threaded_update_moved_count()
{
  uint64_t sum_moved_count = 0;
  for (uint16_t index = 0; index < _num_threads; index++) {
    const Particles_worker *worker = _worker[index];
    sum_moved_count += worker->get_moved_count();
  }
  _moved_count = sum_moved_count / _num_threads;
}

void
Particles::update_row_block_borders()
{
  uint32_t moved_count = 0;
  for (uint16_t index = 1; index < _num_threads; index++) {
    const uint32_t x1 = _worker[index]->get_row_block_x0();
    const uint32_t x0 = x1 - 1;
    for (uint32_t y0 = 0; y0 < _height; y0++) {
      const double remaining_momentum_x0 =
        _worker[index - 1]->get_remaining_momentum_x_rightmost_x(y0);
      const double remaining_momentum_x1 =
        _worker[index]->get_remaining_momentum_x_leftmost_x(y0);
      if (remaining_momentum_x1 > remaining_momentum_x0) {
        // swap inertia
        const double inertia0 = _worker[index - 1]->get_inertia_rightmost_x(y0);
        const double inertia1 = _worker[index]->get_inertia_leftmost_x(y0);
        _worker[index - 1]->set_inertia_rightmost_x(y0, inertia1);
        _worker[index]->set_inertia_leftmost_x(y0, inertia0);

        // swap pixel
        const QRgb swap_pixel = _image->pixel(x0, y0);
        _image->setPixel(x0, y0, _image->pixel(x1, y0));
        _image->setPixel(x1, y0, swap_pixel);

        moved_count++;
      }
    }
  }
}

void
Particles::update_column_block_borders()
{
  uint32_t moved_count = 0;
  for (uint16_t index = 1; index < _num_threads; index++) {
    const uint32_t y1 = _worker[index]->get_column_block_y0();
    const uint32_t y0 = y1 - 1;
    for (uint32_t x0 = 0; x0 < _height; x0++) {
      const double remaining_momentum_y0 =
        _worker[index - 1]->get_remaining_momentum_y_lowermost_y(x0);
      const double remaining_momentum_y1 =
        _worker[index]->get_remaining_momentum_y_uppermost_y(x0);
      if (remaining_momentum_y1 > remaining_momentum_y0) {
        // swap inertia
        const double inertia0 = _worker[index - 1]->get_inertia_lowermost_y(x0);
        const double inertia1 = _worker[index]->get_inertia_uppermost_y(x0);
        _worker[index - 1]->set_inertia_lowermost_y(x0, inertia1);
        _worker[index]->set_inertia_uppermost_y(x0, inertia0);

        // swap pixel
        const QRgb swap_pixel = _image->pixel(x0, y0);
        _image->setPixel(x0, y0, _image->pixel(x0, y1));
        _image->setPixel(x0, y1, swap_pixel);

        moved_count++;
      }
    }
  }
}

void
Particles::single_threaded_update_moved_count(const uint32_t moved_count)
{
  _moved_count = 0.2 * moved_count + 0.8 * _moved_count;
}

void
Particles::single_threaded_update()
{
  const double dx = _sensors->get_ax();
  const double dy = _sensors->get_ay();

  for (uint16_t y = 0; y < _height; y++) {
    int32_t index1 = y * _width;
    for (uint16_t x = 0; x < _width; x++) {
      _remaining_momentum_x[index1] += _inertia[index1] * dx;
      _remaining_momentum_y[index1] += _inertia[index1] * dy;
      index1++;
    }
  }

  uint32_t moved_count = 0;
  for (int32_t y1 = 0; y1 < _height - 1; y1++) {
    const int32_t index0 = y1 * _width;
    int32_t index1 = index0;
    int32_t index2 = index1 + 1;
    int32_t index3 = index1 + _width;
    for (int32_t x1 = 0; x1 < _width - 1; x1++) {
      const double remaining_momentum_x1 = _remaining_momentum_x[index1];
      const double remaining_momentum_y1 = _remaining_momentum_y[index1];
      if ((_remaining_momentum_x[index2] > remaining_momentum_x1) &&
          (remaining_momentum_x1 > 0.0)) {
        swap_pixels_x(x1, y1, +1, index1, index2);
        moved_count++;
      }
      if ((_remaining_momentum_y[index3] > remaining_momentum_y1) &&
          (remaining_momentum_y1 > 0.0)) {
        swap_pixels_y(x1, y1, +1, index1, index3);
        moved_count++;
      }
      index1++;
      index2++;
      index3++;
    }
  }
  for (int32_t y1 = _height - 1; y1 > 0; y1--) {
    const int32_t index0 = y1 * _width;
    int32_t index1 = index0 + _width - 1;
    int32_t index2 = index1 - 1;
    int32_t index3 = index1 - _width;
    for (int32_t x1 = _width - 1; x1 > 0; x1--) {
      const double remaining_momentum_x1 = _remaining_momentum_x[index1];
      const double remaining_momentum_y1 = _remaining_momentum_y[index1];
      if ((_remaining_momentum_x[index2] < remaining_momentum_x1) &&
          (remaining_momentum_x1 < -0.0)) {
        swap_pixels_x(x1, y1, -1, index1, index2);
        moved_count++;
      } else if ((_remaining_momentum_y[index3] < remaining_momentum_y1) &&
          (remaining_momentum_y1 < -0.0)) {
        swap_pixels_y(x1, y1, -1, index1, index3);
        moved_count++;
      }
      index1--;
      index2--;
      index3--;
    }
  }
  single_threaded_update_moved_count(moved_count);
}

void
Particles::multi_threaded_update()
{
  await_flush_completed(-1);
  await_work_completed(-1);

  // collect moved_count values from workers
  multi_threaded_update_moved_count();

  //update_row_block_borders();
  //update_column_block_borders();
}

void
Particles::update()
{
#if MULTI_THREADED
  multi_threaded_update();
#else
  single_threaded_update();
#endif
  if (_particles_change_listener) {
    _particles_change_listener->particles_changed();
  } else {
    // listener not yet registered
  }
  frame_usleep();
}

void
Particles::frame_usleep() const
{
  const double start_cooling_break_temperature =
    _config->get_start_cooling_break_temperature();
  const double stop_cooling_break_temperature =
    _config->get_stop_cooling_break_temperature();
  const double cooling_break_interval =
    start_cooling_break_temperature - stop_cooling_break_temperature;
  const uint32_t frame_usleep_min = _config->get_frame_usleep_min();
  const uint32_t frame_usleep_max = _config->get_frame_usleep_max();
  const double vc_temperature = _cpu_status->get_vc_temperature();
  const double approximity =
    cooling_break_interval > 0 ?
    (vc_temperature - stop_cooling_break_temperature) /
    (cooling_break_interval) :
    0.5;
  const double bound_approximity =
    approximity > 1.0 ? 1.0 : (approximity < 0.0 ? 0.0 : approximity);
  const useconds_t time =
    (int)(frame_usleep_min +
          bound_approximity * (frame_usleep_max - frame_usleep_min));
  usleep(time);
}

const double
Particles::get_activity_level() const
{
  return (0.5 * _moved_count) / (_width * _height);
}

void
Particles::produce(Audio_slice *audio_slice)
{
  if (!audio_slice) {
    Log::fatal("Particles::produce(): audio_slice is NULL");
  }
  const snd_pcm_uframes_t period_size = audio_slice->get_period_size();
  const unsigned int channels = audio_slice->get_channels();
  const double volume = (0.5 * _moved_count) / (_width * _height);
  double *sample_ptr = audio_slice->get_samples_buffer();
  for (unsigned int channel = 0; channel < channels; channel++) {
    for (snd_pcm_uframes_t frame = 0; frame < period_size; frame++) {
      *sample_ptr = (((double)rand()) / RAND_MAX - 0.5) * volume;
      sample_ptr++;
    }
  }
}

const double
Particles::get_pitch() const
{
  return _sensors->get_pitch();
}

const double
Particles::get_roll() const
{
  return _sensors->get_roll();
}

const double
Particles::get_ax() const
{
  return _sensors->get_ax();
}

const double
Particles::get_ay() const
{
  return _sensors->get_ay();
}

void
Particles::handle_sweep(const QPointF pos0,
                        const QPointF pos1,
                        const time_t delta_sec,
                        const suseconds_t delta_usec)
{
  _sweep_inertia->add_sweep(pos0, pos1, delta_sec, delta_usec);
}

void
Particles::sweep_fade_step()
{
  _sweep_inertia->fade_step();
}

const int8_t
Particles::get_gravity() const
{
  return _gravity;
}

void
Particles::set_gravity(const int8_t gravity)
{
  if ((gravity < -32) || (gravity > 31)) {
    Log::fatal("Particles::set_gravity(): gravity out of range");
  }
  _gravity = gravity;
  for (uint16_t index = 0; index < _num_threads; index++) {
    Particles_worker *worker = _worker[index];
    worker->set_gravity(gravity);
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
