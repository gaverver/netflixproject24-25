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
