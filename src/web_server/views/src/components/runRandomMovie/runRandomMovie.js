import "./runRandomMovie.css"
import { useState, useEffect } from "react"
import { useNavigate } from "react-router-dom"
import VideoStreamer from "../VideoStreamer"

const RunRandomMovie = ({ id }) => {
  const [movieDetails, setMovieDetails] = useState(null)

  const navigate = useNavigate()

  useEffect(() => {
    const fetchMovie = async () => {
      try {
        const movie_res = await fetch(`http://localhost:3001/api/movies/${id}`)
        const json = await movie_res.json()
        setMovieDetails(json)
      } catch (err) {
        navigate("/serverError")
      }
    }

    fetchMovie()
  }, [id, navigate])

  const play = () => {
    navigate(`/movies/watch/${id}`)
  }

  const moreDetails = () => {
    navigate(`/movies/${id}`)
  }

  return (
    <div className="random-movie-component">
      {movieDetails && (
        <>
          <VideoStreamer videoId={movieDetails.video} controls={false} className="stream-random-movie" />
          <div className="age-limit">{movieDetails.age_limit}</div>
          <div className="random-movie-info">
            <div className="random-movie-name">{movieDetails.name}</div>
            <div className="random-movie-buttons">
              <button className="play-random-movie" onClick={play}>
                Play
              </button>
              <button className="random-movie-details" onClick={moreDetails}>
                More Info
              </button>
            </div>
          </div>
        </>
      )}
    </div>
  )
}

export default RunRandomMovie

