#coding:utf-8
from socket import *

port = 8899

# Create a server socket, bind it to a port and start listening
# use AF_INET for address family and SOCK_STREAM for protocol
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(('', port))
tcpSerSock.listen(5)

while True:
    # Establish connection with client
    print('Ready to serve...')
    tcpCliSock, addr = tcpSerSock.accept()
    print('Received a connection from: ', addr)

    # Receive requests (HTTP GET) from the client
    message = tcpCliSock.recv(4096).decode()
    print(message)

    # Extract the required information from the client request:
    # eg. webpage and file names
    filename = message.split()[1].partition("//")[2].replace('/', '_')
    fileExist = "false"

    try:
        # Check whether the required files exist in the cache
        f = open(filename, "r")
        outputdata = f.readlines()
        fileExist = "true"
        print('File exists!')

        # Load the file and send a response back to the client
        for i in range(0, len(outputdata)):
            tcpCliSock.send(outputdata[i].encode())
        print('Read from cache')

    except IOError:
        print('File does not exist!')
        if fileExist == "false":
            # create a socket on the proxy server to send the request
            # to the actual webserver
            c = socket(AF_INET, SOCK_STREAM)
            hostn = message.split()[1].partition("//")[2].partition("/")[0]
            print('Host Name: ', hostn)

            try:
                # Connect your client socket to the webserver at port 80
                c.connect((hostn, 80))
                print('Socket connected to port 80')

                # Send request to the webserver
                c.sendall(message.encode())

                # Read the response into buffer
                buffer = c.recv(4096)
                tcpCliSock.sendall(buffer)

                # Create a new file in the cache for the requested file.
                # Also send the response in the buffer to client socket
                # and the corresponding file in the cache
                tmpFile = open("./" + filename, "w")
                tmpFile.writelines(buffer.decode().replace('\r\n', '\n'))
                tmpFile.close()

            except Exception as e:
                print(str(e))
                print("Illegal request")

        else:
            # HTTP response message for file not found
            print('HTTP/1.0 404 File not found')

    # Close the client and the server sockets
    tcpCliSock.close()
tcpSerSock.close()
