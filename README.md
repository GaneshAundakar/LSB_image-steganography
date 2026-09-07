# LSB Image Steganography Engine in C

A modular, command-line steganography engine developed in Standard C. The application hides and extracts secret text files within 24-bit uncompressed BMP images using Least Significant Bit (LSB) substitution without producing noticeable visual distortion.

---

## 🚀 Key Features

* **Lossless LSB Encoding:** Modifies only the least significant bit of raw pixel color channels (`image_byte & 0xFE | bit`), keeping color deviations within $\pm 1$ level.

* **Header Preservation:** Directly copies the initial 54-byte BMP header to protect critical image metadata (dimensions, bits-per-pixel, and raster offsets).

* **Self-Contained Protocol:** Stores encoding metadata directly within the carrier image:
  * **Magic String:** Validates presence of encrypted payload before decoding.
  * **Extension Metadata:** Tracks secret file extension length and format (e.g., `.txt`).
  * **Payload Length:** Embeds secret file size (32-bit integer) to prevent reading arbitrary data during decoding.

* **Pre-Encoding Capacity Verification:** Validates source image capacity before writing to disk:
  $$\text{Capacity} > (\text{Magic String} + \text{Extn Size} + \text{Extn} + \text{File Size} + \text{Secret Payload}) \times 8$$

---

## 📐 Encoding Protocol & Data Layout

The engine writes encoded data into the image sequentially following the 54-byte BMP header:

```text
+---------------+-------------------+----------------------+--------------------+--------------------+--------------------+---------------------+
|  BMP Header   |   Magic String    | Extn Size (Integer)  | File Extn String   | File Size (Integer)|   Secret Payload   | Remaining Raw Bytes |
|   (54 Bytes)  |      (16 Bits)    |       (32 Bits)      |     (32 Bits)      |      (32 Bits)     |   (Size * 8 Bits)  |     (Unchanged)     |
+---------------+-------------------+----------------------+--------------------+--------------------+--------------------+---------------------+
```

---

## 📁 Project Structure

```text
.
├── Makefile
├── README.md
├── common.h         # Shared definitions and MAGIC_STRING macro
├── encode.h         # Encoding function prototypes and EncodeInfo struct
├── encode.c         # LSB encoding logic and BMP processing routines
├── decode.h         # Decoding function prototypes and DecodeInfo struct
├── decode.c         # Secret recovery and extraction logic
├── types.h          # Custom types and return status enums (Status, OperationType)
└── main.c           # CLI argument parsing and top-level execution loop
```

---

## 🛠️ Build & Compilation

Ensure you have `gcc` and `make` installed.

### Compile with Make

```bash
make
```

### Manual Compilation

```bash
gcc -Wall -Wextra -std=c99 main.c encode.c decode.c -o stego_app
```

---

## 💻 Usage Instructions

### 1. Encoding (Hiding Data)

Syntax:
```bash
./stego_app -e <source_image.bmp> <secret_file.txt> [output_image.bmp]
```

* **Default output:** If the third argument is omitted, the program defaults to saving the stego image as `stego.bmp`.

Example:
```bash
./stego_app -e beautiful.bmp secret.txt output.bmp
```

### 2. Decoding (Extracting Data)

Syntax:
```bash
./stego_app -d <stego_image.bmp> [output_file_name]
```

Example:
```bash
./stego_app -d output.bmp recovered_secret.txt
```

---

