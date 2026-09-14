import socket

def encode_message(message: str):

   return f"{message}\n".encode("UTF-8")

def open_connection(ip: str, port: int):

    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        sockfd.connect((ip, port))

        return sockfd, None

    except socket.error as error:
        return None, str(error)


def close_connection(sockfd: socket.socket):

    try:
        sockfd.shutdown(socket.SHUT_RDWR)
    except OSError:
        pass

    sockfd.close()

def send_file_name(sockfd: socket.socket, file_name: str):

    sockfd.sendall(encode_message(file_name))

def send_file_size(sockfd: socket.socket, file_size: int):

    sockfd.sendall(encode_message(file_size))

def send_file_content(sockfd: socket.socket, file_path: str):

    with open(file_path, "rb") as f:

        while True:

            bytes_read = f.read()

            if not bytes_read:
                break

            sockfd.sendall(bytes_read)

    sockfd.sendall("\n".encode("UTF-8"))
