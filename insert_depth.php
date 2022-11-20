<?php

if(isset($_GET["meeting"]) && (isset($_GET["diepte"]))) {
   $meeting = $_GET["meeting"]; //get meeting_id value from HTTP GET
   $diepte = $_GET["diepte"]; // get diepte value from HTTP GET
   

   $servername = "localhost";
   $username = "root";
   $password = "password";
   $database_name = "api_iot";
   
   
   // Create MySQL connection fom PHP to MySQL server
   $connection = new mysqli($servername, $username, $password, $database_name);
   // Check connection
   if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
   }
   
   $sql = "INSERT INTO waterput (meeting_id, diepte) VALUES ($meeting, $diepte)";

   if ($connection->query($sql) === TRUE) {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $connection->error;
   }

   $connection->close();
} else {
   echo "depth is not set in the HTTP request";
}
?>
