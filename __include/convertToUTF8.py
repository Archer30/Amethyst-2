import os
import chardet

def convert_to_utf8(directory):
    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)
        if os.path.isfile(filepath):
            # Detect file encoding
            with open(filepath, 'rb') as file:
                raw_data = file.read()
                result = chardet.detect(raw_data)
                original_encoding = result['encoding']

            # If encoding is confidently detected
            if original_encoding is not None and result['confidence'] > 0.5:
                try:
                    with open(filepath, 'r', encoding=original_encoding) as file:
                        content = file.read()

                    with open(filepath, 'w', encoding='utf-8') as file:
                        file.write(content)
                except UnicodeDecodeError:
                    print(f"Skipping file due to decoding error: {filename}")
            else:
                print(f"Encoding not detected confidently for file: {filename}")

# Use a dot '.' to refer to the current directory
current_directory = '.'
convert_to_utf8(current_directory)