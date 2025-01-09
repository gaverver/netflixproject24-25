# Web Server Functionality and MongoDB - excercise 3


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
## Separation of the Recommendation System from the Web Server
First, the C++ server code from exercise 2 is located in a separate branch named "ex2". In this assignment, the src folder contains two subfolders. The first, "recommendation_system," includes the C++ server code from exercise 2, updated with the changes required for exercise 3. The second, "web_server," contains all the code for the web server, including the JavaScript code.
## Server Execution
In order to run the server using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -f Dockerfile.server -t server .`
   2. **Create a new container:**
      `docker run -d --name myappcontainer --network=host server <server_port>`

If you want to access from remoted device also, run this:
   1. **Create a new image:**  
      `docker build -f Dockerfile.server -t server .`
   2. **Create network:**  
      `docker network create  --subnet=<host-ip>/24  netflix_network`  
   3. **Create and run a new container:**  
      `docker run -d --name serverContainer --network=netflix_network -p <server_port>:<server_port> server <server_port>`  

## Web Server Execution
In order to run the web server using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **create a new image:**  
          `docker build --build-arg CONNECTION_STRING=<mongoDBConnectionString> --build-arg PORT=<webServer_port> --build-arg CPP_IP=<cppServer_ip> --build-arg CPP_PORT=<cppServer_port> -f Dockerfile.web -t server .`  
   2. **create and run the container:**  
          `docker run -d --name myappcontainer -p <webServer_port>:<webServer_port> server`  
## Web Server Images Functionality
- **Post /images**
  - **Description:** The operation creates an image which is stored in the database.
  - **Requirments:** Pass the "Content-Type: application/octet-stream" as http header. Also, add to the request --data-binary "@<path_to_image>".
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X POST --data-binary "@<path_to_image>" http://localhost:<webServer_port>/images -H "Content-Type: application/octet-stream"
    ```

- **Get /images/:id**
  - **Description:** The operation retrives an image which is stored in the database and identified by the given id.
  - **Requirments:** Pass the "Content-Type: application/octet-stream" as http header. Also, add to the request --data-binary "@<path_to_image>".
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET --data-binary "@<path_to_image>" http://localhost:<webServer_port>/images/<image_id> -H "Content-Type: application/octet-stream"
    ```

- **Delete /images/:id**
  - **Description:** The operation deletes an image which is stored in the database and identified by the given id.
  - **Requirments:** Pass the "Content-Type: application/octet-stream" as http header. Also, add to the request --data-binary "@<path_to_image>".
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X DELETE --data-binary "@<path_to_image>" http://localhost:<webServer_port>/images/<image_id> -H "Content-Type: application/octet-stream"
    ```
## Web Server Functionality
### User API
- **GET /api/users/:id**
  - **Description:** fetches the user details associated with a given ID, such as their name, photo, and additional information stored in the database.
  - **Example Usage:**  
    for windows and linux:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/api/users/<user_id>

- **POST /api/users**
  - **Description:** This operation creates a new user. All the details of the user are sent in the body.
  - **Requirments:** When creating a user, the following must be sent in the body: username(string), password(string), email(string), phoneNumber(10 digit string). Also, picture(integer) is optional. You must select a number greater/equal than 1 and there is a limited number of photos you can choose from.  
  - **Example Usage:**   
    for linux:  
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/users \  
     -H "Content-Type: application/json" \  
     -d '{"username":"ozer","password":"12345","email":"itzik@gmail.com", "phoneNumber":"0512369874"}'
    ```
    for windows(cmd):  
    ```bash  
    curl -i -X POST http://localhost:<webServer_port>/api/users ^  
     -H "Content-Type: application/json" ^  
     -d "{\"username\":\"ozer\",\"password\":\"12345\",\"email\":\"itzik@gmail.com\", \"phoneNumber\":\"0512369874\"}"
  

- **POST /api/tokens** 
  - **Description:** The user's username and password are provided in the request body. The system then verifies whether a user with the given information is registered.  
  - **Requirments:** The following must be sent in the body: username(string), password(string)  
  - **Example Usage:**    
    for linux:  
    ```bash  
    curl -i -X POST http://localhost:<webServer_port>/api/tokens \  
    -H "Content-Type: application/json" \  
    -d '{"username":"gavriel","password":"5555"}''
    ```
    for windows(cmd):  
    ```bash  
    curl -i -X POST http://localhost:<webServer_port>/api/tokens ^  
    -H "Content-Type: application/json" ^  
    -d "{\"username\":\"gavriel\", \"password\":\"5555\"}"  

### Category API
- **Get /api/categories**
  - **Description:** fetches all the info of all the existing categories in the system.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i http://localhost:<webServer_port>/api/categories
    ```

- **Post /api/categories**
  - **Description:** Creates a new category. All the details of the category are sent in the body.
  - **Requirments:** When creating a category, the following must be sent in the body: name(string). Also, promoted(bool) and movieIds(list of id's) are optional.
  - **Example Usage:**  
    for linux:
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/categories \
     -H "Content-Type: application/json" \
     -d '{"name": "Drama", "promoted": true}'
    ```
    for windows(cmd):
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/categories ^  
     -H "Content-Type: application/json" ^  
     -d "{\"name\": \"Drama\", \"promoted\": true}"
    ```

- **Get /api/categories/:id**
  - **Description:** fetches all the info of the category with the given id from the system.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i http://localhost:<webServer_port>/api/categories/<category_id>
    ```
    
- **Patch /api/categories/:id**
  - **Description:** Updates the details of the category identified by the specified id using the information provided in the request body.
  - **Requirments:** When updating a category, you must enter at least one of the following in the body: name(string), promoted(bool), movieIds(list of id's).
  - **Example Usage:**  
    for linux:
    ```bash
     curl -i -X PATCH http://localhost:<webServer_port>/api/categories/<category_id> \
     -H "Content-Type: application/json" \
     -d '{"name": "Action and Adventure"}'
    ```
    for windows:
    ```bash
     curl -i -X PATCH http://localhost:<webServer_port>/api/categories/<category_id> ^  
     -H "Content-Type: application/json" ^  
     -d "{\"name\": \"Action and Adventure\"}"
    ```
    
- **Delete /api/categories/:id**
  - **Description:** Deletes the category with the given id from the system.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X DELETE http://localhost:<webServer_port>/api/categories/<category_id>
    ```

### Movie API 
**Note:** before every command that you want to pass an image id, you'll first need to generate an image as described earlier. Once the image is created, you can use its ID in your commands.
- **Post /api/movies**
  - **Description:** Creates a new movie. All the details of the category are sent in the body.
  - **Requirments:** When creating a movie, the following must be sent in the body: name(string), description(string), creators(list of string). Also, actors(list of string), published(date), age_limit(int between 0 and 18), categories(list of id's), users(list of id's), photo(id) are optional.
  - **Example Usage:**  
    for linux:  
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/movies \
     -H "Content-Type: application/json" \
     -d '{"name": "Gavri", "creators": ["gavriel shandalov"], "description": "here is a description"}'
    ```
    for windows:
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/movies -H "Content-Type: application/json" -d "{\"name\": \"Gavri\", \"creators\": [\"gavriel shandalov\"], \"description\": \"here is a description\"}"
    ```
- **Get /api/movies**
  - **Description:** The operation returns movies by category(only promoted ones). Promoted categories include 20 random unseen movies for the user. A special category lists the last 20 movies the user watched in random order.
  - **Required:** pass the user id as http header in like this: "userId: <user_id>".
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/api/movies -H "userId: <user_id>"
    ```

- **Get /api/movies/:id**
  - **Description:** The operation returns the information about the movie whose id is as given.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/api/movies/<user_id>
    ```

- **Delete /api/movies/:id**
  - **Description:** Deletes the movie with the given id from the system.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X DELETE http://localhost:<webServer_port>/api/movies/<user_id>
    ```

- **Put /api/movies/:id**
  - **Description:** Replaces an existing movie identified by the given id with the given fields.
  - **Requirments:** When updating a category, you must enter the following in the body: name(string), description(string), creators(list of string). Also, actors(list of string), published(date), age_limit(int between 0 and 18), categories(list of id's), users(list of id's), photo(id) are optional.
  - **Example Usage:**  
    for linux:  
    ```bash
    curl -i -X PUT http://localhost:<webServer_port>/api/movies \
     -H "Content-Type: application/json" \
     -d '{"name": "Gavri", "creators": ["gavriel shandalov"], "description": "here is a description"}'
    ```
    for windows:
    ```bash
    curl -i -X PUT http://localhost:<webServer_port>/api/movies -H "Content-Type: application/json" -d "{\"name\": \"Gavri\", \"creators\": [\"gavriel shandalov\"], \"description\": \"here is a description\"}"
    ```

- **Get /api/movies/:id/recommend/**
  - **Description:** Retrieves recommended movies from the recommendation system for the current user, based on the movie identified by the given id.
  - **Requirments:** pass the user id as http header in like this: "userId: <user_id>".
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/api/movies/<movie_id>/recommend/ -H "userId: <user_id>"
    ```

- **Post /api/movies/:id/recommend/**
  - **Description:** The operation adds to the recommendation system that the current user has watched the movie identified by the given id.  
  - **Requirments:** pass the user id as http header in like this: "userId: <user_id>".  
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X POST http://localhost:<webServer_port>/api/movies/<movie_id>/recommend/ -H "userId: <user_id>"
    ```

- **Get /api/movies/search/:query/**
  - **Description:** Retrieves the movies that match the search result of the query string. In other words, it searches for the query string in any of the fields of the movies and retrieves the movies where the string appears in one of their fields.  
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/api/movies/search/<query_string>/
    ```
## data management
The data is stored in files. Inside 'data' folder there are 2 txt files: users.txt and movies.txt. In users.txt, each line has numbers seperated by space: ' ', such that the first number refers to the user id, and the other numbers refers to the movie ids of the movies that the user watched. For convenience, the movie ids are sorted and has no duplicates, this maintence helps us to boost the performance of complex commands, and saves space.
## running examples
### POST and PATCH and DELETE
![image1](https://github.com/user-attachments/assets/3c4a419c-9e99-4b5e-955d-101526b7ecc9)
### GET and help and data is stored even after exiting and connecting again
![img2](https://github.com/user-attachments/assets/05c394c8-2dbe-4456-ad91-d7b027d33489)
### Running 2 clients simultaneously
![image](https://github.com/user-attachments/assets/0777d62e-ed53-4402-9b2f-b3b3880b77b9)


