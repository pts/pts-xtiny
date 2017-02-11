#define DUMMY /*
true <<'#if 00' #*/
/*
 * multi_trampoline.c: a chdir() + [chroot() + umask()] + setreuid() + execve() for a CGI, possibly setuid
 * by pts@fazekas.hu at Fri Jun  9 12:09:32 CEST 2006
 * pts-xtiny version at Sat Feb 11 09:17:41 CET 2017
 *
 * multi_trampoline.c is a parametric C program, which, when compiled,
 * executes a real program (usually a CGI script) in a specially prepared a
 * UNIX environment. The envioronment is specified when multi_trampoline.c
 * is compiled. The following aspects of the UNIX environment can be modified:
 * removal of environment variables from environ,
 * UID, GID, EUID, EGID, umask(), chdir(), chroot() and argv[0].
 * The argv[] coming from the
 * invocation command line is preserved upon execution, except for argv[0]
 * which is replaced. It is safe to add a setuid/setgid bits of the binary
 * of multi_trampoline.c. initgroups() and setgroups() are not called,
 * because this works only as root. multi_trampoline.c contains
 * optimizations for Linux i386, so the generated binary will be statically
 * linked and very small.
 *
 * This is version multi_trampoline.c version 0.04. See also VERSION below.
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Usage: ./multi_trampoline.c <option> ...
 *
 * Example:
 *
 *   ./multi_trampoline.c --command=/usr/bin/svnserve --target=svnserve_trampoline --umask=002 --setids=real --chroot=/var/chroot/svn --chdir=/var/chroot/svn
 *
 * Options:
 *
 * --target=<file>  the trampoline binary to be created
 * --command=<prog>  command to execute, $PATH won't be consulted
 * --argv0=<str>  argv[0] to set (default: <prog> of --command)
 * --chdir=<dir>  chdir() (before, and maybe after chroot() and setuids)
 * --chdir2=<dir>  chdir() (after chroot() and setuids)
 * --chroot=<dir>  chroot()
 * --umask=<mask>  umask(); dec, hex or oct
 *   --umask=0022 is for disabling group write access on newly created files
 *   --umask=0002 is for enabling  group write access on newly created files
 * --setids=real  EGID:=GID; EUID:=UID;
 * --setids=effective  GID:=EGID; UID=EUID;
 * --setids=<uid>,<gid>  EGID:=GID:=<gid>; UID:=GID:=<uid>;
 *  Specified ids can be numeric or names.
 * --onlyenv=<spec> comma-separated list of environment variables to keep,
 *   e.g. PATH=,HTTP_
 * 
 * Dat: needs bash or zsh (or ksh?), doesn't work with (t)csh
 * Dat: EARGV translation above is quoting metachars with backslash:
 *      perl -le 'print join(",",map{s@["\\\n]@"\\".($&eq"\n"?"n":$&)@ge;"\"$_\""}@ARGV)' -- a b c
 *
 */

#ifdef __XTINY__
#include <xtiny.h>
#else
#define _BSD_SOURCE 1 /* setregid(), setreuid() */
#include <unistd.h> /* execve(), setregid(), setreuid(), chroot() */
#include <stdlib.h> /* exit() */
#include <sys/stat.h> /* umask */
extern char** environ;
#endif

int main(int argc, char**argv) {
#ifdef A_CHDIR
  int z;
#endif
  (void)argc; (void)argv;
#ifdef U_UMASK
  umask(U_UMASK); /* for multi_trampoline.c */
#endif
  /* ^^^ Dat: for setgid dirs in CVS and SVN */
#ifdef A_CHDIR
  z=chdir(A_CHDIR);
#endif
#ifdef A_CHROOT
  if (z!=0) return 125; /* Dat: A_CHROOT implies A_CHDIR */
  if (0!=chroot(A_CHROOT)) return 124;
#endif
#if U_SETIDS
#if U_SETIDS==1
  setregid(getegid(),getegid()); /* Dat: group ID first */
  setreuid(geteuid(),geteuid());
#endif
#if U_SETIDS==2
  setregid(getgid(),getgid()); /* Dat: group ID first */
  setreuid(getuid(),getuid());
#endif
#if U_SETIDS==3
  /* Imp: maybe initgroups() */
  setregid(U_SET_GID,U_SET_GID); /* Dat: group ID first */
  setreuid(U_SET_UID,U_SET_UID);
#endif
#ifdef A_CHDIR
#ifndef A_CHDIR2
  if (z!=0) z=chdir(A_CHDIR);
#else
  if (z!=0) z=chdir(A_CHDIR2);
#endif
#endif
#endif /* U_SETIDS */
#ifdef A_CHDIR
  if (z!=0) return 125;
#endif
#ifdef A_CHDIR2
  if (0!=chdir(A_CHDIR2)) return 123;
#endif
#ifdef A_ONLYENV
  { /* Dat: we modify envinron in place: we remove some vars */
    char const *p;
    char const *q;
    char const *e;
    char **ep, **ew;
    for (ep=ew=environ; *ep!=0/*NULL*/; ep++) {
      p=A_ONLYENV; /* comma-separated list of environment variables (ending with =) or prefixes to keep */	
      while (1) { /* Dat: now p is the next entry-part to match */
        while (*p==',') p++;
        if (*p=='\0') break;
        q=p;
        while (*p!=',' && *p!='\0') p++;
        /* Dat: now env prefix is q...p */
        e=*ep;
        while (*q!='=' && *q!=',' && *q!='\0' && *q==*e) { q++; e++; }
        if ((*q=='=' && *e=='=') || *q=='\0' || *q==',') { *ew++=*ep; break; } /* match => keep env var */
      }
    }
    *ew=0/*NULL*/;
  }
#endif
  argv[0]=A_ARGV0; /* for multi_trampoline.c */
  execve(A_COMMAND,argv,environ);
  return 126;
} /*
#if 00
set -o shwordsplit 2>/dev/null # Dat: for zsh
export \
VERSION="0.04";
echo "This is multi_trampoline.c v$VERSION by pts@fazekas.hu
The license is GNU GPL >=2.0. It comes without warranty. USE AT YOUR OWN RISK!" >&2
CARGS="`perl -e 'for (@ARGV) {
    if (/^--(target|command|chdir|chdir2|chroot|argv0|onlyenv)=(.*)/s) { $H{$1}=$2 }
    elsif (/^--(umask)=(0[xX][\da-fA-F]+|0[0-7]*|[1-9]\d*)/s) { $U{$1}=$2 }
    elsif (/^--(setids)=real\Z(?!\n)/) { $U{$1}=2 }
    elsif (/^--(setids)=effective\Z(?!\n)/) { $U{$1}=1 }
    elsif (/^--(setids)=(.*?),(.*)\Z(?!\n)/) {
      my @u=(0,0,$2); my @g=(0,0,$3);
      @u=getpwnam($u[2]) if $u[2]=~/\D/; die if !$u[2];
      @g=getgrnam($g[2]) if $g[2]=~/\D/; die if !$g[2];
      $U{set_uid}=0+$u[2]; $U{set_gid}=0+$g[2]; $U{setids}=3;
    }
    else { die "invalid option: $_\n" }
  }
  die "missing --command=\n" if !defined $H{command};
  die "missing --target=\n" if !defined $H{target};
  die "missing --chdir= before --chroot\n" if !defined $H{chdir} and
      defined $H{chroot};
  $H{argv0}=$H{command} if !defined $H{argv0};
  %CQ=("\\\""=>"\\\"","\\\\"=>"\\\\","\r"=>"r","\n"=>"n","\0"=>"0","\t"=>"t",
    " "=>"\x20");
  sub csq($) { my $S=$_[0]; $S=~s@(["\r\n\t\0 ])@\\\\$CQ{$1}@g; "\\\"$S\\\"" }
  print "-o ".($H{target})."\n"; delete $H{target}; # Imp: better than csq?
  for (sort keys%H) { print "-DA_\U$_\E=".csq($H{$_})."\n" }
  for (sort keys%U) { print "-DU_\U$_\E=$U{$_}\n" }
' -- "$@"`"
test "$CARGS" || exit 2
WFLAGS="-Wall -W -Wstrict-prototypes -Wnested-externs"
WFLAGS="$WFLAGS -Winline -Wpointer-arith -Wbad-function-cast -Wcast-qual"
WFLAGS="$WFLAGS -Wmissing-prototypes -Wmissing-declarations"
CFLAGS="$WFLAGS -DNDEBUG=1 -s -Os -ansi"
set -x; umask 022
xtiny gcc $CARGS $CFLAGS "$0" ||
      gcc $CARGS $CFLAGS "$0" || exit 3
echo "Binary created OK" >&2
#endif */
