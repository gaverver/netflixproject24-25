import './movieList.css';
import React, { useState, useEffect } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import MoviePic from '../moviePic/moviePic'

const MovieList = ({ list, contentType }) => {
    const [List, setList] = useState([]);
    const [isScroll, setScroll] = useState(false);
    const [howMatchLeftScrolls, setLeftScrollsAmount] = useState(0);
    const [howMatchRightScrolls, setRightScrollsAmount] = useState(0);
    const navigate = useNavigate();

    useEffect(() => {
        // setting the list of movies from the input to this function
        setList(list);
        // only if there are more then 7 movies, scroll button is needed because the screen is too small
        // (according to my screen size at least)
        setScroll(list.length > 7);
    }, []);

    // this function will be called everytime the amount of right/left scrolls changes
    useEffect(() => {
        // actual scroll

    }, [howMatchLeftScrolls, howMatchRightScrolls])

    // a function to scroll between movies
    const scroll = async () => {

    }

    return (
        <div className='movies-line'>
          <div className='category-name'>
            {contentType}
          </div>
          <div>
            {List.map((movieId) => (
                <MoviePic key={movieId} id={movieId} />
            ))}
          </div>
          <div>
            {isScroll && (
                <><button className='scroll-left-button' onClick={scroll("left")}>
                    <img src='images_for_decoration/scroll_icon.jpg' />
                </button>
                <button className='scroll-right-button' onClick={scroll("right")}>
                    <img src='images_for_decoration/scroll_icon.jpg' />
                </button></>
            )}
          </div>
        </div>
    );
};

export default MovieList;








/* ANOTHER VERSION:

import './movieList.css';
import React, { useState, useEffect, useRef } from 'react';
import MoviePic from '../moviePic/moviePic';

const MovieList = ({ list, contentType }) => {
    const scrollContainerRef = useRef(null);
    const [canScrollLeft, setCanScrollLeft] = useState(false);
    const [canScrollRight, setCanScrollRight] = useState(false);

    useEffect(() => {
        updateScrollButtons();
    }, [list]);

    const updateScrollButtons = () => {
        const container = scrollContainerRef.current;
        if (!container) return;
        
        setCanScrollLeft(container.scrollLeft > 0);
        setCanScrollRight(container.scrollLeft + container.clientWidth < container.scrollWidth);
    };

    const scroll = (direction) => {
        const container = scrollContainerRef.current;
        if (!container) return;

        const scrollAmount = 300; // Adjust for smooth scrolling (based on your design)
        
        if (direction === "left") {
            container.scrollBy({ left: -scrollAmount, behavior: "smooth" });
        } else {
            container.scrollBy({ left: scrollAmount, behavior: "smooth" });
        }

        setTimeout(updateScrollButtons, 300); // Wait for scroll animation to update buttons
    };

    return (
        <div className="movies-line">
            <div className="category-name">{contentType}</div>

            <div className="movies-container">
                {canScrollLeft && (
                    <button className="scroll-left-button" onClick={() => scroll("left")}>
                        <img src="images_for_decoration/scroll_icon.jpg" alt="Left" />
                    </button>
                )}

                <div className="movies-wrapper" ref={scrollContainerRef} onScroll={updateScrollButtons}>
                    {list.map((movieId) => (
                        <MoviePic key={movieId} id={movieId} />
                    ))}
                </div>

                {canScrollRight && (
                    <button className="scroll-right-button" onClick={() => scroll("right")}>
                        <img src="images_for_decoration/scroll_icon.jpg" alt="Right" />
                    </button>
                )}
            </div>
        </div>
    );
};

export default MovieList;

*/