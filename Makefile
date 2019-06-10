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

include defs.mk
ROOT=.

all:
	cd html ; make all
	cd art-work ; make all
	cd icons ; make all
	cd src ; make all
	cd scripts ; make all

objclean:
	rm -rf $(BUILD)

bkpclean:
	cd html ; make bkpclean
	cd art-work ; make bkpclean
	cd icons ; make bkpclean
	cd src ; make bkpclean
	cd scripts ; make bkpclean
	rm -f *~

coreclean:
	cd html ; make coreclean
	cd art-work ; make coreclean
	cd icons ; make coreclean
	cd src ; make coreclean
	cd scripts ; make coreclean
	rm -f core core.* vgcore.*

clean: objclean

distclean: objclean bkpclean coreclean

tarball: distclean
	@TGZ_DATE=`date +%Y-%m-%d_%H-%M-%S` ; \
	PROJECT_NAME=art-frame ; \
	PROJECT_PATH=`basename \`pwd\`` ; \
	TGZ_PREFIX=$$PROJECT_NAME\_$$TGZ_DATE ; cd .. ; \
	tar cvf ./$$TGZ_PREFIX.tar.bz2 \
		--exclude=untracked_files \
		--transform=s/$$PROJECT_PATH/$$TGZ_PREFIX/ \
		--bzip2 $$PROJECT_PATH

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
