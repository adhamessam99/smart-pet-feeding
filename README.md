# smart-pet-feeding

## Overview:

The Smart Pet Feeding System is an IoT (Internet of Things) project designed to automate the process of feeding pets while also providing real-time monitoring of food levels in the feeding container.
The system utilizes sensors to measure the weight of the food container.
i make the esp8266 publish the sensor reading to the hivemq and in realtime the UI (using react.js) subscribe to the hivemq and take the reading and display the status of the container

## Features:

it communicates with a HiveMQ broker via MQTT over WebSocket (wss) for data transmission, 
The user interface (UI) is built using React to provide a convenient way for users to monitor the status of the feeding container and remotely control the feeding process.

