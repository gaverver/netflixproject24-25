# server and client - excercise 2

## About Last Implementaion
### does the fact that some commands changed their name made you change your code?
No, because there is a pure virtual function called description in ICommand interface, such that every time a command change his name, we change his description and help's execution didn't changed his implementation. (of course we needed to change the name of the command, and so the name of a few files, and so the includes in each file, but we couldn't do it SOLID)
### does the fact that some new commands added to the project made you change your code?
No, because we created ICommand interface, such that it will be easy to add new commands, they just need to implement this interface.
### does the fact that the expected output of the commands have changed made you change your code?

### does the fact that now the input/output comes from sockets of clients instead of the console made you change your code?
No, because we created IMenu interface, such that it will be easy to change the way we print and scan staff, we just need the new method to implement the interface.
## App Information
### what is the application about
The application designed to add users to the database, add/delete movies from user's watched list, recommend movies to users etc. The recommendations are based on the movies that have been watched by other users, and it also rely on recommendation algorithms. It works by taking in user input (movie IDs and user IDs) and returning a list of recommended movies. you can also delete movies from your watched list, or patch an existing user's watched list. we work with a server and each client has his own thread in the server ready to work just for him (for now, until we will implement threadpool). in that way manu users could connect to the server and use our app at the same time. 
### command that can be runned
1. POST, arguments: [userid] [movieid1] [movieid2] ...
     adds user to the system if he didn't exist with movies that he watched. If the users was registered before, it will print an error.
2. PATCH, arguments: [userid] [movieid1] [movieid2] ...
     adds movies to the user's watched list if he was already exist, if he didn't exist before, prints error.
3. DELETE, arguments: [userid] [movieid1] [movieid2] ...
     deletes movies from the user's watched list only if he's already exist and watched all the movies inserted. (if not, prints an error)
4. GET, arguments: [userid] [movieid]
     recommend to a certain user on 10 (at most) movies where the movies cannot be the movie inserted or movies that the user already watched.
     it will recommend according to the algorithm stated at the first exercise.
5. help
     helps the end-user to learn about how to use the functionalities of the application
## Program Execution
In order to run the client using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **create a new image:**  
          `docker build -f Dockerfile.client -t client .`  
   2. **create and run the container:**  
          `docker run -it --network=host client <server_ip> <server_port>`  

In order to run the server using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -f Dockerfile.server -t server .`
   2. **Create network:**  
      `docker network create  --subnet=<host-ip>/24  netflix_network`  
   3. **Create and run a new container:**  
      `docker run -d --name serverContainer --network=netflix_network -p <server_port>:<server_port> server <server_port>`  

## Test Execution

In order to run the tests using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -f Dockerfile.tst -t netflix-tests .`  

   2. **Create a new container:**  
      `docker run netflix-tests`
## data management
The data is stored in files. Inside 'data' folder there are 2 txt files: users.txt and movies.txt. In users.txt, each line has numbers seperated by space: ' ', such that the first number refers to the user id, and the other numbers refers to the movie ids of the movies that the user watched. For convenience, the movie ids are sorted and has no duplicates, this maintence helps us to boost the performance of complex commands, and saves space.
## running examples
### spaces doesn't matter & restart doesn't matter
![image](https://github.com/user-attachments/assets/4fea1103-3798-4b8d-9ec0-4982a0f6603c)
### GET and help and data is stored even after exiting and connecting again
![img2](https://github.com/user-attachments/assets/05c394c8-2dbe-4456-ad91-d7b027d33489)
### adding duplicated information doesn't matter
![image](https://github.com/user-attachments/assets/f276fc99-3d14-4ca3-b9fb-3ec0b144d6cb)


