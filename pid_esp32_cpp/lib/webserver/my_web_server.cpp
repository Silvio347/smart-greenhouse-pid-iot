#include <my_web_server.h>

WebServer server(80);

void myDelayMillis(uint16_t tempo)
{
  uint32_t startTime = millis();

  while ((millis() - startTime) < tempo)
    ;
}

void createAP()
{
  WiFi.softAP(SSID_ESP, PASSWORD_ESP);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP created! " + IP.toString());
}

void connectWiFi()
{
  WiFi.begin(ssid, password);

  uint8_t contador_wifi = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    contador_wifi++;
    if (contador_wifi >= TIMEOUT_WIFI)
    {
      createAP();
      myDelayMillis(50);
      contador_wifi = 0;
      break;
    }
    myDelayMillis(500);
  }

  if (WiFi.isConnected())
  {
    String ip = WiFi.localIP().toString();
    Serial.println("IP " + ip);
  }
  setup_web_server();
}

void setup_web_server()
{
  server.on("/", HTTP_GET, handleRoot);
  server.on("/wifi", HTTP_GET, handleWiFiForm);
  server.on("/mqtt", HTTP_GET, handleMQTTForm);
  server.on("/firmware", HTTP_GET, handleFirmwareUpload);
  server.on("/upload", HTTP_POST, []() {}, handleUploadFirmware);
  server.begin();
  serverStartTime = millis();
}

// Main page with links to settings
void handleRoot()
{
  String html = R"rawliteral(
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
  server.send(200, "text/html", html);
}

// Page to configure Wi-Fi
void handleWiFiForm()
{
  String html = R"rawliteral(
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
  server.send(200, "text/html", html);

  if (server.method() == HTTP_POST)
  {
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");

    ssid = newSSID;
    password = newPassword;

    WiFi.disconnect();
    connectWiFi();
    server.send(200, "text/html", "<html><body><h1>Wi-Fi updated. Rebooting...</h1></body></html>");
  }
}

// Page to configure MQTT
void handleMQTTForm()
{
  String html = R"rawliteral(
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
  server.send(200, "text/html", html);

  if (server.method() == HTTP_POST)
  {
    String newMQTTServer = server.arg("mqtt_server");
    String newMQTTUser = server.arg("mqtt_user");
    String newMQTTPassword = server.arg("mqtt_password");

    mqtt_server = newMQTTServer;
    mqtt_user = newMQTTUser;
    mqtt_password = newMQTTPassword;

    reconnectMQTT();
    server.send(200, "text/html", "<html><body><h1>MQTT updated!</h1></body></html>");
  }
}

// Process firmware upload
void handleUploadFirmware()
{
  HTTPUpload &upload = server.upload();

  if (upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if (!filename.endsWith(".bin"))
    {
      server.send(400, "text/html", "<h1>Invalid file type. Only .bin files are allowed.</h1>");
      return;
    }

    // Start the firmware update process
    if (!Update.begin(UPDATE_SIZE_UNKNOWN))
    {
      server.send(500, "text/html", "<h1>Failed to begin update!</h1>");
      return;
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    // Write firmware data to the device
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      server.send(500, "text/html", "<h1>Failed to write update!</h1>");
      return;
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (Update.end(true))
    {
      server.send(200, "text/html", "<h1>Firmware uploaded and updated successfully!</h1>");
      ESP.restart();
    }
    else
    {
      server.send(500, "text/html", "<h1>Failed to finalize update!</h1>");
    }
  }
}

// Upload firmware Page
void handleFirmwareUpload()
{
  String html = R"rawliteral(
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
  server.send(200, "text/html", html);
}