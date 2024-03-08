#!/bin/bash
os=$(uname -s)
case $os in
    Linux*)
        netstat -t --numeric-ports | grep localhost ; netstat -tl --numeric-ports
        ;;
    Darwin*)
        lsof -iTCP -P | grep -E '8080|8002'
        ;;
    *)
        echo "unknown os"
        exit 1
        ;;
esac