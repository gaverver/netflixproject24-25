import "./notFound.css"
import { useNavigate } from "react-router-dom";

function NotFound() {
    const navigate = useNavigate();
    // function to navigate to the home page
    function navHome() {
        navigate("/home")
    }
    // return the view of the home.
    return(
        <div>
            <div class="error-page-header">
                <p class="logo" onClick={navHome}>Redflix</p>
            </div>
            <div class="backgroundImage">
                <h1>Lost Your Way?</h1>
                <p>Sorry, we can't find that page. You'll find lots to explore on the home page. </p>
                <button class="home-button" onClick={navHome}>Return Home</button>
            </div>
        </div>
        
    )
    
}

export default NotFound;