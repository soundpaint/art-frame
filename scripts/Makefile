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

include ../defs.mk

ROOT=..
BUILD_BIN=$(BUILD)/bin
SRC_SH=$(wildcard *.sh)
OBJ_SH=$(patsubst %,$(BUILD_BIN)/%,$(SRC_SH))
OBJ=$(OBJ_SH)

all: $(OBJ)

$(BUILD_BIN):
	mkdir -p $@

$(BUILD_BIN)/%.sh: %.sh | $(BUILD_BIN)
	cp -pf $< $@

coreclean:
	rm -f core core.* vgcore.*

objclean:
	rm -f $(OBJ)

bkpclean:
	rm -f *~

clean: objclean bkpclean

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
