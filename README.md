# Socket-Programming
This project generates a TCP connection and two UDP connections to mimic the behavior of a client requesting information from a main server, which then requests data from two back-end servers.
To run it, first use the command make all, then boot up serverA and serverB, then mainserver, lastly client.
At the client side, enter user id first, and then country/region name.
The server will respond with the recommended friend in this country/region.
Recommending rule:
  The server will recommend the user with the most common friend as client (user with the highest degree) in the same country/region.
  If more than one users have the same highest degree, recommend the user with the smallest user id.
  If client has already been connected to all friends of his friends, then recommend the user in this country/region with the largest number of friends.
  If client has already been connected to everyone in the country/region, no recommendation.

** Currently the easy case 1 fails. Will fix it soon.
