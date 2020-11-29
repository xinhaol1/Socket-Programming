# Socket-Programming
This project generates a TCP connection and two UDP connections to mimic the behavior of a client requesting information from a main server, which then requests data from two back-end servers.
To run:
  first compile with command “make all”,
  then boot up serverA and serverB with command “make serverA” and “make serverB”, 
  then boot up mainserver with command “make mainserver”,
  lastly boot up client with “make client”.
  At the client terminal, enter user id first, and then country/region name.
  Sample runs and results are presented in queries.txt
  
The server will respond with the recommended friend in this country/region.
Recommending rule:
  The server will recommend the user with the most common friend as client (user with the highest degree) in the same country/region.
  If more than one users have the same highest degree, recommend the user with the smallest user id.
  If client has already been connected to all friends of his friends, then recommend the user in this country/region with the largest number of friends.
  If client has already been connected to everyone in the country/region, no recommendation.

Text Files:
  data1.txt and data2.txt store the user information used for back-end server A and B. 
  Sample runs are presented in the file queries.txt
  data1.txt, data2.txt and queries.txt provided by Professor Ali Zahid at USC
