const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
const path = require('path'); // For serving static files

// Import the router for handling API routes and images router

const api = require('./routes/api');
const imagesRouter = require('./routes/images');
const videoRouter = require('./routes/videos');


// Load environment variables from the configuration file based on the current environment
require('custom-env').env(process.env.NODE_ENV, './config');

// Connect to MongoDB using the connection string from the environment variables
mongoose.connect(process.env.CONNECTION_STRING, {
  useNewUrlParser: true,
  useUnifiedTopology: true
});

// Initialize the Express application
const app = express();
const corsOptions = {
    exposedHeaders: ['Location'], // Expose 'Location' header to the client
};

app.use(cors(corsOptions));
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.json());

// Use the API router for all routes starting with /api
app.use('/api', api);

// Use the images router
app.use('/images', imagesRouter);
app.use('/videos', videoRouter);

// Serve the React app from the build directory
const reactAppPath = path.join(__dirname, './views/build');
app.use(express.static(reactAppPath));

// Route to serve React's index.html for any unmatched routes (excluding /api, /images, etc.)
app.get('*', (req, res) => {
  res.sendFile(path.join(reactAppPath, 'index.html'));
});

// Catch-all route for invalid API or other requests
app.all('/api/*', (req, res) => {
  res.status(404).json({ error: 'Not Found: invalid API URL request' });
});

// A variable with the IP and port of the recommendation system
const recommendConString = process.env.RECOMMEND_CON_STRING;

// Run the server on the port from the configuration file
app.listen(process.env.PORT);

module.exports = { recommendConString };
