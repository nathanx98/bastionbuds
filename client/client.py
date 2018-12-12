import socket
from threading import Thread


DEFAULT_IP = "127.0.0.1"
DEFAULT_PORT = "9001"


def listen(s):
    """Takes a socket and listens for messages forever"""
    while True:
        data = s.recv(1024)
        message = data.decode('utf-8')
        if message == "\QUIT":
            return
        print(message)


def send(s):
    """Takes a socket and sends messages forever"""
    while True:
        command = input()
        if len(command) > 1024:
            print('Command cannot be greater than 1024 characters long. Please try again.')
            continue
        s.send(command.encode())


# Let user specify ip and port
ip = input('Please enter the server IP Address (Default is {}): \n'.format(DEFAULT_IP))
port = int(input('Please enter the server port (Default is {}):'.format(DEFAULT_PORT)))


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))


# Spawn a listen thread and a send thread. Wait for both to join
listen_thread = Thread(target=listen, args=[s], daemon=True)
send_thread = Thread(target=send, args=[s], daemon=True)

listen_thread.start()
send_thread.start()

listen_thread.join()