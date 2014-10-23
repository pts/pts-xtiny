#! /bin/sh --
# by pts@fazekas.hu at Fri Oct 24 00:08:39 CEST 2014

set -ex

rm -f *.o
xtiny gcc -m32 -Os -c *.s
# This stripping shouldn't be needed, it looks small already.
strip -g --remove-section=.eh_frame --remove-section=.comment *.o
ar cr ../lib__xtiny.a *.o
ranlib ../lib__xtiny.a

: c.sh OK.
