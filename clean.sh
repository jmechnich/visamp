#!/bin/sh

rm -rf aclocal.m4 autom4te.cache compile config.guess config.sub configure depcomp install-sh ltmain.sh m4 missing
find . -name Makefile.in -delete
find . -name '*~' -delete
