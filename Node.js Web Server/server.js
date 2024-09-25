const express = require('express');
const path = require('path');
const session = require('express-session');
const bodyParser = require('body-parser');
const { WebSocketServer } = require('ws');

const app = express();
const port = process.env.PORT || 3000;

let isActivated = false; // Tracks ESP32 activation state
let ledState = false; // Tracks LED state

app.use(bodyParser.urlencoded({ extended: true }));

app.set('trust proxy', 1);

app.use(session({
    secret: 'your_secret_key_here',
    resave: false,
    saveUninitialized: true,
    cookie: {
        secure: true,
        httpOnly: true
    }
}));

const USERNAME = process.env.USERNAME;
const PASSWORD = process.env.PASSWORD;

app.get('/', (req, res) => {
  if (req.session.loggedIn) {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
  } else {
    res.redirect('/login');
  }
});

app.get('/login', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'login.html'));
});

app.post('/login', (req, res) => {
  const { username, password } = req.body;

  if (username === USERNAME && password === PASSWORD) {
    req.session.loggedIn = true;
    res.redirect('/');
  } else {
    res.send('Invalid username or password. <a href="/login">Try again</a>');
  }
});

app.get('/logout', (req, res) => {
  req.session.destroy();
  res.redirect('/login');
});

app.use(express.static('public'));

const server = app.listen(port, () => {
    console.log(`Server running on https://${process.env.PROJECT_DOMAIN}.glitch.me`);
});

const wss = new WebSocketServer({ server });

wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            if (data.action === 'activateESP32') {
                isActivated = true;
                ledState = !ledState; // Toggle the LED state
                
                // Send state change only
                wss.clients.forEach(client => {
                    client.send(JSON.stringify({ activate: isActivated, led: ledState }));
                });
            }
        } catch (error) {
            console.log("Non-JSON message received: ", message);
        }
    });
});
