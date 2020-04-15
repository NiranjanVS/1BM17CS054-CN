from socket import *
serverName = '172.17.0.1'
serverPort = 1200
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind((serverName,serverPort))
serverSocket.listen(1)
print("Server is ready to receive")
while 1:
    connectionSocket,addr = serverSocket.accept()
    sentence = connectionSocket.recv(1024).decode()
    files = open(sentence,"r")
    l = files.read(1024)
    connectionSocket.send(l.encode())
    files.close()
    connectionSocket.close()