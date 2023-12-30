import socket

# Sender's IP and port
sender_ip = '192.168.45.49'
sender_port = 12345

# Receiver's IP and port
receiver_ip = '192.168.45.49'
receiver_port = 54321

def send_file(filename):
    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    with open(filename, 'rb') as file:
        while True:
            data = file.read(1024)  # Read data in 1KB chunks
            if not data:
                break
            udp_socket.sendto(data, (receiver_ip, receiver_port))
    
    udp_socket.close()
    print("File sent successfully!")

if __name__ == "__main__":
    file_to_send = "send.txt"
    send_file(file_to_send)
