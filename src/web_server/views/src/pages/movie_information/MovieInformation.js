import "./MovieInformation.css";
import React, { useEffect, useState } from "react";
import Menu from "../../components/menu/menu";
import MovieInfoDisplay from "../../components/MovieInfoDisplay";
import { useNavigate, useParams } from "react-router-dom";

function MovieInformation() {
  const {id} = useParams();
  const navigate = useNavigate();
  const [userId, setUserID] = useState("");

  const fetchUserId = async (token) => {
    const response = await fetch(`http://localhost:3001/api/tokens/${token}`);
    if (!response.ok) {
      console.error("token isn't valid");
      //navigate home
      // navigate("/home")
      return;
    }
    const data = await response.json();
    setUserID(data.userId)
  }

  useEffect(() => {
    const token = sessionStorage.getItem("jwt");
    if (!token) {
      console.error("No token found");
      //navigate home
      // navigate("/home")
      return;
    }
    fetchUserId(token)
  }, [id, fetchUserId]);

  return (
    <div>
        <Menu />
        <MovieInfoDisplay key={id} id={id} userId={userId} />
    </div>
  );
}

export default MovieInformation;