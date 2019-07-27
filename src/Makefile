# art-frame -- an artful sands image emulation
#
# Copyright (C) 2016, 2019 Jürgen Reuter
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see
# <https://www.gnu.org/licenses/>.
#
# For updates and more info or contacting the author, visit:
# <https://github.com/soundpaint/art-frame>
#
# Author's web site: www.juergen-reuter.de

ROOT=..

include ../defs.mk
include qt5.mk
include rtimu.mk

SH = sh -c
CPP = c++
MY_CXX_OPTS = -std=c++11 -Wall -g -O0 $(LOCAL_CXX_OPTS)
MY_LD_OPTS = $(LOCAL_LD_OPTS)

LOCAL_CXX_OPTS = \
  -fPIC

LOCAL_LD_OPTS = \
  -fPIC $(MY_MOC_FILES)

MY_BIN_FILES = $(BUILD_BIN)/art-frame

MY_QT5_OBJ_FILES = \
  activity-monitor.o audio-control.o about-dialog.o art-frame.o \
  frame-display.o global-control.o image-control.o \
  license-dialog.o main-window.o sensors.o sensors-display.o \
  simulation.o simulation-control.o simulation-pause-monitor.o \
  simulation-execution-monitor.o status-line.o \
  thermal-display.o thermal-sensors.o titled-button.o titled-dial.o

MY_MOC_FILES = $(patsubst %.o,$(BUILD)/obj/%.moc.o,$(MY_QT5_OBJ_FILES))

MY_OBJ_FILES = \
  $(patsubst %.o,$(BUILD_OBJ)/%.o, \
  abstract-config-reader.o alsa-player.o audio-player.o \
  audio-slice.o config.o config-image.o config-image-browser.o \
  config-reader.o fractals-factory.o key-bindings.o log.o \
  particles.o particles-worker.o sweep-inertia.o xml-node-list.o \
  $(MY_QT5_OBJ_FILES))

LIB_OBJ_FILES =

MY_INCLUDE_DIRS = \
  -I$(QT5_INCLUDE_DIR) \
  -I$(RTIMULIB_INCLUDE_DIR) \
  -I$(BUILD_SRC) \
  -I.

MY_LIB_DIRS =

MY_LIBS = \
  -lpthread -lusb -lX11 -lasound -lxerces-c -lQt5Core -lQt5Widgets -lQt5Gui -lm -lRTIMULib

CONFIG_XML=$(BUILD_BIN)/config.xml
CONFIG_XSD=$(BUILD_BIN)/config.xsd
STYLE_QSS=$(BUILD_BIN)/style.qss

all: $(CONFIG_XML) $(CONFIG_XSD) $(STYLE_QSS) $(MY_BIN_FILES)

$(BUILD_SRC):
	mkdir -p $@

$(BUILD_OBJ):
	mkdir -p $@

$(BUILD_BIN):
	mkdir -p $@

$(CONFIG_XML): $(SRC)/config.xml
	cp -pf $< $@

$(CONFIG_XSD): $(SRC)/config.xsd
	cp -pf $< $@

$(STYLE_QSS): $(SRC)/style.qss
	cp -pf $< $@

$(BUILD_SRC)/%.moc.cc: %.hh | $(BUILD_SRC)
	$(MOC) -I. -o $@ $<

$(BUILD_OBJ)/about-dialog.o: $(BUILD_SRC)/ABOUT.h

$(BUILD_OBJ)/license-dialog.o: $(BUILD_SRC)/COPYING.h

include common.mk

.SECONDARY: #keep intermediate files

.SUFFIXES:

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
