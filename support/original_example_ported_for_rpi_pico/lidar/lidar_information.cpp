#include <Arduino.h>
#include "node_lidar.h"


bool get_version_t()
{
    printf("encry check version\n");
    //下发获取版本的指令
    Serial2.write(get_version,4);
    //获取回复
    if(node_lidar.lidar_data_processing.waitResponseHeader(0x64)==RESULT_OK)
    {
        printf("version=%d,%d\n",node_lidar.lidar_version[0],node_lidar.lidar_version[1]);
        return true;
        
    }else{
        printf("check version wrong\n");
        return false;
    }
}

