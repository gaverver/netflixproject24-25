// Import all the required libraries
const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
// Import the router for handling API routes and images router
const api = require('./routes/api');
const imagesRouter = require('./routes/images')
const videoRouter = require('./routes/videos')
// Load environment variables from the configuration file based on the current environment
require('custom-env').env(process.env.NODE_ENV, './config');
// Connect to MongoDB using the connection string from the environment variables
mongoose.connect(process.env.CONNECTION_STRING,
{ useNewUrlParser: true,
useUnifiedTopology: true });
// Initialize the Express application
var app = express();
app.use(cors());
app.use(bodyParser.urlencoded({extended : true}));
app.use(express.json());
// Use the API router for all routes starting with /api
app.use('/api', api);

// Use the images router
app.use('/images', imagesRouter);
app.use('/videos', videoRouter);

// a variable with the ip and port of the recommendation system
const recommendConString = process.env.RECOMMEND_CON_STRING
// load the avatars for the clients.
app.use(express.static('../../data/avatars'))
// Serve the React app from the build directory
app.use(express.static(path.join(__dirname, './views/build')));

// Handle all unmatched routes and serve index.html
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, './views/build', 'index.html'));
});

// run the server on the port from the configuration file
app.listen(process.env.PORT);

module.exports = {recommendConString}