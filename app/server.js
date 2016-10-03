var express = require('express');
var app = express();
var port = (process.argv.length >= 3 ? process.argv[2] : 3000);
console.log(port);

app.use(express.static('../dist'));

app.get('/', function(req, res) {
  res.send('official ranking of milos: <br> 1. milo the rapper<br> 2. me<br> 3. other milos<br><br><br>-&#8734;. milo yiannopoulos<br><br> under construction, contact <a href="mailto:me@milo.fyi">me@milo.fyi</a> for issues');
  console.log('served a request!');
});

app.listen(port, function() {
  console.log('starting up on port ' + port + '!');
});
