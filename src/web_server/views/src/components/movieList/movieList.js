import './movieList.css';
import React, { useState, useEffect, useRef } from 'react';
import MoviePic from '../moviePic/moviePic';

const MovieList = ({ list, contentType }) => {
    // useRef for the current position (in terms of scrolling)
    const scrollContainerRef = useRef(null);
    // State for knowing if scrolling left is possible
    const [canScrollLeft, setCanScrollLeft] = useState(false);
    // State for knowing if scrolling right is possible
    const [canScrollRight, setCanScrollRight] = useState(false);

    // whenever the list changes, the scroll buttons disability need to change
    useEffect(() => {
        updateScrollButtons();
    }, [list]);

    // check if the scroll buttons is available
    const updateScrollButtons = () => {
        const container = scrollContainerRef.current;
        if (!container) return;
        
        setCanScrollLeft(container.scrollLeft > 0);
        setCanScrollRight(container.scrollLeft + container.clientWidth < container.scrollWidth);
    };

    // function for scrolling
    const scroll = (direction) => {
        const container = scrollContainerRef.current;
        if (!container) return;

        // adjust for smooth scrolling
        const scrollAmount = 300;
        
        // check scroll direction
        if (direction === "left") {
            container.scrollBy({ left: -scrollAmount, behavior: "smooth" });
        } else {
            container.scrollBy({ left: scrollAmount, behavior: "smooth" });
        }

        // wait for scroll animation to update buttons
        setTimeout(updateScrollButtons, 300);
    };

    // if the list of movies is empty, then there is no need to show anything
    if (list.length === 0) {
        return null;
    }

    // the react component
    return (
        <div className="movies-line">
            <div className="category-name">{contentType}</div>

            <div className="movies-container">
                {canScrollLeft && (
                    <button className="scroll-left-button" onClick={() => scroll("left")}>
                        {/* < char*/}
                        &lt;
                    </button>
                )}

                <div className="movies-wrapper" ref={scrollContainerRef} onScroll={updateScrollButtons}>
                    {/* go through all movies in the list and show them using MoviePic component */}
                    {list.map((movieId) => (
                        <MoviePic key={movieId} id={movieId} />
                    ))}
                </div>

                {canScrollRight && (
                    <button className="scroll-right-button" onClick={() => scroll("right")}>
                        {/* > char*/}
                        &gt;
                    </button>
                )}
            </div>
        </div>
    );
};

export default MovieList;
