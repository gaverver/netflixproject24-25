import './Home.css';
import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import Input from "../../components/input"

function Home() {
  const navigate = useNavigate();

  const handleSignInClick = () => {
    navigate("/login");
  };

  const handleEmailChange = (event) => {
    setEmail(event.target.value);
  };

  const handleFormSubmission = (event) => {
    event.preventDefault();
    if (email) {
      navigate("/register", { state: { email }});
    }
  };

  const [email, setEmail] = useState(" ")



  return (
    <div className="backgroundImage">
      <button id="sign-in" onClick={handleSignInClick}>Sign In</button>
      <h1>Unlimited Movies</h1>
      <form onSubmit={handleFormSubmission}>
        <h4>Ready to watch? Enter your email to create an account and start watching movies</h4>
        <Input className="email-input" autoComplete="email" type="email" placeHolder="Email Address" value={email} onChange={handleEmailChange} required="required"/>
        <button id="register">Get Started {'>'}</button>
      </form>
    </div>
  );
}



export default Home;