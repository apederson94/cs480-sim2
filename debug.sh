#!/bin/bash
make -f sim02_mf clean
make -f sim02_mf
gdb sim02
