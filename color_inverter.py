from PIL import Image, ImageOps

def invert_binary_file(input_file, output_file):
    # Open the input binary file in read mode
    with open(input_file, 'rb') as infile:
        # Open the output binary file in write mode
        with open(output_file, 'wb') as outfile:
            # Read the input file in chunks
            byte = infile.read(1)  # Read one byte at a time
            while byte:
                # Invert the byte by flipping each bit
                inverted_byte = byte[0] ^ 0xFF  # XOR with 0xFF to flip all bits
                outfile.write(bytes([inverted_byte]))  # Write the inverted byte
                byte = infile.read(1)  # Read the next byte

    print(f"Inverted binary file saved as: {output_file}")

# Example usage:
input_bin = 'rgb_cat2'  # Path to the input binary file
output_bin = 'inv_cat2'  # Path to save the inverted binary file

invert_binary_file(input_bin, output_bin)
