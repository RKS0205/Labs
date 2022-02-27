# FRUIT API

FRUIT API is a RESTFUL API that provides access to a database of fruit information.

## TOOLS

- Mongoose : https://github.com/cesanta/mongoose
This library was used to make the server
- mjson : https://github.com/cesanta/mjson
Used to parse files written in json format
- MySQL server and client : ```apt-get install default-libmysqlclient-dev``` ```apt-get install mysql-server```
Used to store the api data
- Curl : ```apt-get install libcurl4-openssl-dev```
Used to request data from https://fruityvice.com/api/fruit/all/

## HOW TO RUN

- Run ```make``` to build the server and cli
- Run ```./api``` to start the server and ```./cli``` to use the CLI
  
## WHAT IT DOES

### SERVER

Upon starting the server, it will attempt to request data from the API ```https://fruityvice.com/api/fruit/all/``` with the Curl library.
After collecting the data in json format, it will then connect to a MySQL server with the user "root" and password "root" (this can be changed in the header).
Once the connection to the MySQL server is established, it will then store the parsed information into a database.
After that, the mongoose library is used to create a webserver with ip ```localhost:8000```.
When the webserver receives a request, it'll examine the URI and HTTP method in order to return an appropriate response.
All requests and responses are logged in the ```server.log``` file.
In order to close the webserver, simply use CTRL-C in the terminal.

### CLI

After executing the program, it will wait for input in terminal.
Use the command ```help``` in order to see the available commands.
This CLI is capable of returning useful information by parsing the ```server.log``` file, can be used to reset the database to the original APIs state and can show all entries currently in the MySQL Database.
