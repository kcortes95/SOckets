all:
	gcc -std=c99 -o udpServer UDPServer.c -w includes/sqlite3.c -lpthread -ldl
	gcc -std=c99 -o udpClient -w UDPClient.c
	gcc -std=c99 -o udpClientMaster -w UDPClientMaster.c
