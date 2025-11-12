from livereload import Server

server = Server()

# Watch the specific HTML file
server.watch('dist/index.html')

# Optionally watch all files in dist (HTML, CSS, JS)
# server.watch('dist/*.*')

# Serve files from the 'dist' directory
server.serve(root='dist', port=5500, open_url_delay=1)