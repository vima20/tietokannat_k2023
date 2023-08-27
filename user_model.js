const db = require('../database');
const bcrypt = require('bcryptjs');

const saltRounds = 10;

const user = {
  getAll: function(callback) {
    return db.query('SELECT * FROM users', callback);
  },
  add: function(newUser, callback) {
    bcrypt.hash(newUser.pin, saltRounds, function(err, hashedPin) {
    if (err) {
        return callback(err);
    }
    return db.query(
        'INSERT INTO users (username, pin) VALUES (?, ?)',
        [newUser.username, hashedPin],
          callback
        );
      });
    }
};

module.exports = user;
