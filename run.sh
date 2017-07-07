gcc -std=c99 -o udpServer UDPServer.c sqlite3.c -lpthread -ldl 
gcc -std=c99 -o udpClient UDPClient.c
gcc -std=c99 -o udpClientMaster UDPClientMaster.c
./udpServer 23000
