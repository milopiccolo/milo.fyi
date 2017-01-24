var express = require('express');
var app = express();
var port = (process.argv.length >= 3 ? process.argv[2] : 3000);
console.log(port);

var bodyParser = require('body-parser');
app.use(bodyParser());

app.use(express.static(__dirname+'/dist',{extensions:['html']}));

app.set('views', __dirname+'/views');
app.set('view engine', 'ejs');

// app.get('/', function(req, res) {
  // res.send('Michael LoPiccolo<br><br> I\'ll make a real website sometime, but until then:<br> <a href="static/">My paper reviewing fairness & nondiscrimination efforts in big data</a><br><a href="');
    //'I hope to ');
  //res.send('official ranking of milos: <br> 1. milo the rapper<br> 2. me<br> 3. other milos<br><br><br>-&#8734;. milo yiannopoulos<br><br> under construction, contact <a href="mailto:me@milo.fyi">me@milo.fyi</a> for issues');
  // console.log('served a request!');
// });

app.listen(port, function() {
  console.log('starting up on port ' + port + '!');
});

app.get('/hamming', function(req, res) {
  res.render('hamming');
});

var exec = require('child_process').exec;
var fs = require('fs');
var runid = 0;
var escape = require('escape-html');

app.post('/hamming', function(req, res) {
  var output = "";
  var file = __dirname+'/tmp/'+runid;
  fs.writeFile(file, req.body.data);
  var command = __dirname+'/progs/hamming ' + file;
  runid++;
  if(req.body.distance && /[0-9]*/.test(req.body.distance)) {
    command += " -d " + req.body.distance;
  }
  exec(command, (error, stdout, stderr) => {
    var out = "";
    if(error) {
      out = "There was an error. Output:\n\n" + stdout;
    }
    else {
      out = stdout;
    }
    var eout = escape(out).replace(/\n/g, "<br>");
    res.render('hamming', {escapedoutput:eout});
  });
});
