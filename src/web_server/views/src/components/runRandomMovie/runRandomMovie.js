import "./runRandomMovie.css"
import { useState, useEffect } from "react"
import { useNavigate } from "react-router-dom"
// import the video streaming component for streaming the random movie
import VideoStreamer from "../VideoStreamer"

const RunRandomMovie = ({ id }) => {
  // State for the movie as a json
  const [movieDetails, setMovieDetails] = useState(null)
  // State that will change every 20 seconds s.t the video will refresh every 20 seconds
  const [key, setKey] = useState(0);
  const navigate = useNavigate()

  // gets the movie details
  useEffect(() => {
    const fetchMovie = async () => {
      try {
        const movie_res = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}`)
        const json = await movie_res.json()
        setMovieDetails(json)
      } catch (err) {
        // if server crushed
        navigate("/serverError")
      }
    }

    fetchMovie()
  }, [id, navigate]) // called every time the id changes

  // this function will be called in loops every 20 seconds to make the VideoStreaming component refresh
  useEffect(() => {
    // set timeout to make video refresh every 20 senods (adding 1 to the value of key)
    const timer = setTimeout(() => {
      setKey((prevKey) => prevKey + 1)
    }, 20000)

    // clearing the timeout
    return () => clearTimeout(timer)
  }, [key])

  // play button functionality
  const play = () => {
    navigate(`/movies/watch/${id}`)
  }

  // moreDetails button functionality
  const moreDetails = () => {
    navigate(`/movies/information/${id}`)
  }

  // the react component
  return (
    <div className="random-movie-component">
      {/* render this component ONLY IF! movieDetails State has seted */}
      {movieDetails && (
        <>
          {/* the key here is for refreshing the component every 20 seconds to make the video restart */}
          <VideoStreamer key={key} videoId={movieDetails.video} controls={false} className="stream-random-movie" />
          <div className="age-limit">+{movieDetails.age_limit}</div>
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

