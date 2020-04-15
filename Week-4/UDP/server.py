from socket import *
serverPort = 12000
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(("127.0.0.1",serverPort))
print("The server is ready to recceive")
while 1:
    sentence,clientAddress = serverSocket.recvfrom(2048)
    files = open(sentence,"r")
    l = files.read(2048)
    serverSocket.sendto(bytes(l,"utf-8"),clientAddress)
    print("sent to client")
    files.close()
