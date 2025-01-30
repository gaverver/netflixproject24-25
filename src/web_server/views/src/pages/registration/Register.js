import "./Register.css"
import RegisterLine from "../../components/RegisterLine/RegisterLine"
import React, { useState } from "react";
import Logo from "../../components/Logo/Logo"
import ImageUploader from "../../components/RegisterImageUploader/ImageUploader";
import { useNavigate, useLocation, Link } from "react-router-dom";

function Register () {
    const navigate = useNavigate();
    const location = useLocation();
    // get the image from the home page for disconnected users
    const initEmail = location.state?.email || "";

    function showToast(message, color = "#333", duration = 3000) {
        // Create toast element
        let toast = document.createElement("div");
        toast.className = "toast";
        toast.textContent = message;
        toast.style.backgroundColor = color;
    
        // Append the toast to the body
        document.body.appendChild(toast);
    
        // Show the toast
        setTimeout(() => {
            toast.classList.add("show");
        }, 100);
    
        // remove the toast after the specified duration
        setTimeout(() => {
            toast.classList.remove("show");
            // Wait for the disappearance to complete before removing the element from the DOM
            setTimeout(() => {
                document.body.removeChild(toast);
            }, 500);
        }, duration);
    }


    // function to navigate to the home page
    function navHome() {
        navigate("/")
    }
    // method to change the username variable(use state)
    const handleEmailChange = (event) => {
        setEmail(event.target.value);
    };
    // method to change the email variable(use state)
    const handleUsernameChange = (event) => {
        setUsername(event.target.value);
    };
    // method to change the password variable(use state)
    const handlePasswordChange = (event) => {
        setPassword(event.target.value);
    };
    // method to change the phoneNumber variable(use state)
    const handlePhoneNumberChange = (event) => {
        setPhoneNumber(event.target.value);
    };
    const [username, setUsername] = useState("")
    const [password, setPassword] = useState("")
    const [phoneNumber, setPhoneNumber] = useState("")
    const [email, setEmail] = useState(initEmail)
    const [imageData, setImageData] = useState(null);



    const handleFormSubmission = async (event) => {
        event.preventDefault();
        let imageInfo = imageData; 
        if (imageData.startsWith("data:image")) {
            imageInfo = imageInfo.split(",")[1];  // Remove the "data:image/png;base64," part
        }
        // converet the entered image into the fitting format(base 64)
        const binaryString = atob(imageInfo);
        const binaryLength = binaryString.length;
        const binaryArray = new Uint8Array(binaryLength);
    
        for (let i = 0; i < binaryLength; i++) {
            binaryArray[i] = binaryString.charCodeAt(i);
        }
    
        const blob = new Blob([binaryArray], { type: "application/octet-stream" });
        try {
            // fetch a POST request to the web-server for creating an image with the entered image
            const imageResponse = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/images`, {
                method: 'POST',
                body: blob,
                headers: {
                    'Content-Type': 'application/octet-stream',
                },
            });
            // handles the case where the image response isn't 201
            if(!imageResponse.ok) {
                // show an appropriate message for the user
                const responseImageData = await imageResponse.json();
                showToast(responseImageData.error, "red")
                return;
            }
            // Get the 'Location' header and extract the image ID
            const locationHeader = imageResponse.headers.get('Location');
            const picture = locationHeader.split("/").pop();
    
            // the data contains the fields that the user entered + his image id.
            const data = {
                username,
                password,
                email,
                phoneNumber,
                picture
            };
            // create a user with the entered information
            const userResponse = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/users`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(data),
            });
            // handles the case where the user response isn't 201
            if (!userResponse.ok) {
                const responseData = await userResponse.json();
                showToast(responseData.error, "red");
                return;
            } 


            const tokenData = {
                username,
                password
            };

            const tokenResponse = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/tokens`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(tokenData),
            })
            // if a token wasn't created succefully, send an appropriate message to the user
            if (!tokenResponse.ok) {
                showToast("registration failed", "red");
                return;
            }
            // get the token of the user from the response
            let tokenJson = await tokenResponse.json();
            let token = tokenJson.token
            // save the token of the user in the session storage
            sessionStorage.setItem("jwt", token)
            showToast("Registration success!", "green");
            navHome();




        } catch (error) {
            showToast("Registration error", "red");
        }
    }
    // the html of the registration page
    return (
        <div>
            <div className="backgroundImage">
                <Logo className="logo"/>
                <div className = "register-place">
                    <header>
                        <h1 className ="reg">Register</h1>
                    </header>
                    <form onSubmit={handleFormSubmission}>
                        <div className="register-fields">
                            <RegisterLine 
                            typeFirst="text" typeSecond="password" 
                            placeHolderFirst="username" placeHolderSecond="password"
                            valueFirst={username} valueSecond={password}
                            onChangeFirst={handleUsernameChange} onChangeSecond={handlePasswordChange} />
                            <RegisterLine 
                            typeFirst="email" typeSecond="tel" 
                            placeHolderFirst="email" placeHolderSecond="phone number"
                            valueFirst={email} valueSecond={phoneNumber}
                            onChangeFirst={handleEmailChange} onChangeSecond={handlePhoneNumberChange} />
                            <label className="image-label">Profile Picture</label>
                           <ImageUploader onImageUpload={setImageData} />
                            <p>Already have an account? <Link to="/login" className="loginLink">Sign in</Link></p>
                            <button type="submit">Register</button> 
                        </div>
                    </form>
                </div>
            </div>
        </div>
    );
}
export default Register;