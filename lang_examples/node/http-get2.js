const http = require('http');
const https = require('https');
const url = "https://jsonplaceholder.typicode.com/todos/1";
process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;
https.get(url, function(response) {
  var finalData = "";

  response.on("data", function (data) {
    finalData += data.toString();
  });

  response.on("end", function() {
    console.log(finalData.length);
    console.log(finalData.toString());
  });

});
