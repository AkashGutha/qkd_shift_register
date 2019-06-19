#!/bin/sh
sopc-create-header-files \
"$PWD/soc_system.sopcinfo" \
--single hps.h \
--module hps
