#!/usr/bin/env python
import socket
import serial
import time
import subprocess
import sys

#host = '192.168.43.133'
#host = '192.168.1.6'

host = '192.168.0.100'
port = 5560

ser=serial.Serial("/dev/ttyACM0",9600)
print("Servidor Mercury 2019.")


def setupServer():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket creado.")
    try:
        s.bind((host, port))
    except socket.error as msg:
            print("La direccion esta siendo usada por un ente desconocido :(.")
            sys.exit()
    print("Socket enlazado correctamente.")
    return s

def setupConnection():
    s.listen(1) # Allows one connection at a time.
    conn, address = s.accept()
    print("Se ha establecido conexion con el cliente con Direccion: " + address[0] + " : " + str(address[1]))
    return conn

def HOLA():
    reply = storedValue
    return reply
def W():
    reply ="ADELANTE"
    num="A"
    ser.write(bytes(num.encode("ascii")))
    return reply
def A():
    reply ="IZQUIERDA"
    num="B"
    ser.write(bytes(num.encode("ascii")))
    return reply
def D():
    reply ="DERECHA"
    num="C"
    ser.write(bytes(num.encode("ascii")))
    return reply
def S():
    reply ="ATRAS"
    num="D"
    ser.write(bytes(num.encode("ascii")))
    return reply
def LIGHTON():
    reply ="ENCENDER LUZ"
    num="L"
    ser.write(bytes(num.encode("ascii")))
    return reply
def LIGHTOFF():
    reply ="APAGAR LUZ"
    num="X"
    ser.write(bytes(num.encode("ascii")))
    return reply
def SUBIR():
    reply ="BRAZO"
    num="F"
    ser.write(bytes(num.encode("ascii")))
    return reply
def BAJAR():
    reply ="BRAZO"
    num="G"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CERRAR():
    reply ="GARRA"
    num="H"
    ser.write(bytes(num.encode("ascii")))
    return reply
def ABRIR():
    reply ="GARRA"
    num="I"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CAMARAIZQUIERDA():
    reply ="MOV CAMARA IZQUIERDA"
    num="J"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CAMARADERECHA():
    reply ="MOV CAMARA DERECHA"
    num="K"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CAMARAARRIBA():
    reply ="MOV CAMARA ARRIBA"
    num="N"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CAMARAABAJO():
    reply ="MOV CAMARA ABAJO"
    num="O"
    ser.write(bytes(num.encode("ascii")))
    return reply
def CAM():
    reply ="CAMARA RECTA"
    num="P"
    ser.write(bytes(num.encode("ascii")))
    return reply
def DISPARO():
    reply ="DISPARO"
    num="Q"
    ser.write(bytes(num.encode("ascii")))
    return reply
def E():
    reply ="PARAR"
    num="E"
    ser.write(bytes(num.encode("ascii")))
    return reply
def REPEAT(dataMessage):
    reply = dataMessage[1]
    return reply

def dataTransfer(conn):
    # A big loop that sends/receives data until told not to.
    while True:
        # Receive the data
        data = conn.recv(1024) # receive the data
        data = data.decode('utf-8')
        # Split the data such that you separate the command
        # from the rest of the data.
        dataMessage = data.split(' ', 1)
        command = dataMessage[0]
        if command == 'HOLA':
            reply = HOLA()
        elif command == 'W':
            reply = W()
        elif command == 'A':
            reply = A()
        elif command == 'D':
            reply = D()
        elif command == 'S':
            reply = S()
        elif command == 'E':
            reply = E()
        elif command == '10':
            reply = LIGHTON()
        elif command == '11':
            reply = LIGHTOFF()
            
        elif command == 'Y':
            reply = SUBIR()
        elif command == 'U':
            reply = BAJAR()
        elif command == 'I':
            reply = CERRAR()
        elif command == 'O':
            reply = ABRIR()

        elif command == '0':
            reply = CAM()
            
        elif command == '2':
            reply = DISPARO()

            
        elif command == '4':
            reply = CAMARAIZQUIERDA()
        elif command == '5':
            reply = CAMARADERECHA()
        elif command == '6':
            reply = CAMARAARRIBA()
        elif command == '7':
            reply = CAMARAABAJO()
        elif command == 'REPEAT':
            reply = REPEAT(dataMessage)
        elif command == 'EXIT':
            print("Our client has left us :(")
            break
        elif command == 'KILL':
            print("Our server is shutting down.")
            s.close()
            break
        else:
            reply = 'Unknown Command'
        # Send the reply back to the client
        conn.sendall(str.encode(reply))
        print("Se han enviado datos al cliente!")
    conn.close()
        

s = setupServer()

while True:
    try:
        conn = setupConnection()
        dataTransfer(conn)
    except:
        num="E"
        ser.write(bytes(num.encode("ascii")))
        print("OH NO!, algo le ha ocurrido al cliente :(")
        break
