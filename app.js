var express = require('express');
var app = express();
var userAuth = 0;

const { exec } = require('child_process');

const bodyParser = require('body-parser')
app.use(bodyParser.urlencoded({extended:false}))

app.use('/', express.static("./public"));

app.get('/', (req, res) => {
  res.sendFile(__dirname + "/mainpage.html");
  console.log("Move to mainpage");
});

app.get('/mainpage', (req, res) => {
  
  console.log("Move to mainpage");
  res.sendFile(__dirname + "/mainpage.html");
});

app.post('/mainpage', (req, res) => {
  var title = req.body.title;
  var content = req.body.content;
  var titlestr = title.toString();
  var contentstr = content.toString();
  if (titlestr == "" || contentstr == ""){
    res.send("<script>alert('Please put keyword and content both'); window.location.replace('/post');</script>");
  }
  var process = exec(`./dEdit "${titlestr}" "${contentstr}"`);

  process.stdout.on('data', function (data) { 
    console.log(data.toString());
    console.log("dEdit Successfuly Wrote to Database");
  });
    
  process.stderr.on('data', function (data) {
    console.error(data.toString());
    console.log("dEdit failed.");
  });

  console.log(title);
  console.log(content);

  console.log("Move to mainpage");
  res.sendFile(__dirname + "/mainpage.html");
});

app.get('/post', (req, res) => {
  if(userAuth == 0){
    res.send("<script>alert('Only user can write post.'); window.location.replace('/');</script>");
    //res.redirect('/');
  }
  console.log("Move to postpage");
  res.sendFile(__dirname + "/post.html");
});

app.get('/login', (req, res) => {
  console.log("Move to loginpage");
  res.sendFile(__dirname + "/login.html");
});

app.post('/login', (req, res) => {
  var id = req.body.username;
  var pw = req.body.password;
  var idstr = id.toString();
  var pwstr = pw.toString();
  var result = "";

  var process = exec(`./login "${idstr}" "${pwstr}"`);

  process.stdout.on('data', function (data) { 
    result = data.toString();
    console.log("login function Successfuly executed.");

    if(result === "superUser"){
      userAuth = 2;
      console.log(`superUser, code : ${userAuth}`);
    }
    else if(result === "normalUser"){
      userAuth = 1;
      console.log(`normalUser, code : ${userAuth}`);
    }
    else if(result === "unauthorized"){
      userAuth = 0;
      console.log(`unauthorized, code : ${userAuth}`);
    }

    console.log("Move to mainpage");
    res.send(`<script>alert('SetState as ${result}.'); window.location.replace('/');</script>`);
  });
    
  process.stderr.on('data', function (data) {
    console.error(data.toString());
    console.log("login function failed.");
    res.sendFile(__dirname + "/login.html");
  });
});

app.get('/close', (req, res) => {
  userAuth = 0;
  console.log("Wiki Closed.");
  res.redirect('/');
});

app.listen(3000, () => {
  console.log("Connected");
})