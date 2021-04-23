import pygame
import socket
import sys
#HOST = '192.168.43.133'
#HOST = '192.168.1.6'
HOST = '192.168.1.10' 

PORT = 8888
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

pygame.init()
try:
            s.connect((HOST,PORT))
except socket.error:
            print ("RIP, No se ha encontrado el servidor :(")
            sys.exit()
def main():
    pygame.display.set_caption('MERCURY 2019')
    surface = pygame.display.set_mode((800, 600))
    clock = pygame.time.Clock()
    running = True
 
    font = pygame.font.Font(None, 20)
    linesize = font.get_linesize()
    joysticks = [pygame.joystick.Joystick(i) for i in range(pygame.joystick.get_count())]
    for joy in joysticks:
        joy.init()
 
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.JOYAXISMOTION:
                axis = int(event.axis)
                value = int(event.value)
                if value == 0:
                    s.send(str.encode('E'))
                    s.send(str.encode('\r'))
                if axis == 1 and value == -1:
                    s.send(str.encode('W'))
                    s.send(str.encode('\r'))
                if axis == 1 and value == 1:
                    s.send(str.encode('S'))
                    s.send(str.encode('\r'))
                if axis == 0 and value == -1:
                    s.send(str.encode('A'))
                    s.send(str.encode('\r'))
                if axis == 0 and value == 1:
                    s.send(str.encode('D'))
                    s.send(str.encode('\r'))


                if axis == 3 and value == -1:
                    s.send(str.encode('I'))
                    s.send(str.encode('\r'))
                if axis == 3 and value == 1:
                    s.send(str.encode('O'))
                    s.send(str.encode('\r'))
                if axis == 2 and value == -1:
                    s.send(str.encode('Y'))
                    s.send(str.encode('\r'))
                if axis == 2 and value == 1:
                    s.send(str.encode('U'))
                    s.send(str.encode('\r'))
                    
            elif event.type == pygame.JOYHATMOTION:
                hat = str(event.value)
                print(hat)
                if hat == '(0, 0)' :
                    s.send(str.encode('E'))
                    s.send(str.encode('\r'))
                if hat == '(0, 1)':
                    s.send(str.encode('W'))
                    s.send(str.encode('\r'))
                if hat == '(0, -1)':
                    s.send(str.encode('S'))
                    s.send(str.encode('\r'))
                if hat == '(-1, 0)':
                    s.send(str.encode('A'))
                    s.send(str.encode('\r'))
                if hat == '(1, 0)':
                    s.send(str.encode('D'))
                    s.send(str.encode('\r'))
            elif event.type == pygame.JOYBUTTONDOWN:
                 boton = str(event.button)
                 s.send(str.encode(boton))
                 s.send(str.encode('\r'))
            elif event.type == pygame.JOYBUTTONUP:
                 s.send(str.encode('E'))
                 s.send(str.encode('\r'))

        surface.fill((0,0,0))
 
 
        pygame.display.flip()
        clock.tick(20)
 
    pygame.quit()
 
main()

