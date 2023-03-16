#####socket_cpp
###Usage
cd svr
gcc -g svr.cpp -lstdc++ -lws2_32 -o svr.exe
.\svr.exe

cd cli
gcc -g cli.cpp -lstdc++ -lws2_32 -o cli.exe
.\cli.exe
