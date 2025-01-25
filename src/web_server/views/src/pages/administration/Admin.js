import './Admin.css';
import React, { useState } from "react";
import MovieAutocomplete from '../../components/MovieAutocomplete';

function Admin() {
   const [id, setId] = useState('');

  // the html of the page + components
  return (
    <MovieAutocomplete setId={setId}/>
  );
}



export default Admin;