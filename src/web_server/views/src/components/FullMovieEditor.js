import React, { useState } from "react";
import MovieAutocomplete from './MovieAutocomplete';
import MovieEditor from './MovieEditor';

function FullMovieEditor() {
    const [id, setId] = useState('');
    // the html of the page + components
    return (
        <div>
            <MovieAutocomplete setId={setId}/>
            <MovieEditor id={id} setId={setId}/>
        </div>
    );
}



export default FullMovieEditor;