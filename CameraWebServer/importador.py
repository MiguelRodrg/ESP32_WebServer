# Archivo para descomprimir el código HTML compreso en el código cpp del programador de la ESP32-CAM

import gzip

with open("camera_index.h", "rb") as f:
    data = f.read()

# Encuentra la parte después de `{`
start = data.find(b"{") + 1
end = data.find(b"}")

compressed_data = bytes(int(x, 16) for x in data[start:end].decode().split(",") if x.strip())
with open("index_ov2640.html.gz", "wb") as f:
    f.write(compressed_data)

# Descomprimir
with gzip.open("index_ov2640.html.gz", "rb") as f_in, open("index_ov2640.html", "wb") as f_out:
    f_out.write(f_in.read())

print("Archivo descomprimido: index_ov2640.html")
