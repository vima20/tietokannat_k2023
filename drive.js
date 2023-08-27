const express = require('express');
const router = express.Router();
const drive = require('../models/drive_model');

router.get('/',
  function (request, response) {
    drive.getAll(function (err, dbResult) {
      if (err) {
        response.json(err);
      }
      else {
        response.json(dbResult);
      }
    })
  });

router.get('/:username',
  function (request, response) {
    drive.getByusername(request.params.username, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        response.json(dbResult);
      }
    })
  });

  router.get('/avg/:username', function (request, response) {
    drive.getAvgByusername(request.params.username, function (err, dbResult) {
      if (err) {
        response.status(500).json({ error: 'Internal Server Error' });
      } else if (dbResult.length === 0) {
        response.status(404).json({ error: 'User not found!' });
      } else if (dbResult[0].average === null) {
        response.status(404).json({ error: 'No drives found!' });
      } else {
        // Convert the minimum distance to a number if it's a string
        if (typeof dbResult[0].average === 'string') {
          dbResult[0].average = parseFloat(dbResult[0].average);
        }
        response.json(dbResult);
      }
    });
  });
        
  router.get('/min/:username', function (request, response) {
    drive.getMinByusername(request.params.username, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        // Convert the minimum distance to a number if it's a string
        if (typeof dbResult[0].minimum === 'string') {
          dbResult[0].minimum = parseFloat(dbResult[0].minimum);
        }
        response.json(dbResult);
      }
    });
  });
  
  router.get('/max/:username',
  function (request, response) {
    drive.getMaxByusername(request.params.username, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        response.json(dbResult);
      }
    })
  });

router.post('/',
  function (request, response) {
    drive.add(request.body, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        //response.json(dbResult);
        response.json(dbResult.affectedRows);
      }
    });
  });

router.delete('/:driveId',
  function (request, response) {
    drive.delete(request.params.driveId, function (err, dbResult) {
      if (err) {
        response.json(err);
      } else {
        response.json(dbResult.affectedRows);
      }
    });
  });


module.exports = router;