const http = require("http");

const host = '0.0.0.0';
const port = 8000;


const books = JSON.stringify([
    { title: "The Alchemist", author: "Paulo Coelho", year: 1988 },
    { title: "The Prophet", author: "Kahlil Gibran", year: 1923 }
]);

const authors = JSON.stringify([
    { name: "Paulo Coelho", countryOfBirth: "Brazil", yearOfBirth: 1947 },
    { name: "Kahlil Gibran", countryOfBirth: "Lebanon", yearOfBirth: 1883 }
]);

var fs = require('fs');
var index;

const requestListener = function (req, res) {

    console.log("received request !");

    res.setHeader("Content-Type", "application/json");
    switch (req.url) {
        case "/books":
            res.writeHead(200);
            res.end(books);
            break
        case "/authors":
            res.writeHead(200);
            res.end(authors);
            break
        case "/test":
            res.writeHead(200, {'Content-Type': 'text/plain'});
            res.end("This is the response");
            break
         case "/":
            res.writeHead(200, {'Content-Type': 'text/html'});
            res.end(index);
            break
    }
};

const server = http.createServer(requestListener);
server.listen(port, host, function() {
    console.log("Server is running on http://${host}:${port}");

    // this is not a Efficiend web server index.html not reload on changes...
    index = fs.readFileSync('index.html');
});
