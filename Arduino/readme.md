<h1>Client</h1>
<p>Author: <a href="https://github.com/itsy-pandacrp">itsy-pandacrp</a></p>
<p align="center">
    <img src="https://img.shields.io/badge/Arduino-1.8.19-008184?style=flat-square&logo=arduino" />
    <img src="https://img.shields.io/badge/Hardware-Arduino%20MKR%20Wifi%201010-CCC?style=flat-square" />
</p>
<ul>
    <li>
        <a href="#Hardware">Hardware</a>
        <ul>
            <li>
                <a href="#Arduino">Arduino</a>
            </li>
            <li>
                <a href="#LCD">LCD</a>
            </li>
            <li>
                <a href="#Temperature">Temperature sensor</a>
            </li>
        </ul>
    </li>
    <li>
        <a href="#Wiring">Wiring</a>
    </li>
    <li>
        <a href="#Config">Config</a>
    </li>
</ul>
<h2>Hardware</h2>
<h3>Arduino</h3>
<p>We used a <b><i>Arduino MKR WiFi 1010</b></i></p>
<p>This board include a WiFi shield that allow the client to connect to the server without any other module</p>
</br>
<h3>LCD</h3>
<p>The lcd device that we used is a <i><b>I2C LCD 16x2</b></i></p>
<p>This kind of device need only 2 pin (SDA and SCL)</p>
</br>
<p>The monitor allow us to see the Arduino's info like (ip, error, temperature, and humidity)</p>
<h3>Temperature sensor</h3>
<p>The sensor that we used is the <b><i>DHT11</b></i></p>
</br>
<p>This kind of sensor get both Temperature and Humidity</p>
</br>
<h2>Wiring</h2>
<p>I2C-SCL : <b>12</b></p>
<p>I2C-SDA : <b>11</b></p>
</br>
<p>DHT-DATA : <b>7</b></p>
<h2>Config</h2>
<p>Before compiling the code, you need to edit the <b><i>config.h</b></b></p>
<p>You don't need to modify anything in <b><i>CUBEIOT.h</b></i>
<p>
    <pre>#define WIFI_SSID "WIFI_SSID" //Network name
#define WIFI_PASS "********" //Network password

#define REQ_SERVER "255.255.255.255" //Server's ip
#define REQ_PORT 0000 //Server's port
#define REQ_SENSORID 12 //Sensor id
#define DHT_DATA 7 //DHT pin
#define DHTTYPE DHT11 //DHT model (DHT11/DHT22)

#define lcd_Adress 0x3F //LCD adress
#define lcd_column 16 //LCD column
#define lcd_row 2 b//LCD row</pre>
