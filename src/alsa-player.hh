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

#ifndef ALSA_PLAYER_HH
#define ALSA_PLAYER_HH

#include <inttypes.h>
#include <iconfig.hh>
#include <audio-player.hh>
#include <audio-slice.hh>

class Alsa_player : public Audio_player
{
public:
  Alsa_player(const IConfig *config);
  virtual ~Alsa_player();
  void consume();
  void reset();
private:
  const IConfig *_config;
  double _sample_scale;
  snd_pcm_t *_handle;
  unsigned int _channels;
  snd_pcm_uframes_t _period_size;
  int _buffer_size;
  uint8_t *_buffer;
  Audio_slice *_audio_slice;
  void init_alsa();
};

#endif /* ALSA_PLAYER_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
