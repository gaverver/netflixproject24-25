const tokensServices = require('../services/tokens')

// check if there a user registered in the system with the username and password got in the body
const doesUserExist = async (req, res) => {
    
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
        // try to find if there exsist such a user
        const foundUser = await tokensServices.doesUserExist(username, password);

        // only if it didn't found
        if (!foundUser) {
            return res.status(404).json({ error:'user not found'})
        }
        // else, we found such user, and we return the user's id
        return res.status(200).json(foundUser._id);
    } catch (error) {
        // server error (to avoid crushing)
        return res.status(500).json({ error:'Internal Server Error' });
    }
};

module.exports = { doesUserExist }