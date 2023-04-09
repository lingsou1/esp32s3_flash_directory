/*
接线说明:无

程序说明:使用闪存文件系统(SPIFFS)来对写入文件进行查看,查看相关目录下有多少文件,文件的名称以及文件的大小

注意事项:下面的网址中有很多已经写好的函数可以用
(https://github.com/espressif/arduino-esp32/blob/master/libraries/SPIFFS/examples/SPIFFS_Test/SPIFFS_Test.ino)

函数示例:无

作者:灵首

时间:2023_3_11

*/
#include <Arduino.h>
#include <SPIFFS.h>


String file_name = "/lingsou/myFile.txt";    //定义文件名(notes.txt)以及文件所在地址
String file_folder = "/lingsou";  
String file_folder_new = "/";   //我不知道为什么不能用这些


/*
# brief 对闪存文件进行格式化操作
# param 无
# retval 无
*/
void SPIFFS_Format_init(){
  Serial.print("\nSPIFFS format begin");
  SPIFFS.format();    //文件格式化的函数
  Serial.print("\nSPIFFS format finshed !!!");

}



/*
# brief 启动SPIFFS
# param 无
# retval 无
*/
void SPIFFS_start_init(){
  if(SPIFFS.begin()){
    Serial.print("\nSPIFFS Start!!!");
  }
  else{
    Serial.print("\nSPIFFS Failed to start!!!");
  }
}





/*
# brief   列出对应目录下有多少文件,文件名称,以及文件大小
# param   fs::FS &fs: 这里的 fs 是下面函数中的代称,需要在该参数中输入 SPIFFS,(即以fs替代SPIFFS)
# param   const char * dirname: 不知道为什么不能用宏定义,只能自己输入查询的文件目录,如("/lingsou") 
# param   uint8_t levels :这个参数是什么我也不知道,0是可以使用的,3输出的结果好像也没有问题,是重复调用,查看多级目录码?
# retval  无,但是会通过串口输出一些内容
*/
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}



void setup() {

  Serial.begin(9600);
  Serial.print("...");

  //启动SPIFFS
  SPIFFS_start_init();    

  //写入内容
  File dataFile = SPIFFS.open(file_name,"w");   // 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  dataFile.println("hello world!!!");   //闪存具体的写入内容(向dataFile写入字符串信息)
  dataFile.close();   //关闭文件,打开文件进行需要的操作后要及时关闭文件
  Serial.print("\nFinshed dealing data to SPIFFS\n");

  //输出对应目录下的文件详情
  listDir(SPIFFS,"/lingsou",3);   //这是输出:"/lingsou" 文件夹的下的文件
  listDir(SPIFFS,"/",3);    //这是输出:"/" 文件夹的下的文件(就应该是SPIFFS中所有的文件)

}

void loop() {

}