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

SH = sh -c
CPP = c++
MOC = moc
MY_CXX_WARNINGS = -Wall #-Wextra # -Wconversion
MY_CXX_OPTS = -std=c++11 $(MY_CXX_WARNINGS) -g -O0 $(LOCAL_CXX_OPTS)
MY_LD_OPTS = $(LOCAL_LD_OPTS)

ADDITIONAL_GENERATED_FILES = RTIMULib.ini

$(MY_BIN_FILES): $(MY_OBJ_FILES) $(MY_MOC_FILES) | $(BUILD_BIN)
	$(CPP) $(MY_CXX_OPTS) $(MY_INCLUDE_DIRS) $(LIB_OBJ_FILES) $(MY_OBJ_FILES) $(MY_LD_OPTS) $(MY_LIB_DIRS) $(MY_LIBS) -o $@

$(BUILD_OBJ)/%.o: %.cc | $(BUILD_OBJ)
	$(CPP) $(MY_INCLUDE_DIRS) $(MY_CXX_OPTS) -c -o $@ $<

$(BUILD_OBJ)/%.moc.o: $(BUILD_SRC)/%.moc.cc | $(BUILD_OBJ)
	$(CPP) $(MY_INCLUDE_DIRS) $(MY_CXX_OPTS) -c -o $@ $<

common:
	$(SH) "if [ `basename \`pwd\`` != "common" ] ; then (cd ../common ; make) ; fi"

objclean:
	rm -f $(MY_OBJ_FILES) $(MY_BIN_FILES) $(MY_MOC_FILES) \
		$(ADDITIONAL_GENERATED_FILES)

bkpclean:
	rm -f *~

coreclean:
	rm -f core core.* vgcore.*

clean: objclean

distclean: objclean bkpclean coreclean

tarball: distclean
	@TGZ_DATE=`date +%Y-%m-%d_%H-%M-%S` ; \
	PROJECT_NAME=art-frame ; \
	PROJECT_PATH=`basename \`pwd\`` ; \
	TGZ_PREFIX=$$PROJECT_NAME\_$$TGZ_DATE ; cd .. ; \
	tar cvf ./$$TGZ_PREFIX.tar.bz2 \
		--exclude=tmp \
		--transform=s/$$PROJECT_PATH/$$TGZ_PREFIX/ \
		--bzip2 $$PROJECT_PATH

.SECONDARY: $(MY_OBJ_FILES) $(MY_MOC_FILES)

#  Local Variables:
#    coding:utf-8
#    mode:Makefile
#  End:
