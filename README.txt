Run Server:
    sudo docker build -t mylaps/rcs .
    sudo docker run -d --network=host mylaps/rcs:latest
