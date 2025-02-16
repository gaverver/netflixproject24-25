import "./Login.css"
import { useState } from 'react'
import { useNavigate, Link } from 'react-router-dom'
import Logo from "../../components/Logo/Logo"
import Input from "../../components/Input/input"
function Login() {
    const [username, setUsername] = useState("")
    const [password, setPassword] = useState("")
    const navigate = useNavigate();
    function navHome() {
        navigate("/")
    }
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
    // method to change the email variable(use state)
    const handleUsernameChange = (event) => {
        setUsername(event.target.value);
    };
    // method to change the password variable(use state)
    const handlePasswordChange = (event) => {
        setPassword(event.target.value);
    };
    const handleLoginFormSubmission = async (event) => { 
        event.preventDefault();
        try {
            let enteredData = {
                username,
                password
            }
            const tokenResponse = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/tokens`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(enteredData),
            })
            let tokenJson = await tokenResponse.json();
            // if a token wasn't created succefully, send an appropriate message to the user
            if (!tokenResponse.ok) {
                let errorMsg = tokenJson.error;
                showToast(errorMsg, "red");
                return;
            }
            // get the token of the user from the response
            let token = tokenJson.token
            // save the token of the user in the session storage
            sessionStorage.setItem("jwt", token)
            showToast("Login success!", "green");
            navHome();
        } catch (error) {
            showToast("Login error", "red");
        }
    }
    return (
        <div>
            <div className="backgroundImage">
                <Logo className="logo"/>
                <div className="loginContainer">
                    <h1 className="signin">Sign in</h1>
                    <div className="login-fields">
                        <form onSubmit={handleLoginFormSubmission} className="loginForm">
                            <Input text="Username" className="itzik" placeHolder="Username" type="text" value={username} onChange={handleUsernameChange} required="required" />
                            <Input text="Password" className="itzik" placeHolder="Password" type="password" value={password} onChange={handlePasswordChange} required="required" />
                            <p>Don't have an account? <Link to="/register" className="registerLink">Register</Link></p>
                            <button type="submit">Sign in</button> 
                        </form>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default Login;