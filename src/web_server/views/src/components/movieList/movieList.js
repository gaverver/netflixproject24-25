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
