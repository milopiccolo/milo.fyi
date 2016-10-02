var express = require('express');
var app = express();
var port = (process.argv.length > 0 ? process.argv[2] : 80);
console.log(port);

app.get('/', function(req, res) {
  res.send("official ranking of milos: <br> 1. milo the rapper<br> 2. me<br> 3. other milos<br><br><br>-inf. milo yiannopoulos");
  console.log('served a request!');
});


app.listen(port, function() {
  console.log('starting up!');
});
