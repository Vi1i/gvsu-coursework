import socket

serverIP = "127.0.0.1"
serverPort = 9876
message = "HEY BUDDY"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(message, (serverIP, serverPort))