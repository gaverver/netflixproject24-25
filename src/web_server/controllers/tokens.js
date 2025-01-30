const tokensServices = require('../services/tokens')

// check if there a user registered in the system with the username and password got in the body
const createToken = async (req, res) => {
    
    // for comfort
    const { username, password } = req.body;

    // username is a required field in the request body
    if (username === undefined) {
        return res.status(400).json({ error:'username is required' });
    }

    // validate the input
    if (typeof username !== 'string') {
        return res.status(400).json({ error: 'Invalid data: username must be a string' });
    }

    // password is a required field in the request body
    if (password === undefined) {
        return res.status(400).json({ error:'password is required' });
    }

    // validate the input
    if (typeof password !== 'string') {
        return res.status(400).json({ error: 'Invalid data: password must be a string' });
    }
    
    // avoid server crushes
    try {
        // create a new token for the user
        const token = await tokensServices.createToken(username, password);

        // only if it didn't found
        if (!token) {
            return res.status(404).json({ error:'Invalid username or password'})
        }
        // else, we found such user, and we return the token we created for him
        return res.status(201).json({ token });
    } catch (error) {
        // server error (to avoid crushing)
        return res.status(500).json({ error:'Internal Server Error' });
    }
};

// gets a token and returns the id of the user beind it
const getIdFromToken = async (req, res) => {
    const token = req.params.token;
    try {
        // gets the id
        const userId = await tokensServices.getIdFromToken(token);
        // if verification failed, because the token isn't good
        if (!userId) {
            return res.status(404).json({ error:'Invalid token'});s
        }
        // else, the token is fine and the userId is returned
        return res.status(200).json({ userId });
    } catch(error) {
        // server error (to avoid crushing)
        return res.status(500).json({ error:'Internal Server Error' });
    }
};

const isLoggedIn = async (req, res, next) => {
    if (req.headers.authorization) {
        const token = req.headers.authorization.split(" ")[1];
        if (await tokensServices.isLoggedIn(token)) {
            return next();
        } else {
            return res.status(401).send("Invalid Token");
        }
    } else
        return res.status(403).send('Token required');
}


const isAdmin = async (req, res, next) => {
    if (req.headers.authorization) {
        const token = req.headers.authorization.split(" ")[1];
        x = await tokensServices.privilegeLevelByToken(token)
        if (x === 1) {
            return next();
        } else {
            return res.status(403).send("Blocked access");
        }
    } else
        return res.status(403).send('Token required');
}

module.exports = { createToken,  isLoggedIn, isAdmin, getIdFromToken }