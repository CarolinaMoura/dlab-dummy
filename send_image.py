import serial
import time
from PIL import Image

BAUDRATE = 250000
ARDUINO_PORT = '/dev/cu.usbmodem21301'

arduino = serial.Serial(port=ARDUINO_PORT, baudrate=BAUDRATE, timeout=1)
time.sleep(2)  # Allow time for Arduino to reset


# Load and Convert Image to RGB565
def convert_image_to_rgb565(image_path):
    img = Image.open(image_path).resize((10, 10), Image.Resampling.BILINEAR)  # Resize to match your display
    img = img.convert("RGB")  # Ensure RGB mode (discard alpha channel if any)
    rgb565_data = []

    # Normalize colors (optional, to enhance colors)
    img = Image.eval(img, lambda x: int(x * 1.1) if x * 1.1 < 255 else 255)  # Brighten slightly

    for pixel in img.getdata():
        r, g, b = pixel
        # Convert RGB888 to RGB565
        rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
        rgb565_data.append(rgb565)

    return rgb565_data


# Send Image Data Over Serial
def send_image_data(image_path):
    rgb565_data = convert_image_to_rgb565(image_path)
    byte_array = bytearray()
    for color in rgb565_data:
        high_byte = (color >> 8) & 0xFF
        low_byte = color & 0xFF
        high_byte = low_byte = 0
        byte_array.extend([high_byte, low_byte])  # Add both bytes to the array

    arduino.write(byte_array)  # Send all at once
    with open("output_image", "wb") as f:
        f.write(byte_array)
    print("Image data sent.")

# Example Usage
send_image_data("images/checkboard.png")
arduino.close()