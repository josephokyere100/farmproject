<?php
// Database connection details
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp_now_agric_project";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the parameters from the URL
$nodeID = (int) $_GET['nodeID'];
$temperature = (float) $_GET['temperature'];
$humidity = (float) $_GET['humidity'];
$moisture = (float) $_GET['moisture'];

// Prepare the SQL query
$sql = "INSERT INTO sensor_data (NodeID, Temperature, Humidity, Moisture)
        VALUES (?, ?, ?, ?)";

$stmt = $conn->prepare($sql);
$stmt->bind_param("iddd", $nodeID, $temperature, $humidity, $moisture);

// Execute the query
if ($stmt->execute()) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$stmt->close();
$conn->close();
?>