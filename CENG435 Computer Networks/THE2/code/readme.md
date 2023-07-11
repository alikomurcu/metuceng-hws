### CENG435 THE2
- To close the connection betwwen client and server, second press of enter key closes the server, third press of enter key closes the client.
- I developed the algorithm for payload of data = 16 bytes, so total packet size is being 25 bytes. But it works with any even number of data due to checksum algorithm. If any overflow happens, just make the macro PAYLOAD 16.
- Warnings can occur after compiling, just ignore them.
- Multithreading is used, so that mutexes are everywhere :)
- There are two main scripts, server.cpp and client.cpp. Compiling this scripts with the threads.cpp will give us two binaries, client and server.
- To run these binaries check the pdf and set the vagrant environments and containers up.
- In the end, we have an application which uses Go-Back-N algorithm over UDP.