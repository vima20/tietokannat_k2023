const express = require('express');
const router = express.Router();
const user = require('../models/user_model');

router.get('/', function (request, response) {
  user.getAll(function (err, dbResult) {
    if (err) {
      response.json(err);
    }
    else {
      response.json(dbResult);
    }
  })
});

router.post('/',
  function (request, response) {
    user.add(request.body, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        //response.json(dbResult);
        response.json(dbResult.affectedRows);
      }
    });
  });

module.exports = router;