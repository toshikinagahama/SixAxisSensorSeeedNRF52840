import http.server
import ssl

# サーバのアドレスとポートを設定
server_address = ("0.0.0.0", 4443)
handler = http.server.SimpleHTTPRequestHandler
httpd = http.server.HTTPServer(server_address, handler)

# SSLを設定
httpd.socket = ssl.wrap_socket(
    httpd.socket,
    server_side=True,
    certfile="cert.pem",
    keyfile="key.pem",
    ssl_version=ssl.PROTOCOL_TLS,
)

# 起動
print("HTTPS Server running on https://0.0.0.0:4443")
httpd.serve_forever()
