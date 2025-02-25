#ifndef pages_H
#define pages_H

#include <Arduino.h>

String principal = R"rawliteral(
    <html>
      <head>
        <style>
          body {
            background-color: #121212;
            color: #ffffff;
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
            flex-direction: column;
          }
          h1 {
            color: #00bcd4;
            animation: fadeIn 1s ease-in-out;
            margin-bottom: 20px;
          }
          p a {
            color: #00bcd4;
            text-decoration: none;
            font-size: 1.2em;
            animation: fadeIn 1s ease-in-out;
            transition: color 0.3s;
          }
          p a:hover {
            color: #4dd0e1;
          }
          @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
          }
        </style>
      </head>
      <body>
        <h1>ESP32 Configuration</h1>
        <p><a href='/wifi'>Configure Wi-Fi</a></p>
        <p><a href='/mqtt'>Configure MQTT</a></p>
        <p><a href='/firmware'>Upload Firmware</a></p> <!-- Link para a página de upload -->
      </body>
    </html>
  )rawliteral";

String wifi = R"rawliteral(
    <html>
      <head>
        <style>
          body {
            background-color: #121212;
            color: #ffffff;
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
            flex-direction: column;
          }
          h1 {
            color: #00bcd4;
            animation: fadeIn 1s ease-in-out;
            margin-bottom: 20px;
          }
                    a {
            color: #00bcd4;
            text-decoration: none;
            font-size: 1.2em;
            margin-top: 15px;
            display: inline-block;
          }
          a:hover {
            color: #4dd0e1;
          }
          form {
            display: inline-block;
            animation: fadeIn 1s ease-in-out;
          }
          input[type='text'], input[type='password'] {
            padding: 10px;
            margin: 5px 0;
            border: 1px solid #00bcd4;
            border-radius: 5px;
            background-color: #1e1e1e;
            color: #ffffff;
            width: 300px;
          }
          input[type='submit'] {
            background-color: #00bcd4;
            color: #ffffff;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            cursor: pointer;
            transition: background-color 0.3s;
            width: 320px;
          }
          input[type='submit']:hover {
            background-color: #4dd0e1;
          }
          @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
          }
        </style>
      </head>
      <body>
        <h1>Configure Wi-Fi</h1>
        <form action='/wifi' method='POST'>
          <input type='text' name='ssid' placeholder='SSID'><br>
          <input type='password' name='password' placeholder='Password'><br>
          <input type='submit' value='Save'>
        </form>
        <a href="/">Go Back</a>
      </body>
    </html>
  )rawliteral";

String mqtt = R"rawliteral(
    <html>
      <head>
        <style>
          body {
            background-color: #121212;
            color: #ffffff;
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
            flex-direction: column;
          }
          h1 {
            color: #00bcd4;
            animation: fadeIn 1s ease-in-out;
            margin-bottom: 20px;
          }
          form {
            display: inline-block;
            animation: fadeIn 1s ease-in-out;
          }
          input[type='text'], input[type='password'] {
            padding: 10px;
            margin: 5px 0;
            border: 1px solid #00bcd4;
            border-radius: 5px;
            background-color: #1e1e1e;
            color: #ffffff;
            width: 300px;
          }
                    a {
            color: #00bcd4;
            text-decoration: none;
            font-size: 1.2em;
            margin-top: 15px;
            display: inline-block;
          }
          a:hover {
            color: #4dd0e1;
          }
          input[type='submit'] {
            background-color: #00bcd4;
            color: #ffffff;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            cursor: pointer;
            transition: background-color 0.3s;
            width: 320px;
          }
          input[type='submit']:hover {
            background-color: #4dd0e1;
          }
          @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
          }
        </style>
      </head>
      <body>
        <h1>Configure MQTT</h1>
        <form action='/mqtt' method='POST'>
          <input type='text' name='mqtt_server' placeholder='MQTT Server'><br>
          <input type='text' name='mqtt_user' placeholder='User'><br>
          <input type='password' name='mqtt_password' placeholder='Password'><br>
          <input type='submit' value='Save'>
        </form>
        <a href="/">Go Back</a>
      </body>
    </html>
  )rawliteral";

String uploadFW = R"rawliteral(
    <html>
      <head>
        <style>
          body {
            background-color: #121212;
            color: #ffffff;
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
            flex-direction: column;
          }
          h1 {
            color: #00bcd4;
            animation: fadeIn 1s ease-in-out;
            margin-bottom: 20px;
          }
          form {
            display: inline-block;
            animation: fadeIn 1s ease-in-out;
          }
          input[type='file'] {
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #00bcd4;
            border-radius: 5px;
            background-color: #1e1e1e;
            color: #ffffff;
            font-size: 1.1em;
          }
          input[type='submit'] {
            background-color: #00bcd4;
            color: #ffffff;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            cursor: pointer;
            transition: background-color 0.3s;
            width: 320px;
          }
          input[type='submit']:hover {
            background-color: #4dd0e1;
          }
          a {
            color: #00bcd4;
            text-decoration: none;
            font-size: 1.2em;
            margin-top: 15px;
            display: inline-block;
          }
          a:hover {
            color: #4dd0e1;
          }
          @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
          }
        </style>
      </head>
      <body>
        <h1>Upload Firmware</h1>
        <form action='/upload' method='POST' enctype='multipart/form-data'>
          <input type='file' name='firmware' accept='.bin'><br>
          <input type='submit' value='Upload Firmware'>
        </form>
        <a href="/">Go Back</a>
      </body>
    </html>
  )rawliteral";

#endif