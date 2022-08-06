Run Server:
    sudo docker build -t mylaps/rcs .
    sudo docker run -d --network=host mylaps/rcs:latest

** You can run the image without -d to see the test results.
** The test coverage is not much, I just put some samples for the demonstration.
** Didn't have enough time to dockerize the client(QT) side. Also client side became a bit messy because I had to rush it a bit. my apologize.
** On client side first upload a csv file, it'll appear on the list view (or press refresh to get the old ones) then by clicking on each one you'll get the winner.