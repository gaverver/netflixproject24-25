# Web Server Functionality and MongoDB - excercise 3


## App Information
### what is the application about
The application designed to be like Netflix, but without the view, just the functionality in the backend. using API, users can connect to our server, and run some commands through curl (CRUD operations, on different urls), they can add new users, moveis, images or categories to the data base, or get details on them or patch/put or delete them. (in the next exercise we will have to consider which kind of users can do all this commands and which are not allowed, but not in this exercise). there are more functionalities our app could do like recommend on some movies to some users using the algorithm from last exercise, and search for a movie which contains a field that contains a sub-string of the input from the user, what gives the user an option to search for a movie through his name or get all movies an actor play in etc...
our server can handle many users at the same time.
## Separation of the Recommendation System from the Web Server
First, the C++ server code from exercise 2 is located in a separate branch named "ex2". In this assignment, the src folder contains two subfolders. The first, "recommendation_system," includes the C++ server code from exercise 2, updated with the changes required for exercise 3. The second, "web_server," contains all the code for the web server, including the JavaScript code.
## Recommendation System Execution
In order to run the server using Docker, you need to run the following commands when you are in the directory of the Dockerfile:

   1. **Create a new image:**  
      `docker build -f Dockerfile.server -t server .`
   2. **Create network:**  
      `docker network create netflix_network`  
   3. **Create and run a new container:**  
      `docker run -d --name serverContainer --network=netflix_network -p <server_port>:<server_port> server <server_port>`  

## Web Server Execution
In order to run the web server using Docker, you need to run the following commands when you are in the directory of the Dockerfile:
**Note:** the serverContainer in the "CPP_IP=serverContainer" is the name of the container when you create it for the cpp server. You first need to run the Cpp server and only then the web server
   1. **create a new image:**  
          `docker build --build-arg CONNECTION_STRING=<mongoDBConnectionString> --build-arg PORT=<webServer_port> --build-arg CPP_IP=serverContainer --build-arg CPP_PORT=<cppServer_port> -f Dockerfile.web -t web_server .`  
   2. **create and run the container:**  
          `docker run -d --name  myappcontainer --network=netflix_network -p <webServer_port>:<webServer_port> web_server`  
## Web Server Images Functionality
- **Post /images**
  - **Description:** The operation creates an image which is stored in the database.
  - **Requirments:** Pass the "Content-Type: application/octet-stream" as http header. Also, add to the request --data-binary "@<path_to_image>", where "path_to_image" must end with *.png or *.jpg or ... according to the type of the file that contains the image.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X POST --data-binary "@<path_to_image>" http://localhost:<webServer_port>/images -H "Content-Type: application/octet-stream"
    ```

- **Get /images/:id**
  - **Description:** The operation retrives an image which is stored in the database and identified by the given id.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X GET http://localhost:<webServer_port>/images/<image_id>
    ```

- **Delete /images/:id**
  - **Description:** The operation deletes an image which is stored in the database and identified by the given id.
  - **Example Usage:**  
    for linux and windows:
    ```bash
    curl -i -X DELETE http://localhost:<webServer_port>/images/<image_id>
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
### Recommendation System
The data is stored in files. Inside 'data' folder there are 2 txt files: users.txt and movies.txt. In users.txt, each line has numbers seperated by space: ' ', such that the first number refers to the user id, and the other numbers refers to the movie ids of the movies that the user watched. For convenience, the movie ids are sorted and has no duplicates, this maintence helps us to boost the performance of complex commands, and saves space.
### Web Server
The data is mostly stored inside mongoDB. We have 4 collections in our database: users, movies, categories and images. In the users collection we save users with the following fields: username, password, email, phoneNumber, picture, movies_watched. In the categories collection we save categories with the following fields: name, promoted, movieIds. In the movies collection we save movies with the following fields: name, description, actors, published, age_limit, creators, categories, users, photo. In the images collection we save images with the following fields: data and contentType. Moreover, the avatars that the user can choose to be their picture are saved in an avatars folder which is loaded for the users in the beginning of the program. 
## running examples
### /api/users commands examples
#### Post /api/users - creating a user
![createUser](https://github.com/user-attachments/assets/5f93dfa0-7f34-44a5-b435-663748d6bd53)
![createUser1](https://github.com/user-attachments/assets/41be80e2-ed53-40c8-950a-8a42e21aeb45)
![createUserInvalid](https://github.com/user-attachments/assets/d24c1d4d-e63c-49bc-a330-921989b2a43b)
#### Get /api/users/:id - getting information about a user
![getUser](https://github.com/user-attachments/assets/72eb21c6-c917-41b3-bc2f-3e0370bcd327)
![getUserNotFound](https://github.com/user-attachments/assets/b726723f-61c2-4945-ae9e-f7fcb1f1d17f)
#### POST /api/tokens - check if a user is registered
![tokens](https://github.com/user-attachments/assets/5a1e1923-231c-4377-ba06-317be3761599)
### Categories appendix running
![appendix1](https://github.com/user-attachments/assets/35089eeb-8c71-43bb-8293-13f27c3f572c)
![mongoappendix1](https://github.com/user-attachments/assets/b7e08edd-f8b9-417c-bdb1-7082bf7d4b07)
![appendix2](https://github.com/user-attachments/assets/07ed9639-38ca-4628-8698-4500077b2cfd)
![mongoappendix2](https://github.com/user-attachments/assets/a7d97c44-6a98-45da-bdcd-d2096fb9c68b)
![appendix3](https://github.com/user-attachments/assets/e982a31f-3c72-47fc-b5c4-2bc42796415d)
![mongoappendix3](https://github.com/user-attachments/assets/697dba8b-1194-4a13-ab6c-9d7ea84c5612)
![appendix4](https://github.com/user-attachments/assets/a11e5171-7acb-4b63-803a-6e74b482d8af)
![appendix5](https://github.com/user-attachments/assets/883f8af3-39a1-4c7a-b326-de058b111127)
![mongoappendix5](https://github.com/user-attachments/assets/aa479987-fd04-42dd-b79b-f5ff7c229a13)
### Movies - recommend Get and Post, movie and user creation
![recommend1](https://github.com/user-attachments/assets/fbda8776-17b0-4da5-a5e5-83a79b9fc6e8)
![recommendmongo1](https://github.com/user-attachments/assets/904827ac-46c8-40bd-9ab3-7915ffd38851)
![recommend2](https://github.com/user-attachments/assets/33ac73f4-e03b-48e3-82bb-a61862cce61d)
![recommendmongo2](https://github.com/user-attachments/assets/6ad97232-150d-4dd5-a7c3-b71accd12e18)
![recommend3](https://github.com/user-attachments/assets/8f62ef29-3542-4dec-8258-b5ef1afe71ff)
![recommend4](https://github.com/user-attachments/assets/c41ede5e-eb2c-4c8a-b773-50fe15017baa)
![recommend5](https://github.com/user-attachments/assets/bdccb048-fb9a-4195-894f-917c16ff86da)
### Movies - invalid check for Post /api/movies
![invalidcheck](https://github.com/user-attachments/assets/665482ba-acb9-42b6-bf2d-9a3f06ba6e01)
### Movies - invalid check for Get /api/movies/:id
![invalididmovies](https://github.com/user-attachments/assets/09c9b174-d203-4994-9c7d-89c4341a0f4e)
### Images - Create and Delete
![imagesrun1](https://github.com/user-attachments/assets/b44141b9-a6b2-42a3-8cb3-675404523d96)
![imagesmongo1](https://github.com/user-attachments/assets/ade96664-4f00-45f6-9aca-5ff3ee3a3ce6)
![imagesrun2](https://github.com/user-attachments/assets/c759f527-da07-4c65-9c2d-1379a75714a4)
![imagemongo2](https://github.com/user-attachments/assets/76eda8a4-d987-4cb1-a910-cde48b172dab)









