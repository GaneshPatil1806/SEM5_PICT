import socket

# Receiver's IP and port
receiver_ip = '192.168.45.49'
receiver_port = 54321

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind((receiver_ip, receiver_port))

def receive_file(filename):
    with open(filename, 'wb') as file:
        while True:
            data, sender_address = udp_socket.recvfrom(1024)
            if not data:
                break
            file.write(data)
    
    udp_socket.close()
    print("File received successfully!")

if __name__ == "__main__":
    file_to_receive = "received_file.txt"
    receive_file(file_to_receive)
