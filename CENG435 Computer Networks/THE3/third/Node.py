# Read the neighborhood information from the .costs file
# Send the node’s distance vector to every immediate neighbor
# Listen for updates from the neighbors
# Attempt to update the distance vector by using received distance vectors
# If no update happens for 5 seconds, close all connections and print the distance vector

import socket
import sys
import time
import threading
from threading import Lock
import json
# Global variables
mutex = Lock()

edited = True
outfunc = False
# parse the input file
def parseInputFile(portNumber):
    # open the file
    f = open(str(portNumber) + ".costs", "r")
    # get the number of nodes
    file = f.readlines()
    # close the file
    f.close()
    # get the number of nodes
    numberOfNodes = int(file[0])
    # get the neighbors and costs
    nodes = []
    costs = []
    count = 3000
    for i in range(numberOfNodes):
        nodes.append(count)
        costs.append(9999999)
        if portNumber-3000 == i:
            costs[i] = 0
        count += 1
    for i in range(1, len(file)):
        if int(file[i].split()[0]) in nodes:
            index = nodes.index(int(file[i].split()[0]))
            costs[index] = int(file[i].split()[1])
    dVecs = {}
    for i in range(numberOfNodes):
        dVecs[nodes[i]] = int(costs[i])
    neighbors = []
    costs_last = []
    for i in range(1, len(file)):
        line = file[i].split()
        neighbors.append(int(line[0]))
        costs_last.append(int(line[1]))
    # return the number of nodes, neighbors, and costs_last
    # print("last: " , portNumber, dVecs)
    # print("neiggh", neighbors, costs_last)
    return numberOfNodes, dVecs, nodes, costs_last


def main():
    # get the port number
    portNumber = int(sys.argv[1])
    # get the input file
    # parse the input file
    numberOfNodes, dVecs, neighbors, costs = parseInputFile(portNumber)
    # create a socket with TCP
    # sSnd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sRcv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # # bind the socket to the port number
    # s.bind(('localhost', portNumber))
    sRcv.bind(('localhost', portNumber))
    sRcv.listen()
    # print("listen to port ", portNumber)
    # create a thread to listen to the neighbors
    t = threading.Thread(target=listenToNeighbors, args=(dVecs, sRcv, portNumber, neighbors, costs))
    # t2 = threading.Thread(target=sendDistanceVector, args=(portNumber, dVecs, neighbors))
    t.start()
    # wait for 5 seconds
    time.sleep(3)
    # t2.start()
    # send the distance vector to the neighbors
    sendDistanceVector(portNumber, dVecs, neighbors)
    # close the socket
    t.join()
    # t2.join()
    # sSnd.close()
    sRcv.close()
    # print the distance vector
    printDistanceVector(portNumber, dVecs, neighbors)


# listen to the neighbors
def listenToNeighbors(dVecs, sRcv, portNumber, neighbors, costs):
    # if there is no message for 5 seconds, break the loop
    sRcv.settimeout(5)
    # listen to the neighbors
    while True:
        try:
            # receive the message
            connection, address = sRcv.accept()
            # get the sender port number
            # sender = int(address[1])
            message = connection.recv(1024).decode()
            newDvecs = json.loads(message.split("!")[0])
            sender = int(message.split("!")[1]) # port number of sender
            # print("received message from ", sender, ":", newDvecs)
            # get the distance vector

            # update the distance vector
            updateDistanceVector(portNumber, dVecs, neighbors, sender, newDvecs)
        except socket.timeout:      # interrupt the loop if there is no message for 5 seconds
            # outfunc = True
            break


# update the distance vector
def updateDistanceVector(portNumber, dVecs, neighbors, sender, newDvecs):
    # get the cost to the sender
    cost = int(dVecs[sender])
    edited = False
    # update the distance vector
    for key in newDvecs:
        if int(key) in dVecs:
            if int(dVecs[int(key)]) > int(newDvecs[key]) + cost:
                dVecs[int(key)] = int(newDvecs[key]) + cost
                edited = True
        # else:
        #     dVecs[key] = int(newDvecs[key] + cost)
    # send the distance vector to the neighbors
    if edited:
        sendDistanceVector(portNumber, dVecs, neighbors)

# send the distance vector to the neighbors
def sendDistanceVector(portNumber, dVecs, neighbors):
    # while not outfunc:
    #     if edited:
        for i in range(len(neighbors)):
            # get the neighbor
            neighbor = neighbors[i]
            # create the message
            message = json.dumps(dVecs)
            message += "!" + str(portNumber)
            # create a socket with TCP
            sSnd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # connect to neighbor
            sSnd.connect(('localhost', neighbor))
            # send the message
            sSnd.sendall(message.encode())  # address is 2-tuple (host, port)
            sSnd.close()


# print the distance vector
def printDistanceVector(portNumber, dVecs, neighbors):
    for i in range(len(neighbors)):
        print(str(portNumber), "-" + str(neighbors[i]), "|", str(dVecs[neighbors[i]]))
    # print(portNumber, ":", dVecs)
main()