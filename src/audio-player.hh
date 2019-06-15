/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2005, 2006, 2014, 2015, 2016, 2019 Jürgen Reuter
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

#ifndef AUDIO_PLAYER_HH
#define AUDIO_PLAYER_HH

#include <pthread.h>
#include <itransport-control.hh>
#include <iaudio-producer.hh>

struct audio_thread_info_t;

class Audio_player : public ITransport_control
{
public:
  Audio_player();
  virtual ~Audio_player();
  void connect_to(IAudio_producer *audio_producer);
  IAudio_producer *get_connect() const;
  virtual void consume() = 0;
  virtual void reset() = 0;
  void resume();
  void pause();
  const bool is_running() const;
  void mute();
  void unmute();
  const bool is_muted() const;
  void set_volume(const double volume);
  const double get_volume() const;
protected:
  double _volume;
private:
  struct audio_thread_info_t {
    Audio_player *audio_player;
    bool pause_req;
  };
  IAudio_producer *_audio_producer;
  pthread_t _audio_thread;
  pthread_mutex_t _serialize_pause_resume;
  audio_thread_info_t *_audio_thread_info;
  bool _is_muted;
  static void *loop(audio_thread_info_t *arg);
};

#endif /* AUDIO_PLAYER_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
