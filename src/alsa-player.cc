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

#include <alsa-player.hh>
#include <inttypes.h>
#include <sstream>
#include <log.hh>
#include <iaudio-producer.hh>

#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_CHANNELS 2
#define AUDIO_OVERSAMPLING 1
#define ALSA_PLAYBACK_DEVICE "default" // "plughw:0,0" /* playback device */
#define ALSA_SAMPLE_RATE AUDIO_SAMPLE_RATE
#define ALSA_BUFFER_TIME 250000 /* ring buffer length in us */
#define ALSA_PERIOD_TIME 50000 /* period time in us */
#define ALSA_PERIOD_EVENT 0 /* produce poll event after each period */
#define ALSA_RESAMPLE_RATE 1

Alsa_player::Alsa_player(const double sample_scale, const bool verbose)
  : Audio_player::Audio_player()
{
  init_alsa(sample_scale, AUDIO_CHANNELS, verbose);

  _buffer_size = _period_size * _channels * 2 /* 2 -> sample size */;
  _buffer = (uint8_t *)malloc(_buffer_size);
  if (!_buffer) {
    Log::fatal("Alsa_player::Alsa_player() not enough memory");
  }

  _audio_slice = new Audio_slice(_channels, _period_size);
  if (!_audio_slice) {
    Log::fatal("Alsa_player::Alsa_player() not enough memory");
  }
}

Alsa_player::~Alsa_player()
{
  if (is_running()) {
    stop();
  }
  if (_handle) {
    snd_pcm_close(_handle);
    _handle = 0;
  }
  delete _audio_slice;
  _audio_slice = 0;
  if (_buffer) {
    free(_buffer);
    _buffer = 0;
  }
  _channels = 0;
  _period_size = 0;
  _buffer_size = 0;
  _sample_scale = 0.0;
}

void
Alsa_player::init_alsa(const double sample_scale,
		       const unsigned int channels, const bool verbose)
{
  _sample_scale = sample_scale;
  _channels = channels;

  /* Open the PCM device in playback mode */
  int err;
  if ((err = snd_pcm_open(&_handle, ALSA_PLAYBACK_DEVICE,
			  SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed opening PCM device \"" << ALSA_PLAYBACK_DEVICE <<
      "\": " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* Allocate parameters object and fill it with default values*/
  snd_pcm_hw_params_t *params;
  snd_pcm_hw_params_alloca(&params);

  snd_pcm_hw_params_any(_handle, params);

  /* set interleaved mode */
  if ((err = snd_pcm_hw_params_set_access(_handle, params,
					  SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed setting interleaved mode: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* set format to 16 bits signed PCM, little endian */
  if ((err = snd_pcm_hw_params_set_format(_handle, params,
					  SND_PCM_FORMAT_S16_LE) < 0)) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed setting format: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* set number of channels */
  if ((err = snd_pcm_hw_params_set_channels(_handle, params, _channels)) < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed setting number of channels: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* set sample rate */
  unsigned int rate = ALSA_SAMPLE_RATE;
  if ((err = snd_pcm_hw_params_set_rate_near(_handle, params, &rate, 0)) < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed setting rate: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* set buffer time */
  unsigned int buffer_time = ALSA_BUFFER_TIME;
  err = snd_pcm_hw_params_set_buffer_time_near(_handle, params,
					       &buffer_time, 0);
  if (err < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "unable to set buffer time " << buffer_time <<
      " for playback: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* set period time */
  unsigned int period_time = ALSA_PERIOD_TIME;
  err = snd_pcm_hw_params_set_period_time_near(_handle, params,
					       &period_time, 0);
  if (err < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "unable to set period time " << period_time <<
      " for playback: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* write parameters */
  if ((err = snd_pcm_hw_params(_handle, params)) < 0) {
    std::stringstream msg;
    msg << "Alsa_player::init_alsa(): " <<
      "failed setting hardware parameters: " << snd_strerror(err);
    Log::fatal(msg.str());
  }

  /* resume information */
  if (verbose) {
    {
      std::stringstream msg;
      msg << "Alsa_player::init_alsa(): ALSA device name: " <<
	snd_pcm_name(_handle);
      Log::info(msg.str());
    }
    {
      std::stringstream msg;
      msg << "Alsa_player::init_alsa(): ALSA device state: " <<
	snd_pcm_state_name(snd_pcm_state(_handle));
      Log::info(msg.str());
    }
    snd_output_t *snd_output;
    err = snd_output_stdio_attach(&snd_output, stdout, 0);
    if (err < 0) {
      std::stringstream msg;
      msg << "Alsa_player::init_alsa(): output failed: " << snd_strerror(err);
      Log::fatal(msg.str());
    }
    snd_pcm_dump(_handle, snd_output);
  }

  /* allocate buffer to hold single period */
  snd_pcm_hw_params_get_period_size(params, &_period_size, 0);
}

void
Alsa_player::reset()
{
  _audio_slice->clear();
  snd_pcm_recover(_handle, -EPIPE, true);
}

void
Alsa_player::consume()
{
  double sum_of_sample_values = 0.0;
  IAudio_producer *audio_producer = get_connect();
  audio_producer->produce(_audio_slice);

  double *source_buffer_ptr = _audio_slice->get_samples_buffer();
  uint8_t *sink_buffer_ptr = _buffer;

  if (is_muted()) {
    memset(sink_buffer_ptr, 0, _channels * _period_size * 2 * sizeof(uint8_t));
  } else {
    const double volume = get_volume();
    for (snd_pcm_uframes_t frame = 0; frame < _period_size; frame++) {
      for (unsigned int channel = 0; channel < _channels; channel++) {
	const double sample_value = (*source_buffer_ptr) * 32767.0 * volume;
	sum_of_sample_values += sample_value;
	source_buffer_ptr++;
	const int16_t pcm_sample_value = sample_value;
	*sink_buffer_ptr = (uint8_t)(pcm_sample_value & 0xff);
	sink_buffer_ptr++;
	*sink_buffer_ptr = (uint8_t)((pcm_sample_value >> 8) & 0xff);
	sink_buffer_ptr++;
      }
    }
  }

  int err;
  if ((err = snd_pcm_writei(_handle, _buffer, _period_size)) == -EPIPE) {
    Log::error("<<<<<<<<<<<<<<< ALSA buffer underrun >>>>>>>>>>>>>>>");
    snd_pcm_recover(_handle, err, true);
  } else if (err < 0) {
    std::stringstream msg;
    msg << "Alsa_player::consume(): failed writing to ALSA device: " <<
      snd_strerror(err);
    Log::error(msg.str());
    snd_pcm_recover(_handle, err, true);
  } else {
    // ok
    /*
    std::stringstream msg;
    msg << "Alsa_player::consume(): sum of sample values: " <<
      sum_of_sample_values;
    Log::info(msg.str());
    */
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
