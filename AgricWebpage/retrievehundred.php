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

// Get nodeID from the request
$nodeID = isset($_GET['nodeID']) ? $_GET['nodeID'] : '';

// Prepare the SQL query to retrieve data for the specified nodeID
$sql = "SELECT Temperature, Humidity, Moisture, Time, Date 
        FROM sensor_data 
        WHERE nodeID = ? 
        ORDER BY EntryID DESC 
        LIMIT 100";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $nodeID);
$stmt->execute();

$result = $stmt->get_result();

$data = [];
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $data[] = $row; // Append each row of data to the array
    }
}

// Return data as JSON
echo json_encode($data);

// Close statement and connection
$stmt->close();
$conn->close();
?>
