import socket
import os
import sys
import subprocess as sp

# vcvars.bat path
VcVars = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvars64.bat"
# Your default system encoding
enc = 'cp936'

# Host and port for server and client
host = socket.gethostname()  # Host machine name(dont change)
port = 26817  # +1s

# Compile server
def Server():
    # First open a server
    srv = socket.socket()
    srv.bind((host, port))
    # Compile process
    proc = sp.Popen(['cmd', '/k', VcVars],
            encoding=enc, bufsize=1, universal_newlines=True,
            stdin=sp.PIPE, stdout=sp.PIPE)
    cmdin = proc.stdin
    cmdout = proc.stdout
    # Prepare
    cmdin.write('prompt $- \necho end prepare\n')  # Set prompts for check
    while cmdout.readline() != 'end prepare\n': pass # Ignore until begin
    cmdout.readline()  # Ignore the null line
    # Wait for connection
    srv.listen()
    while not proc.poll():
        # Accept any connection
        print('wait for connection')
        client, _ = srv.accept()
        # Receive message and pass to compile process
        cmd = client.recv(1024).decode(enc)
        print('command: ', cmd)
        cmdin.write(cmd)
        cmdin.write('echo end command\n')
        # Read output
        now = cmdout.readline()
        output = []
        while now != 'end command\n':
            print(now)
            output.append(now)
            now = cmdout.readline()
        cmdout.readline()
        # Send back and close
        client.send(bytes(''.join(output[2:-2]), encoding=enc))
        client.close()

# If server is required, start server
if len(sys.argv) >= 2 and sys.argv[1] == 'server':
    Server()
# Try connect
try:
    client = socket.socket()
    client.connect((host, port))
    ctnt = bytes('nmake ' + ' '.join(sys.argv[1:]) + '\n', encoding=enc)
    client.send(ctnt)
    print(client.recv(2048).decode(enc))
    client.close()
except socket.error as e:
    print('socket error:', e.errno);
    os.spawnl(os.P_DETACH, sys.executable, " make.py server")

