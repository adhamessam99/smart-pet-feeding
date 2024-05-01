import React, { useEffect, useState } from 'react';
import mqtt from 'mqtt';

var options={port:8884,protocol:'wss',username:'Device001',password:'Device001'}

const Status = () => {
  const [ultrasonicData, setUltrasonicData] = useState('');

  useEffect(() => {
    // Connect to the HiveMQ broker
    
    const client = mqtt.connect('wss://ae22b2dd89f2466cac9dda2003ef03af.s2.eu.hivemq.cloud:8884/mqtt',options);
     
    
    // Subscribe to the MQTT topic
    client.on('connect', () => {
        
    console.log('Connected to HiveMQ broker');
    client.subscribe('testTopic');
    
    });
        
    // Handle incoming messages on the subscribed topic
    client.on('message', (topic, message) => {
        
      setUltrasonicData(message.toString());
    });

    // Clean up the MQTT connection when the component is unmounted
    return () => {
      client.end();
      
    };
  }, []);
  
    

  return (
    <div>
      <h2>check the container status below:</h2>
      <label>Current Status: {ultrasonicData}</label>
      

    </div>
  );
};

export default Status ;
