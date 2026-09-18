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

def send_metadata(sockfd: socket.socket, file_name: str, file_size: int):

    data = f"METADATA {file_name} {file_size}"

    sockfd.sendall(encode_message(data))

def send_file(sockfd, file_path):

    with open(file_path, "rb") as file:

        while True:

            data = file.read(8192)

            if not data:
                break

            sockfd.sendall(data)
