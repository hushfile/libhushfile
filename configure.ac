AC_PREREQ(2.68)
AC_INIT([hushfile], [1.0.0], [ahf@0x90.dk])
AC_CONFIG_SRCDIR([src/hushfile.c])
AC_CONFIG_AUX_DIR(config)
AC_LANG([C])

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

AC_HEADER_ASSERT
AC_HEADER_STDBOOL

AC_TYPE_SIZE_T

AC_PROG_CC_C99
if test x"$ac_cv_prog_cc_c99" = x"no" ; then
    AC_MSG_ERROR([Hushfile-CLI requires a C compiler that supports ISO C99])
fi

HUSHFILE_CFLAGS=
HUSHFILE_PC_CFLAGS=
HUSHFILE_PC_LIBS=

PKG_PROG_PKG_CONFIG([0.9.0])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

GENERATED_FILE=misc/generated.txt
AC_SUBST_FILE(GENERATED_FILE)

if test -d "${GIT_DIR:-${ac_top_srcdir:-./}/.git}" ; then
    GITHEAD=`git describe 2>/dev/null`
    if test -z "${GITHEAD}" ; then
        GITHEAD=`git rev-parse HEAD`
    fi

    if test -n "`git diff-index -m --name-only HEAD`" ; then
        GITHEAD="${GITHEAD}-dirty"
    fi
else
    GITHEAD=
fi

AC_MSG_CHECKING([for git head])
AC_ARG_WITH([git-head], AS_HELP_STRING([--with-git-head], [Specify Git head]), GITHEAD=`eval echo $withval`, GITHEAD=`eval echo $GITHEAD`)
AC_MSG_RESULT([$GITHEAD])
AC_SUBST([GITHEAD])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

TEST_CFLAGS="-pedantic -Wall -W -Wextra -Wbad-function-cast -Wcast-align -Wcast-qual -Wfloat-equal -Wformat=2 -Wformat-security -Wformat-nonliteral -Winit-self -Winline -Wlogical-op -Wmissing-prototypes -Wmissing-declarations -Wmissing-format-attribute -Wmissing-noreturn -Wpointer-arith -Wredundant-decls -Wshadow -Wswitch-default -Wunused"
for cflag in $TEST_CFLAGS ; do
    AX_CHECK_COMPILER_FLAGS([$cflag], [HUSHFILE_CFLAGS="$HUSHFILE_CFLAGS $cflag"], [])
done

AC_SUBST([HUSHFILE_CFLAGS])
AC_SUBST([HUSHFILE_PC_CFLAGS])
AC_SUBST([HUSHFILE_PC_LIBS])

AC_CONFIG_HEADER(config.h)
AC_OUTPUT(
    Makefile
    src/Makefile
)

echo
echo "Hushfile-CLI was configured successfully"
echo "----------------------------------------"

if test "x$GITHEAD" = "x" ; then
    echo "Git Revision ............. N/A"
else
    echo "Git Revision ............. $GITHEAD"
fi

echo
echo "Please run make (or gmake on FreeBSD) to build Hushfile-CLI."
