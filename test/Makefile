# ************************************************************* -*- Makefile -*-
#
# Copyright (C) 2004-2015 Andreas Huggel <ahuggel@gmx.net>
#
# This Makefile is part of the Exiv2 distribution.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#    1. Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#    2. Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#    3. The name of the author may not be used to endorse or promote
#       products derived from this software without specific prior
#       written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# File:      Makefile
# Author(s): Andreas Huggel (ahu) <ahuggel@gmx.net>
# History:   26-Dec-04, ahu: created
#
# Description:
#  Simple makefile to run the Exiv2 unit tests
#
# Restrictions:
#  Only tested with GNU make.
#

# Default make target
all: test

top_srcdir = ..

# Initialisations
SHELL = /bin/sh

.SUFFIXES:

# ******************************************************************************
# Targets
.PHONY: all test testv teste mostlyclean clean distclean maintainer-clean

##
# to save download time for simple build/test, we download video and eps data on demand
SVN    = svn://dev.exiv2.org/svn/testdata/trunk

#  ## Sadly on SunOS make terminates saying "unexpected end of line"
#  # determine the test platform
#  UNAME := $(shell uname)
#  UNIX=FALSE
#  ifeq ($(UNAME),SunOS)
#  UNIX=TRUE
#  endif
#  ifeq ($(UNAME),FreeBSD)
#  UNIX=TRUE
#  endif

##
# Add test drivers to this list
TESTS1 = addmoddel.sh         \
         conversions.sh       \
         geotag-test.sh       \
         icc-test.sh          \
         iso65k-test.sh       \
         modify-test.sh       \
         path-test.sh         \
         stringto-test.sh     \
         webp-test.sh         \
         write2-test.sh       \

# the following tests are broken on UNIX
# You can manually run all the tests: `cd <exiv2dir>/test ; env EXIV2_BINDIR=$PWD/../build/bin make test`
# You can manually run a single test: `cd <exiv2dir>/test ; env EXIV2_BINDIR=$PWD/../build/bin ./iotest.sh`
# Or: make iotest
TESTS2 = exifdata-test.sh     \
         exiv2-test.sh        \
         imagetest.sh         \
         iotest.sh            \
         iptctest.sh          \
         preview-test.sh      \
         tiff-test.sh         \
         write-test.sh        \
         xmpparser-test.sh

# video tests
TESTV  = video-test.sh
TESTVW = write-video-test.sh

# EPS tests
TESTE  = eps-test.sh          \
         eps-preview-test.sh

# webready tests
TESTX  = httpiotest.sh        \
         curliotest.sh        \
         sshiotest.sh

addmoddel     \
conversions   \
geotag-test   \
icc-test      \
iso65k-test   \
modify-test   \
path-test     \
stringto-test \
webp-test     \
write2-test   \
exifdata-test \
exiv2-test    \
imagetest     \
iotest        \
iptctest      \
preview-test  \
tiff-test     \
write-test    \
xmpparser-test:
	@echo
	@echo ---- Running test: $@.sh ----
	@echo
	./$@.sh

version_test \
unit_test    :
	-@./$@.sh

bash_tests:
	-if [ -e $$EXIV2_BINDIR/../Makefile ]; then \
	    -@./$@.sh ; \
	else \
	    make alltest ; \
	fi

tests:
	-if [ -e $$EXIV2_BINDIR/../Makefile ]; then \
	    -@./$@.sh ; \
	else \
	    make unit_test ; make alltest ; make python_tests ; make version_test ; \
	fi


alltest:
	@echo
	@echo ---- Running bash tests: ALL ----
	@echo
	mkdir -p tmp
	@rm -rf tmp/test-failed
	@list='$(TESTS1) $(TESTS2)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p;  \
	    rc=$$?; \
	    if [ $$rc -ne 0 ]; then echo '***' ; echo '***' $$p result = $$rc ; echo '***'       ; fi ; \
	    if [ $$rc -ne 0 ]; then              echo '***' $$p result = $$rc >> tmp/test-failed ; fi ; \
	done
	@if [ -e tmp/test-failed ]; then echo ; echo '***' FAILED ; cat tmp/test-failed ; echo '***' ; fi

unixtest:
	@echo
	@echo ---- Running bash tests: UNIX subset ----
	@echo
	mkdir -p tmp
	@rm -rf tmp/test-failed
	@list='$(TESTS1)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p;  \
	    rc=$$?; \
	    if [ $$rc -ne 0 ]; then echo '***' ; echo '***' $$p result = $$rc ; echo '***'       ; fi ; \
	    if [ $$rc -ne 0 ]; then              echo '***' $$p result = $$rc >> tmp/test-failed ; fi ; \
	done
	@if [ -e tmp/test-failed ]; then echo '***' FAILED ; cat tmp/test-failed ; echo '***' ; fi

python_tests:
	-( cd ../tests ; python3 runner.py --verbose )


testv:
	@for t in /video ; do \
		if [ ! -e data/$$t ]; then \
	echo svn export '$(SVN)'$$t data$$t -r 3100 ; \
	     svn export '$(SVN)'$$t data$$t -r 3100 ; \
	fi ; done
	@list='$(TESTV)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p; \
	done

testvw:
	@for t in /video ; do \
		if [ ! -e data/$$t ]; then \
	echo svn export '$(SVN)'$$t data$$t ; \
	     svn export '$(SVN)'$$t data$$t ; \
	fi ; done
	@list='$(TESTVW)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p; \
	done

teste:
	@for t in /eps ; do \
		if [ ! -e data/$$t ]; then \
	echo svn export '$(SVN)'$$t data$$t ; \
	     svn export '$(SVN)'$$t data$$t ; \
	fi ; done
	@list='$(TESTE)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p; \
	    rc=$$?; \
	    if [ $$rc -ne 0 ]; then \
		echo result = $$rc ; \
		exit $$rc ; \
		fi ; \
	done

testx:
	@list='$(TESTX)'; for p in $$list; do \
	    echo Running $$p ...; \
	    ./$$p; \
	done

mostlyclean clean:
	rm -rf $(top_srcdir)/test/tmp/*

distclean: clean
	$(RM) *~ *.bak *#

# This command is intended for maintainers to use; it deletes files
# that may need special tools to rebuild.
maintainer-clean: distclean
	rm -rf $(top_srcdir)/data/video $(top_srcdir)/test/data/eps

# That's all Folks!
##
