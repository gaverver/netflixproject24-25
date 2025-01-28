import "./Logo.css"
import { useNavigate } from "react-router-dom";

function Logo() {
    const navigate = useNavigate();
    function navDisconnectedHome() {
        navigate("/home")
    }
    // clicking on the logo should navigate to the home page for disconnected users
    return(
        <h1 className="logo" onClick={navDisconnectedHome}>Catflix</h1>
    )
}

export default Logo;