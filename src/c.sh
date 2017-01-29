#! /bin/bash --
# by pts@fazekas.hu at Sun Jan 29 19:54:21 CET 2017
FILES_S_IN_ORDER='
    start/__xtiny_environ.s
    start/__xtiny_errno.s
    start/_start.s
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
set -- $FILES_S_IN_ORDER
set -ex
cd "${0%/*}"
test -f string/memset.s
rm -rf obj
mkdir obj
cd obj
gcc -m32 -c "${@/#/..\/}"
rm -f lib__xtiny.a
set -- "${@#*/}"
# Automatic ranlib.
ar cr lib__xtiny.a "${@/%.*/.o}"
cd ..
set +x 
echo "Install with: cp -a src/obj/lib__xtiny.a ./"
: c.sh OK
