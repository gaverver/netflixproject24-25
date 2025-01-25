import { useState } from "react";

const MovieEditor = ({id}) => {
    const [name, setName] = useState('');
    const [description, setDescription] = useState('')
    const [actors, setActors] = useState([]);
    const [age_limit, setAge_limit] = useState('');
    const [creators, setCreators] = useState([]);
    const [categories, setCategories] = useState([]);
    const [photo, setPhoto] = useState('');
    if (id === '')
        return null;

    const fetchMovie = async () => {
        try {
            const response = await fetch(`http://localhost:3001/api/movies/${id}`);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        
            const data = await response.json();
            setName(data.name)
            console.log(data.name)
            setDescription(data.description)
            setActors(data.actors)
            setAge_limit(data.age_limit)
            setCreators(data.creators)
            setCategories(data.categories)
            setPhoto(data.photo)

        } catch (error) {
            console.error('Error fetching movies:', error);
        }

    };
    fetchMovie()
    return (
        <div className="movieautocomplete">
                <ul className="edit-list">
                    <li>{name}</li>
                    <li>{description}</li>
                    <li>{actors}</li>
                    <li>{age_limit}</li>
                    <li>{creators}</li>
                    <li>{categories}</li>
                    <li>{photo}</li>
                </ul>
        </div>
    )


};

export default MovieEditor;