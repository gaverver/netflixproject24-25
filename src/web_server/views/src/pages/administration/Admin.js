import './Admin.css';
import React, { useState } from "react";
import MovieAutocomplete from '../../components/MovieAutocomplete';
import MovieEditor from '../../components/MovieEditor';

function Admin() {
   const [id, setId] = useState('');

  // the html of the page + components
  return (
    <div>
        <MovieAutocomplete setId={setId}/>
        <MovieEditor id={id}/>
    </div>
  );
}



export default Admin;