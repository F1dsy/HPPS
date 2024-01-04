import socket
import sys
import socketserver

from shared import *
from threading import Lock

# A handler function, called on each incoming message to the server
class StableHandler(socketserver.StreamRequestHandler):
    def handle(self):

        # TODO You must decide how to implement the stable handler, so that it 
        # can recieve all reindeer messages that their holiday is done, and 
        # then inform the last reindeer that it must inform the others and 
        # santa that it is time to deliver presents       
        pass
        
        msg = self.request.recv(MAX_MSG_LEN)

        if b'-' in msg:
            body = msg[msg.index(b'-')+1:]
            msg = msg[:msg.index(b'-')]
        
        if msg==MSG_HOLIDAY_OVER:
            reindeer_host = body[:body.index(b':')].decode()
            reindeer_port = int(body[body.index(b':')+1:].decode())
           
            if len(self.server.reindeer_counter) == self.server.num_reindeer-1:
                print("All Reindeer are back from holiday")
                sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sending_socket.connect((reindeer_host, reindeer_port))
                send_msg =  bytearray(MSG_NOTIFY)
                send_msg.extend(f"-{self.server.santa_host}:{self.server.santa_port}".encode())
                for host,port in self.server.reindeer_counter:
                    send_msg.extend(f"-{host}:{port}".encode())
                sending_socket.sendall(send_msg)
                sending_socket.close()
                self.server.reindeer_counter = []

            else:             
                self.server.reindeer_counter.append((reindeer_host, reindeer_port))
                print(f"{len(self.server.reindeer_counter)} Reindeer are back from holiday")

        

# A socketserver class to run the stable as a constant server
class StableServer(socketserver.ThreadingTCPServer):
    # Constructor for our custom class. If we wish to add more variables or
    # arguments this is where we could do it
    def __init__(self, server_address, num_reindeer, santa_host, santa_port, request_handler_class):
        # Call the parent classes constructor
        super().__init__(server_address, request_handler_class)
        # Record the expected number of reindeer, and santas address
        self.num_reindeer = num_reindeer
        self.santa_host = santa_host
        self.santa_port = santa_port

        # TODO you must decide on any additional variables to set up here

        self.reindeer_counter = []
 
# Base stable function, to be called as a process
def stable(my_host, my_port, santa_host, santa_port, num_reindeer):
    # Start a socketserver to always be listening
    with StableServer((my_host, my_port), num_reindeer, santa_host, santa_port, StableHandler) as stable_server:
        try:
            # Always be able to handle incoming messages
            stable_server.serve_forever()
        finally:
            # If we keyboard interupt this will wrap up all the backend stuff
            stable_server.server_close()

# As an alternative to using the socketserver_santa_problem.py, you may start a 
# standalone elf as described in the handout
if __name__ == "__main__":
    process = Process(target=stable, args=(sys.argv[1:]))
    process.start()