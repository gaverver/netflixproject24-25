# import socket to use it
from socket import socket, AF_INET, SOCK_STREAM
# import sys (system) to be able to get the arguments (the ip and port of the server's socket)
import sys

# main function
if __name__ == "__main__":
    # gets the ip(string) and the port(int), (sys.argv[0] is the name of this file: "client.py")
    dest_ip = sys.argv[1]
    dest_port = int(sys.argv[2])
    # creating a TCP socket with IPv4 protocol
    clientSocket = socket(AF_INET, SOCK_STREAM)
    # connecting to the server
    clientSocket.connect((dest_ip, dest_port))
    # the client never exits (except when he does ctrl + c or something like that)
    while (True):
        # gets the input from the user
        message = input()
        # sending it to the server (the thread that handle this client)
        # adding \n 'cause this is the format we decided in the scan function in socketMenu
        clientSocket.send(bytes(message + "\n", 'utf-8'))
        # reciving the data back from the server, (the result to our request)
        data = clientSocket.recv(4096)
        # prints it to the console so that the client could see his answer
        print(data.decode('utf-8'), end="")
    
