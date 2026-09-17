from pathlib import (
    Path
)

def get_file_name(file_path: str):
    
    return Path(file_path).name.replace(" ", "")

def is_plain_text_file(file_path: Path):

    try:

        with open(file_path, "rb") as f:

            block = f.read()

            if b'\x00' in block:
                return False
            
            else:
                return True
            
    except IOError:
        return False

def file_bytes_size(file_path: str):

    return Path(file_path).stat().st_size