server.exe:server.c
	gcc server.c -o server.exe -l ws2_32
clean:
	rm -f *.exe