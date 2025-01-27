const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
const api = require('./routes/api');
const imagesRouter = require('./routes/images');
const videoRouter = require('./routes/videos');

// Load environment variables
require('custom-env').env(process.env.NODE_ENV, './config');

// Connect to MongoDB
mongoose.connect(process.env.CONNECTION_STRING, {
    useNewUrlParser: true,
    useUnifiedTopology: true
});

// Initialize the Express app
var app = express();

// CORS setup to expose the 'Location' header
const corsOptions = {
    exposedHeaders: ['Location'], // Expose 'Location' header to the client
};

app.use(cors(corsOptions));
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.json());

// Use the API router
app.use('/api', api);

// Use the images router
app.use('/images', imagesRouter);
app.use('/videos', videoRouter);

// Catch-all route
app.all('*', (req, res) => {
    res.status(404).json({ error: 'Not Found: invalid url request' });
});

// Server listening on configured port
const recommendConString = process.env.RECOMMEND_CON_STRING;
app.listen(process.env.PORT);

module.exports = { recommendConString };
