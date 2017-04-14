var express = require('express');
var app = express();
var port = (process.argv.length >= 3 ? process.argv[2] : 3000);
console.log(port);

var multer = require('multer');


app.use(express.static(__dirname+'/dist',{extensions:['html']}));

app.set('views', __dirname+'/views');
app.set('view engine', 'ejs');

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

app.get('/hamming', function(req, res) {
  res.render('hamming');
});

var upload = multer({
  dest: "tmp/",
  limits: {
    fileSize: 1000 * 1024 * 1024, // TODO check this
    fields: 2,
    fieldSize: 1000 * 1024 * 1024,
    files: 1
  }
});

app.use('/hamming', upload.any());
app.post('/hamming', function(req, res) {
  var filename;
  
  var out = '';
  if(req.files.length==0) {
    filename = 'tmp/'+runid;
    fs.writeFile(filename, req.body.text);
    runid++;
    out += "Reading from text field...\n\n"
  }
  else {
    filename = req.files[0].path;
    out += "Reading from file...\n\n"
  }
  var command = __dirname+'/progs/hamming ' + filename;
  if(req.body.maxhd) {
    command += ' -d' + req.body.maxhd;
  }
  exec(command, (error, stdout, stderr) => {
    if(error) {
      out += "There was an error. Output:\n\n" + stdout;
    }
    else {
      out += stdout;
    }
    var eout = escape(out).replace(/\n/g, "<br>");
    res.render('hamming', {escapedoutput:eout});

    exec("rm " + filename);
    for(i in req.files) {
      var rmcommand = "rm " + req.files[i].path;
      exec(rmcommand);
    }
  });
});
