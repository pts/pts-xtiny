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

EXIT_FILES_S_IN_ORDER='
    start/__xtiny_exit.S
'

FILES_S_IN_ORDER='
    misc/__xtiny_environ.s
    misc/__xtiny_errno.s
    misc/abort.s
    misc/atexit.s
    ctype/isalpha.s
    ctype/isdigit.s
    ctype/isspace.s
    ctype/isxdigit.s
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
rm -f include xtiny

GCCDETFLAGS=-frandom-seed=9853

mkdir obj__xtiny
cd obj__xtiny
set -- $FILES_S_IN_ORDER
gcc $GCCDETFLAGS -m32 -c "${@/#/../}"
rm -f ../lib__xtiny.a
set -- "${@#*/}"
# Automatic ranlib.
# Don't include libgcc/withgot.exclude_obj/*.o, because
# they reference the _GLOBAL_OFFSET_TABLE_.
# D: for deterministic mode, don't include timestamps etc.
ar crD ../lib__xtiny.a "${@/%.*/.o}" ../libgcc/*.obj/*.o
cd ..

for VV in {i,n}{f,n}; do
  DEFINES=
  test "${VV:0:1}" = n || DEFINES="$DEFINES -DDO_INIT_ARRAY"
  test "${VV:1:1}" = n || DEFINES="$DEFINES -DDO_FINI_ARRAY"
  mkdir obj__xtiny_start_"$VV"
  cd obj__xtiny_start_"$VV"
  set -- $START_FILES_S_IN_ORDER
  gcc $GCCDETFLAGS -m32 -c $DEFINES "${@/#/../}"
  rm -f ../lib__xtiny_start_"$VV".a
  set -- "${@#*/}"
  # Automatic ranlib.
  ar crD ../lib__xtiny_start_"$VV".a "${@/%.*/.o}"
  cd ..
done

mkdir obj__xtiny_exit
cd obj__xtiny_exit
set -- $EXIT_FILES_S_IN_ORDER
gcc -m32 -c "${@/#/../}"  # No -DDO_INIT_ARRAY, no -DDO_FINI_ARRAY.
rm -f ../lib__xtiny_exit.a
set -- "${@#*/}"
ar crD ../lib__xtiny_exit.a "${@/%.*/.o}"
cd ..

ln -s ../include

cp ../xtiny ./xtiny
cd malloc
# calloc.c uses malloc.
# realloc_grow.c uses malloc and free.
C_FILES='__forward_malloc.c __forward_malloc_free.c __xtiny_lite_malloc.c calloc.c realloc_grow.c'
#   -- not production-ready yet.
set -- $C_FILES
rm -f *.o
# SUXX: The number NNNN in heap_end.NNNN may still change.
../xtiny gcc $GCCDETFLAGS -s -O2 -W -Wall -Wextra -Werror -c "$@"
ar crD ../lib__xtiny.a "${@/%.*/.o}"
cd ..

cp ../xtiny ./xtiny
cd misc
C_FILES='
    strdup.c
    strstr.c
    getenv.c
'
set -- $C_FILES
rm -f *.o
# SUXX: The number NNNN in heap_end.NNNN may still change.
../xtiny gcc $GCCDETFLAGS -s -O2 -W -Wall -Wextra -Werror -c "$@"
ar crD ../lib__xtiny.a "${@/%.*/.o}"
cd ..

set +x 
echo "Install with: cp -a src/lib__*.a ./"
: c.sh OK
