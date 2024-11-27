from http.server import HTTPServer, CGIHTTPRequestHandler

class MyHandler(CGIHTTPRequestHandler):
    # Define el directorio cgi-bin
    cgi_directories = ['/cgi-bin']

if __name__ == "__main__":
    server_address = ('', 8000)  # Escuchar en el puerto 8000
    httpd = HTTPServer(server_address, MyHandler)
    print("Servidor corriendo en http://localhost:8000")
    httpd.serve_forever()
