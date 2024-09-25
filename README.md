## ESP32 WebSocket LED Control Project

This project enables remote control of an LED connected to an ESP32 microcontroller using WebSockets. It consists of two main components: the ESP32 firmware and a Node.js-based web server, which facilitates real-time communication between the ESP32 and a web interface.

### Features:
- **WebSocket Communication**: Secure WebSocket (WSS) for real-time communication.
- **Remote LED Control**: Toggle an LED remotely through a web interface.
- **Secure Login System**: Protect access to the control interface with a username and password.
- **HTTPS Support**: Secure communication between the ESP32 and the server.
- **Optimized Network Usage**: Minimizes unnecessary data transmission to reduce network load.
- **Cross-Platform Compatibility**: The server can be deployed on platforms like [Glitch](https://glitch.com) or any Node.js hosting service.

### Project Structure:
The project is split into two parts:
1. **ESP32 Firmware**: Code for the ESP32 to handle WebSocket communication and control the LED.
2. **Node.js Web Server**: Code to run a WebSocket server and provide a web-based interface for controlling the ESP32.

### How to Set Up:

#### 1. **Download Project Files**:
   You need to download the files for **both** the ESP32 firmware and the Node.js server.

   - **ESP32 Firmware**: This contains the code for the ESP32. Upload it to your microcontroller using [PlatformIO](https://platformio.org/).
   - **Web Server**: This contains the Node.js code for hosting the WebSocket server and the web interface. You will deploy it on a platform that supports Node.js, such as [Glitch](https://glitch.com).

#### 2. **Set Up ESP32 Firmware**:

   1. **Download and Install PlatformIO**: 
      This project uses PlatformIO, which can be installed as an extension in Visual Studio Code or via the command line. Follow [PlatformIO installation instructions](https://platformio.org/install) if you haven't already.

   2. **Configure Wi-Fi and WebSocket Server**:
      In the `src/main.cpp` file, replace the placeholders with your own Wi-Fi credentials and server address:
      ```cpp
      const char* ssid = "your_SSID";  // Your WiFi network name
      const char* password = "your_password";  // Your WiFi password
      const char* websocketServer = "your_server_address";  // WebSocket server URL
      ```

   3. **SSL Certificate for HTTPS**:
      To ensure secure communication, you will need to provide the SSL certificate for the WebSocket server. Retrieve the SSL certificate using the following PowerShell command:
      ```bash
      openssl s_client -showcerts -connect your_server_address:443
      ```
      Find the last certificate in the output, and format it as shown in the example below:
      ```cpp
      const char* rootCACertificate = \
      "-----BEGIN CERTIFICATE-----\n" \
      "your_certificate_data_here\n" \
      "-----END CERTIFICATE-----\n";
      ```

      If the certificate is not already in the proper format, you can use tools like ChatGPT to help you format it correctly by pasting the certificate string and asking for formatting assistance.

   4. **Upload Firmware to ESP32**:
      Use PlatformIO to upload the firmware to your ESP32. From the terminal, run the following commands:
      ```bash
      pio run  # Build the project
      pio run --target upload  # Upload the firmware to the ESP32
      ```

#### 3. **Set Up Node.js Web Server**:

   1. **Download and Set Up the Web Server Files**:
      Place the web server files in a separate directory from the ESP32 firmware.

   2. **Install Node.js and Dependencies**:
      Ensure you have Node.js installed on your system or hosting platform. Navigate to the `server` directory and install the required dependencies using:
      ```bash
      npm install
      ```

   3. **Configure Environment Variables**:
      Create a `.env` file in the server directory to store sensitive information, like the login credentials:
      ```bash
      USERNAME=your_login_username
      PASSWORD=your_login_password
      ```

   4. **Deploy the Server**:
      To run the server locally or on a platform like Glitch, use the following command:
      ```bash
      npm start
      ```
      If you're using [Glitch](https://glitch.com), you can upload the server files to your Glitch project. Once the server is running, the WebSocket server URL will be displayed in the **Logs** section at the bottom left corner of the Glitch editor. This is the address you need to enter in your ESP32 firmware as the `websocketServer` URL.

#### 4. **Login and Control the LED**:
   Once the server is running, navigate to your server’s URL in a web browser, log in using the credentials from your `.env` file, and use the interface to control the LED connected to your ESP32.

### Notes:
- **Do Not Mix ESP32 and Server Files**: Ensure that the ESP32 firmware is uploaded only to your ESP32 microcontroller, while the web server code should be deployed on a separate Node.js-compatible server.
- **SSL/TLS Security**: Make sure you retrieve the correct SSL certificate for secure WebSocket communication, and ensure it is properly formatted in the code.
- **Server Requirements**: Use a platform that supports Node.js, such as [Glitch](https://glitch.com) or your own Node.js hosting solution.
