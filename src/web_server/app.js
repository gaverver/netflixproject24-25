// Import all the required libraries
const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
// Import the router for handling API routes
const api = require('./routes/api');
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

// Catch-all route for everything that doesn't start with /api
app.all('*', (req, res) => {
    res.status(404).json({ error: 'Not Found: invalid url request' });
});

// a variable with the ip and port of the recommendation system
const recommendConString = process.env.RECOMMEND_CON_STRING
// load the avatars for the clients.
app.use(express.static('../../data/avatars'))
// run the server on the port from the configuration file
app.listen(process.env.PORT);

module.exports = {recommendConString}