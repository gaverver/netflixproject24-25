# Catflix website and android app - final excercise

## Work Flow
Through this project, we have divided the missions between us, in an equal way, and tried to make all of us learn the most through this project.
The way we divided the missions between us is explained below:
### Gavrial
The SCRUM MASTER of excercises 1 and 2.
Worked on the users all the way, also did threadpool and the Login, Registraion and Home for unconnected users pages.(both in web and anroid)
Also did the Docker-compose (yml), and did the videos in the MVC architecture.
### Amit
The SCRUM MASTER of excercise 3.
Worked on the movies all the way, also did DBfile, and the Admin, Watch Movie and MovieInfo pages.(both in the web and the android app)
Also did the video streaming part, and also did the side menu part in android (there is also top menu in android just like in netflix)
### Asif
The SCRUM MASTER of this final excercise.
worked on the Categories and images all the way, also handled token functionality, and the Home for logged in users, Categories and Search Results pages.(both in web and android)
Also did the top menu in the web and also the top menu in the android, and also the moviePic in the web.
## Running the app
In order to run the app, you need to run the docker-compose at first. First, you need to create a .env in the folder of the docker-compose.yml with the following info: CONNECTION_STRING, PORT, CPP_PORT, JWT_SECRET in the exact same format as the following format:  
![env_file_format](https://github.com/user-attachments/assets/f4b540f6-3770-404c-876a-f854d15116e1)
then, you need to run the following command:
`docker-compose --env-file .env up --build`  
![docker_run1](https://github.com/user-attachments/assets/67f16ff3-0c69-41f6-b318-804d62b1783a)
![docker_run2](https://github.com/user-attachments/assets/7338d08c-b131-441a-b352-b7860d27d96f)
Now, you are all set up. You can enter the web using the url's in the web page with the port you have entered in the .env file. Moreover, now you can run the android in the following way: 
clone your project into android studio, and afterwards open the project from the android folder that is inside the src folder.
In the strings.xml file, change the "Base_url" to match the values you entered in the .env file.
![BaseUrl](https://github.com/user-attachments/assets/0143c48a-29e9-4375-a1ac-859efa69c50f)

for more details about our web app, see in the wiki: https://github.com/gaverver/netflixproject24-25/wiki












