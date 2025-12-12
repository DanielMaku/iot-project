#ifndef HOMEPAGE_H
#define HOMEPAGE_H

// HTML code for the first part of the page (wrapped in F() for storing in flash memory)
String homePagePart1 = F(R"=====( <!DOCTYPE html>
<html lang="en"> <!-- Needed for Screenreaders !-->
<head>
<!-- UTF-8 character set covers most characters in the world -->
  <meta charset="utf-8">
  <!-- Make page respond to screen size !-->
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
 
<!--Include a Title. Used by Search Engines -->
<title> Smart Home System </title>

<style>
   body {      
      background-color: white;
      color: black;
      font-family: Arial;
      text-align: center;
   }
   .flex-Container{
      display: flex;
      flex-direction: column;
      background-color: DodgerBlue;
      align-items: center;
   }
   h1 {
      font-size: 40px;
      font: bold;
      color: black;
   }
   p {
      font-size: 25px;
      font-family: Arial;
      color: black;
   }  
   table {
      margin-top: 20px;
      margin-bottom: 20px;
      border: 1px solid #ddd;
      padding: 15px;
      font-size: 20px;
   }
   th, td {
      padding: 8px;
      text-align: left;
      border-bottom: 1px solid #ddd;
   }
</style>

<script>
    // Function to fetch sensor data from the server
    function updateSensorData() {
        fetch('/sensorData')
        .then(response => response.json())
        .then(data => {
            document.getElementById('temp').innerText = data.temperature;
            document.getElementById('humidity').innerText = data.humidity;
            document.getElementById('distance').innerText = data.distance;
        })
        .catch(error => console.error('Error fetching sensor data:', error));
    }

    // Update the sensor data every  second
    setInterval(updateSensorData, 1000);
</script>
</head>

<body>
    <div class="flex-Container">
    <h1> Smart Home System </h1>
    <p>current sensor readings:</p>
   
    <table>
        <tr>
            <th>Sensor</th>
            <th>Value</th>
            <th>Unit</th>
        </tr>
        <tr>
            <td>Temperature</td>
            <td id="temp"></td>
            <td>°C</td>
        </tr>
        <tr>
            <td>Humidity</td>
            <td id="humidity"></td>
            <td>%</td>
        </tr>
        <tr>
            <td>Distance</td>
            <td id="distance"></td>
            <td>cm</td>
        </tr>
    </table>
    </div>
</body>
</html>
)=====");

// HTML code for the second part (footer, wrapped in F() for storing in flash memory)
String homePagePart2 = F(R"=====( 
<hr>
&copy Daniel Maku
</body>
</html>
)=====");

#endif // HOMEPAGE_H
