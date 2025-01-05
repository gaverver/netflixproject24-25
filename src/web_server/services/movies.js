const Movie = require('../models/movies')
const User = require('../models/')
const App = require('../app')
const net = require('net')

const createMovie = async (name, description, actors, published, age_limit, creators, categories) => {
    //create movie with required fields
    const movie = new Movie({
        name: name,
        description: description,
        creators: creators,
    })
    //check if 'actors' passed
    if (actors !== undefined) {
        movie.actors = actors
    }
    //check if 'published' passed
    if (published !== undefined) {
        movie.published = published
    }
    //check if 'age_limit' passed
    if (age_limit !== undefined) {
        movie.age_limit = age_limit
    }
    //check if 'categories' passed
    if (categories !== undefined) {
        movie.categories = categories
    }
    return await movie.save()
}

const deleteMovie = async (id) => {
    //delete from mongoDB
    const movie = await Movie.findByIdAndDelete(id);
    if (!movie) return null;
    //delete movie from recommendation system
    const [ip, port] = App.recommendConString.split(':');
    for (const userId of movie.users) {
        const response = await sendMessageToServer(ip, parseInt(port), `DELETE ${userId} ${id}`);
    }

    return movie;
}

const getMovieById = async (id) => {
    return await Movie.findById(id);
}

const updateMovie = async (id, data) => {
    movie = getMovieById(id);
    for (const key in data) {
        movie[key] = data[key]
    }
    return await movie.save();
}

const getRecommendation = async (userId, movieId) => {
    const [ip, port] = App.recommendConString.split(':');
    const response = await sendMessageToServer(ip, parseInt(port), `GET ${userId} ${movieId}`);
    // Remove the "200 Ok\n\n" prefix
    const cleanResponse = response.replace(/^200 Ok\n\n/, '');
}




//helper function to send a message to the server and wait for a response
const sendMessageToServer = (ip, port, message) => {
    return new Promise((resolve, reject) => {
        const client = new net.Socket();

        client.connect(port, ip, () => {
            client.write(message);
        });

        client.on('data', (data) => {
            resolve(data.toString());
            client.destroy();
        });

        client.on('error', (error) => {
            reject(error);
        });

    });
};