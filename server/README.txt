Run Server:
    sudo docker build -t mylaps/rcs .
    sudo docker run -d --network=host mylaps/rcs:latest
Note:
    You can run the image without -d to see the test results
    The test coverage is not much, I just put some samples for the demonstration