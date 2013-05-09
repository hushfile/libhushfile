#!/bin/sh
# vim: set sw=4 sts=4 et tw=80 :

changelog() {
    echo ">>> Generating ChangeLog" 1>&2
    git log > ChangeLog
}

run() {
    echo ">>> $@" 1>&2

    if ! $@ ; then
        echo "Error!" 1>&2
        exit 1
    fi
}

changelog
run mkdir -p config
run aclocal -I m4
run autoconf
run autoheader
run automake --add-missing --copy
