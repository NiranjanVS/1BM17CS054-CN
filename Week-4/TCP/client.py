from socket import *
serverName = '172.17.0.1'
serverPort = 1200
clientSocket = socket(AF_INET,SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence = input("Enter the file name : ")
clientSocket.send(sentence.encode())
filecontents = clientSocket.recv(1024).decode()
print('From server : ',filecontents)
clientSocket.close()
#
