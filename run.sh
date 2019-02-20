#!/bin/bash
make -f sim02_mf clean
make -f sim02_mf
./sim02 config0.cnf
