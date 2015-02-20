Server is run just as python UDPServer.py

Client is run as python UDPClient.py SERVERIP SERVERPORT
Example python UDPClient.py 127.0.0.1 9876

Client then asks the user for a file name (include the extension). This is sent to the server and the server checks if it is present in the same directory
If it is not, then the server sends a message back to the client telling it that it doesn't have the file and the client closes.
The server remains ready for another request.

If the file is on the server, the server sends a message saying so to the client and then begins sending the file. Currently the file is 
sent in 1000 byte chunks. The client takes each of these chuncks and writes it to a file with the same name as the requested file. 
Downloading a file with the same name as a file will overwrite the current file. So far this has worked with all file types including
jpg, txt, exe, and docx. The largest file I've sent was 5Mb and it worked fine.

We need to still add sliding window and some sort of verification into our program. I did not use scapy at all because of the problem of
needing to use raw sockets which require higher privledges. 

