// require the users modle, because I want to search for specific users
const User = require('../models/users');

// return if a user with the username and passowrd below exists or not
const doesUserExist = async (user_name, pass) => {
    // it will return the first user he found with the username and password above, and null if he didn't found any
    return await User.findOne({ username: user_name, password: pass });
};

module.exports = {doesUserExist}