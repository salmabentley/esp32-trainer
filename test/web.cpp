    // if(!LittleFS.begin()){
    //     Serial.println("LittleFS Mount Failed");
    //     return;
    // }

    // WiFi.begin(SSID, PASSWORD);
    // while (WiFi.status() != WL_CONNECTED) { delay(1000); Serial.println("Connecting..."); }
    // Serial.println(WiFi.localIP());

    // //routes
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    //     request->send(LittleFS, "/dashboard.html", String(), false, processor);
    // });
    // server.on("/training", HTTP_GET, [](AsyncWebServerRequest *request){
    //     request->send(LittleFS, "/training.html", String(), false, processor);
    // });
    // server.on("/history", HTTP_GET, [](AsyncWebServerRequest *request){
    //     request->send(LittleFS, "/history.html", String(), false, processor);
    // });

    // server.begin();