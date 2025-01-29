import './Home.css';
import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import Input from "../../components/Input/input"

function Home() {
  const navigate = useNavigate();
  // navigate to the login page by click
  const handleSignInClick = () => {
    navigate("/login");
  };
  // method to change the email variable(use state)
  const handleEmailChange = (event) => {
    setEmail(event.target.value);
  };
  // method to handle form submission
  const handleFormSubmission = (event) => {
    event.preventDefault();
    if (email) {
      // navigate to the registartion page with the entered email
      navigate("/register", { state: { email }});
    }
  };

  const [email, setEmail] = useState(" ")


  // the html of the page + components
  return (
    <div className="backgroundImage">
      <button id="sign-in" onClick={handleSignInClick}>Sign In</button>
      <h1>Unlimited Movies</h1>
      <form onSubmit={handleFormSubmission}>
        <Input text="Ready to watch? Enter your email to create an account and start watching movies" className="email-input" type="email" placeHolder="Email Address" value={email} onChange={handleEmailChange} required="required"/>
        <button id="register">Get Started {'>'}</button>
      </form>
    </div>
  );
}



export default Home;