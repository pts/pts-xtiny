#! /bin/bash --
# by pts@fazekas.hu at Sun Jan 29 19:54:21 CET 2017
#
# If we ever need .c files: gcc -nostdlib -nostartfiles -nodefaultlibs
#

# It's important that _start.s is immediately followed by __xtiny_exit.s.
START_FILES_S_IN_ORDER='
    start/_start.S
    start/__xtiny_exit.S
'

FILES_S_IN_ORDER='
    misc/__xtiny_environ.s
    misc/__xtiny_errno.s
    misc/abort.s
    misc/atexit.s
    string/memccpy.s
    string/memchr.s
    string/memcmp.s
    string/memcpy.s
    string/memset.s
    string/strcasecmp.s
    string/strcat.s
    string/strchr.s
    string/strcmp.s
    string/strcpy.s
    string/strlen.s
    string/strncmp.s
    string/strncpy.s
    string/strrchr.s
'
set -ex
cd "${0%/*}"
test -f string/memset.s
rm -rf obj obj__* lib__*.a */*.o

mkdir obj__xtiny
cd obj__xtiny
set -- $FILES_S_IN_ORDER
gcc -m32 -c "${@/#/..\/}"
rm -f ../lib__xtiny.a
set -- "${@#*/}"
# Automatic ranlib.
# Don't include libgcc/withgot.exclude_obj/*.o, because
# they reference the _GLOBAL_OFFSET_TABLE_.
ar cr ../lib__xtiny.a "${@/%.*/.o}" ../libgcc/*.obj/*.o
cd ..

for VV in {i,n}{f,n}; do
  DEFINES=
  test "${VV:0:1}" = n || DEFINES="$DEFINES -DDO_INIT_ARRAY"
  test "${VV:1:1}" = n || DEFINES="$DEFINES -DDO_FINI_ARRAY"
  mkdir obj__xtiny_start_"$VV"
  cd obj__xtiny_start_"$VV"
  set -- $START_FILES_S_IN_ORDER
  gcc -m32 -c $DEFINES "${@/#/..\/}"
  rm -f ../lib__xtiny_start_"$VV".a
  set -- "${@#*/}"
  # Automatic ranlib.
  ar cr ../lib__xtiny_start_"$VV".a "${@/%.*/.o}"
  cd ..
done

set +x 
echo "Install with: cp -a src/lib__*.a ./"
: c.sh OK
