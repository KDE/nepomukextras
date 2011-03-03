#! /usr/bin/env bash
$EXTRACTRC *.ui gui/*.ui >> rc.cpp
$XGETTEXT *.cpp -o $podir/nepomukextras.pot
