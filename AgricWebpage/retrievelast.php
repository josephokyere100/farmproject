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

// Get nodeID from the HTML page
if (isset($_GET['nodeID'])) {
    $nodeID = $_GET['nodeID'];

    // Prepare the SQL query to retrieve the latest entry for the specified nodeID
    // Order by EntryID to get the most recent data
    $sql = "SELECT Temperature, Humidity, Moisture 
            FROM sensor_data 
            WHERE nodeID = ? 
            ORDER BY EntryID DESC 
            LIMIT 1";

    // Prepare and bind statement
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("s", $nodeID);

    // Execute the statement
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Check if any data is returned
    if ($result->num_rows > 0) {
        // Fetch the latest entry
        $row = $result->fetch_assoc();
        
        // Output the sensor data
        echo $row["Temperature"] . "," . $row["Humidity"] . "," . $row["Moisture"];
    } else {
        echo "No data found for nodeID: " . htmlspecialchars($nodeID);
    }

    // Close statement
    $stmt->close();
} else {
    echo "Please provide a nodeID.";
}

// Close connection
$conn->close();
?>
