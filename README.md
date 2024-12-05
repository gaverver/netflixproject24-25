# recommendation system - excercise 1

## App Information
### what is the application about
The application designed to recommend movies to users. The recommendations are based on the movies that have been watched by other users, and it also rely on recommendation algorithms. It works by taking in user input (movie IDs and user IDs) and returning a list of recommended movies.
### command that can be runned
1. add \[userid] \[movieid1] \[movieid2] …  
     adds user to the system (if it didn't exists) with movies that he watched. If the users was registered before, it adds the movies to the system.
3. recommend \[userid] \[movieid]  
     return for a user a list of 10 recommended movies releted to a movie
3. help  
     helps the end-user to learn about how to use the functionalities of the application
## Program Execution

In order to run the application using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -t netflix .`  

   2. **Create a new container:**  
      `docker run -d --name netflixcontainer netflix`  

Then, when you are all set up and have an image and a container, you need to run the same container in order for the data to be saved between executions. Now, you **only** need to do this step to execute the program:  

   - **Run the container:**  
     `docker exec -it netflixcontainer ./netflix`  
## Test Execution

In order to run the tests using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -f Dockerfile.tst -t netflix-tests .`  

   2. **Create a new container:**  
      `docker run netflix-tests`
## data management
The data is stored in files. Inside 'data' folder there are 2 txt files: users.txt and movies.txt. In users.txt, each line has numbers seperated by space: ' ', such that the first number refers to the user id, and the other numbers refers to the movie ids of the movies that the user watched. For convenience, the movie ids are sorted and has no duplicates, this maintence helps us to boost the performance of recommendation command, and saves space.
## running examples
### spaces doesn't matter & restart doesn't matter
![image](https://github.com/user-attachments/assets/4fea1103-3798-4b8d-9ec0-4982a0f6603c)
### rcommendation test
![image](https://github.com/user-attachments/assets/78c3e14b-9ceb-4a28-9c38-bfe0d0b894d3)
### adding duplicated information doesn't matter
![image](https://github.com/user-attachments/assets/f276fc99-3d14-4ca3-b9fb-3ec0b144d6cb)


