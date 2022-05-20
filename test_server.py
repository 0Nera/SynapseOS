import socket, time

n = 0
while True:
    print(n)
    sock = socket.socket()
    sock.connect(('localhost', 2030))
    sock.send(b"Hello!\n")
    sock.close()
    time.sleep(1)
    n += 1