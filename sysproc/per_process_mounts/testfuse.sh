#!/bin/sh

mkdir -p ./fusemount_test 

echo "[$$] ---- testfuse.sh ----> MOUNTING" >>namespace.txt
./hello_fuse ./fusemount_test >> namespace.txt

ls -la ./fusemount_test >> namespace.txt
cat ./fusemount_test/hello >> namespace.txt

echo "NOTE: hello_fuse is still running" >>namespace.txt

ps aux | grep hello_fuse >>namespace.txt
echo "[$$] ---- testfuse.sh ----> EXITING" >>namespace.txt