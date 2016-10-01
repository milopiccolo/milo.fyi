var express = require('express');
var app = express();

app.get('/', function(req, res) {
  res.send("official ranking of milos: <br> 1. milo the rapper<br> 2. me<br> 3. other milos<br><br><br>-inf. milo yiannopoulos")
  console.log('served a request!')
})


express.listen(80, function()) {
  console.log('starting up!')
}