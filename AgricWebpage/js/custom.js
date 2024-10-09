
function fetchNodeData(nodeID, temperatureID, humidityID, moistureID) {
    $.ajax({
        url: 'retrievelast.php',
        type: 'GET',
        data: { nodeID: nodeID },
        success: function(data) {
            // Assuming the PHP script returns data as "temperature,humidity,moisture"
            const [temperature, humidity, moisture] = data.split(',');

            // Update the corresponding divs with the fetched data
            document.getElementById(temperatureID).textContent = `Temperature: ${temperature} °C`;
            document.getElementById(humidityID).textContent = `Humidity: ${humidity} %`;
            document.getElementById(moistureID).textContent = `Soil Moisture: ${moisture}`;
        }
    });
}

// Function to fetch data for all nodes every 30 seconds
function fetchAllNodeData() {
    fetchNodeData(0, 'node1-temperature', 'node1-humidity', 'node1-moisture');
    fetchNodeData(1, 'node2-temperature', 'node2-humidity', 'node2-moisture');
    fetchNodeData(2, 'node3-temperature', 'node3-humidity', 'node3-moisture');
    fetchNodeData(3, 'node4-temperature', 'node4-humidity', 'node4-moisture');
}

// Initial fetch
fetchAllNodeData();

// Fetch data every 30 seconds
setInterval(fetchAllNodeData, 30000);
