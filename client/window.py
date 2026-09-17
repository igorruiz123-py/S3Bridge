from PySide6.QtWidgets import (
    QMainWindow,
    QWidget,
    QLineEdit,
    QPushButton,
    QLabel,
    QFileDialog
)

from PySide6.QtCore import Qt

from socket_funcs import (
    open_connection,
    close_connection,
    send_metadata
)

from app_funcs import (
    get_file_name,
    is_plain_text_file,
    file_bytes_size
)


class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.sockfd = None
        self.chosen_file = False
        self.file_path = None

        self.setWindowTitle("S3Bridge Client")
        self.setFixedSize(800, 800)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.connection_panel = QWidget(self.central_widget)
        self.connection_panel.setFixedSize(400, 300)
        self.connection_panel.move(200, 20)

        self.connection_panel.setStyleSheet("""
            QWidget {
                background-color: #252525;
                border: 1px solid #555555;
                border-radius: 10px;
            }
        """)

        self.title_connection_panel = QLabel("Connect to S3Bridge server", self.connection_panel)
        self.title_connection_panel.move(100, 50)

        self.hostname = QLineEdit(self.connection_panel)
        self.hostname.setPlaceholderText("Enter hostname")
        self.hostname.setFixedSize(210, 25)
        self.hostname.move(95, 110)

        self.port_text = QLineEdit(self.connection_panel)
        self.port_text.setPlaceholderText("Enter port")
        self.port_text.setFixedSize(210, 25)
        self.port_text.move(95, 150)

        self.connect_button = QPushButton("connect", self.connection_panel)
        self.connect_button.setFixedSize(90, 30)
        self.connect_button.move(155, 200)

        self.connect_button.setCheckable(True)
        self.connect_button.clicked.connect(self.toggle_connection)

        self.connection_status_message = QLabel("", self.connection_panel)
        self.connection_status_message.setFixedSize(300, 25)
        self.connection_status_message.move(50, 250)
        self.connection_status_message.setAlignment(Qt.AlignCenter)

        self.file_transfer_panel = QWidget(self.central_widget)
        self.file_transfer_panel.setFixedSize(400, 320)
        self.file_transfer_panel.move(200, 350)

        self.file_transfer_panel.setStyleSheet("""
            QWidget {
                background-color: #252525;
                border: 1px solid #555555;
                border-radius: 10px;
            }
        """)

        self.title_file_transfer_panel = QLabel(self.file_transfer_panel)
        self.title_file_transfer_panel.setText("Upload plain text files to server")
        self.title_file_transfer_panel.move(100, 50)

        self.choose_file_button = QPushButton("choose file", self.file_transfer_panel)
        self.choose_file_button.setFixedSize(90, 30)
        self.choose_file_button.move(155, 100)
        self.choose_file_button.setCheckable(True)
        self.choose_file_button.clicked.connect(self.open_file)

        self.choose_file_status_message = QLabel("", self.file_transfer_panel)
        self.choose_file_status_message.setFixedSize(300, 25)
        self.choose_file_status_message.move(50, 150)
        self.choose_file_status_message.setAlignment(Qt.AlignCenter)

        self.upload_file_button = QPushButton("upload file", self.file_transfer_panel)
        self.upload_file_button.setFixedSize(90, 30)
        self.upload_file_button.move(155, 190)
        self.upload_file_button.setCheckable(True)
        self.upload_file_button.clicked.connect(self.upload_file)

        self.upload_file_status_message = QLabel("", self.file_transfer_panel)
        self.upload_file_status_message.setFixedSize(300, 25)
        self.upload_file_status_message.move(50, 240)
        self.upload_file_status_message.setAlignment(Qt.AlignCenter)


    def toggle_connection(self, checked):

        if checked:
            self.connect_server()
        else:
            self.disconnect_server()

    def connect_server(self):

        hostname = self.hostname.text()
        port_text = self.port_text.text()

        if not hostname:
            self.connection_status_message.setText("No hostname provided")
            self.connect_button.setChecked(False)
            return

        if not port_text:
            self.connection_status_message.setText("No port provided")
            self.connect_button.setChecked(False)
            return

        try:
            port = int(port_text)

        except ValueError:
            self.connection_status_message.setText("Invalid port format")
            self.connect_button.setChecked(False)
            return

        if port < 1 or port > 65535:
            self.connection_status_message.setText("Invalid port range")
            self.connect_button.setChecked(False)
            return

        sockfd, error = open_connection(hostname, port)

        if sockfd is not None:
            self.sockfd = sockfd

            self.connection_status_message.setText("Connected")

            self.connect_button.setText("disconnect")

        else:
            self.connection_status_message.setText(f"Connection failed: {error}")

            self.connect_button.setChecked(False)

    def disconnect_server(self):

        if self.sockfd is not None:

            close_connection(self.sockfd)

            self.sockfd = None

            self.connection_status_message.setText("Disconnected")

            self.connect_button.setText("connect")

    def open_file(self):

        if self.sockfd is not None:

            file_path, selected_filters = QFileDialog.getOpenFileName(self, "Open plain text files", "", "Text Files (*.txt)")

            if file_path:

                if is_plain_text_file(file_path):

                    self.file_name = get_file_name(file_path)

                    self.choose_file_status_message.setText(f"{self.file_name}")

                    self.chosen_file = True

                    self.file_path = file_path

                else:
                    self.choose_file_status_message.setText("Only plain text files allowed")
                    self.chosen_file = False
                    return None

        else:

            self.choose_file_status_message.setText("connection to S3Bridge server is required")
            return None

    def upload_file(self):

        if self.sockfd is not None and self.chosen_file is True:

            file_size = file_bytes_size(self.file_path)

            send_metadata(self.sockfd, self.file_name, file_size)

            self.upload_file_status_message.setText("file upload successfully to S3Bridge server")

        else:

            self.upload_file_status_message.setText("failed to upload file to S3Bridge server")

