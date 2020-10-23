# 用于加密和解密chrome cookie  基于chromium 78 版本的加密解密库编写，其他版本未测试
#for decode and encode chrome cookie based chromium 78 source code  
1.git clone https://github.com/zhbn/chrome-chromium-cookie-handle-node-addon.git  
2.cd chrome-chromium-cookie-handle-node-addon  
3.npm install   
4.node-gyp rebuild  
5.npm test   
5.addon build/Release/cookie-handle.node     
