#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <thread>

#include "node_lidar.h"
#include "lidar_data_processing.h"
#include "point_cloud_optimize.h"
#include "lidar_information.h"

#define ROZMER 240              // 240 zodpoveda 2.5 x 2.5 cm polickam na ploche 3x3 metre
#define POLROZMER 120
#define DOMETRA 40

#define OTOC_LIDAR 126          // korekcia namontovania lidaru


using namespace std;

node_lidar_t node_lidar;


node_lidar_t::~node_lidar_t(){
  	if(scan_node_buf)
	{
		delete[] scan_node_buf;
		scan_node_buf = NULL;
	}
	if (globalRecvBuffer)
	{
		delete[] globalRecvBuffer;
		globalRecvBuffer = NULL;
	}
	Serial.println("Turn off radar");
	//Serial2.close();
	node_lidar.lidar_status.lidar_ready = false;
	node_lidar.lidar_status.close_lidar = true;
	flushSerial();

}

void flushSerial()
{
	if (!node_lidar.lidar_status.isConnected){
		return;
	}
	
	size_t len = Serial2.available();
	
	if (len)
	{
		uint8_t *buffer = static_cast<uint8_t *>(alloca(len * sizeof(uint8_t)));
		size_t bytes_read = Serial2.readBytes(buffer, len);
	}

	delay(20);
}

bool lidar_state_judgment()
{
	static bool status_judge = false;     //Overall status assessment
	static bool lidar_flush = false;      //Has the radar activation command been issued?
	static bool wait_speed_right = false; //Has speed control information been obtained?
	static bool lidar_start_flag = false; //Radar feedback flag after issuing radar start command

	static uint64_t lidar_status_time = 0; //Time of receiving start or restart command

	if(node_lidar.lidar_status.lidar_ready != node_lidar.lidar_status.lidar_last_status || node_lidar.lidar_status.close_lidar)
	{
		Serial.println("State switching");

		node_lidar.lidar_status.close_lidar = false;
		node_lidar.lidar_status.lidar_last_status = node_lidar.lidar_status.lidar_ready;

		lidar_flush = false;
		wait_speed_right = false;
		lidar_start_flag = false;

		lidar_status_time = millis();
		flushSerial();
	}
	if(node_lidar.lidar_status.lidar_trap_restart)
	{
		Serial.print("Restarting due to abnormal status");
    Serial.println(lidar_status_time);
		
		node_lidar.lidar_status.lidar_trap_restart = false;

		
		wait_speed_right = false;
		lidar_flush = false;
		lidar_start_flag = false;

		lidar_status_time = millis();
		Serial2.write(end_lidar, 4);
    //Serial.print("wrote "); Serial.print(end_lidar[0], HEX); Serial.print(end_lidar[1], HEX); Serial.print(end_lidar[2], HEX); Serial.println(end_lidar[3], HEX);
	}
	if(node_lidar.lidar_status.lidar_ready && !wait_speed_right)
	{
		
		if(millis() - lidar_status_time > 1000 && !lidar_flush)
		{
			switch (node_lidar.lidar_general_info.version)
			{
				case M1C1_Mini_v1:
					Serial.println("V1 version starts the radar");
					Serial2.write(start_lidar,4);
          //Serial.print("wrote "); Serial.print(start_lidar[0], HEX); Serial.print(start_lidar[1], HEX); Serial.print(start_lidar[2], HEX); Serial.println(start_lidar[3], HEX);
					wait_speed_right = true;
					break;
				
				case M1C1_Mini_v2:
					Serial.println("V2 X2 version launch radar");
					Serial2.write(start_lidar,4);
          //Serial.print("wrote "); Serial.print(start_lidar[0], HEX); Serial.print(start_lidar[1], HEX); Serial.print(start_lidar[2], HEX); Serial.println(start_lidar[3], HEX);
					wait_speed_right = true;
					break;
				
				case M1CT_TOF:
					Serial.println("TOF Radar activation");
					Serial2.write(start_lidar,4);
          //Serial.print("wrote "); Serial.print(start_lidar[0], HEX); Serial.print(start_lidar[1], HEX); Serial.print(start_lidar[2], HEX); Serial.println(start_lidar[3], HEX);
					if(!node_lidar.lidar_data_processing.wait_start_reply())
					{
						Serial.println("No startup feedback command read --> Send again");
						Serial2.write(start_lidar,4);
						//Serial.print("wrote "); Serial.print(start_lidar[0], HEX); Serial.print(start_lidar[1], HEX); Serial.print(start_lidar[2], HEX); Serial.println(start_lidar[3], HEX);
					}
					wait_speed_right = true;
					break;
				
				default:
					break;
			}
		}
		node_lidar.lidar_time.lidar_frequence_abnormal_time = millis();
		node_lidar.lidar_time.system_start_time = millis();
	}
	return wait_speed_right;
}

	node_info  local_buf[128];
	size_t     count = 128;
	node_info  local_scan[1000];
	size_t     scan_count = 0;
	int32_t   ans = RESULT_FAIL;

int read_once()
{	
	node_lidar.lidar_time.scan_time_record = millis();

		bool state_jugde = lidar_state_judgment();
		if(state_jugde)
		{
			count = 128;
			ans = node_lidar.lidar_data_processing.waitScanData(local_buf, count);
			if(!IS_OK(ans))
			{
				if(millis()-node_lidar.lidar_time.system_start_time > 3000 )
				{
					if(!node_lidar.lidar_status.lidar_restart_try)
					{
						Serial.println("Try restarting the radar");
						node_lidar.lidar_status.lidar_restart_try = true;
						node_lidar.lidar_status.lidar_trap_restart = true;
					}else{
						Serial.println("@@@Radar is stuck");
						node_lidar.lidar_status.lidar_abnormal_state |= 0x01;
						delayMicroseconds(100);
					}
				}	
			}else{
				node_lidar.lidar_status.lidar_restart_try = false;
				node_lidar.lidar_time.system_start_time = millis();
			}
			for (size_t pos = 0; pos < count; ++pos)
			{
				if (local_buf[pos].sync_flag & LIDAR_RESP_MEASUREMENT_SYNCBIT)
				{
					if ((local_scan[0].sync_flag & LIDAR_RESP_MEASUREMENT_SYNCBIT))
					{
						local_scan[0].stamp = local_buf[pos].stamp;
						local_scan[0].scan_frequence = local_buf[pos].scan_frequence;
		
						/*If the frequency anomaly exceeds 30 seconds, an abnormal state is triggered.*/
						if(node_lidar.lidar_general_info.version == M1CT_TOF)
						{
							if(local_scan[0].scan_frequence > 200 || local_scan[0].scan_frequence < 10)
							{
								if(millis()-node_lidar.lidar_time.lidar_frequence_abnormal_time > 30000)
								{
									node_lidar.lidar_status.lidar_abnormal_state |= 0x02;
								}
							}else{
								node_lidar.lidar_time.lidar_frequence_abnormal_time = millis();
							}
						}
						
						if((node_lidar.lidar_time.scan_time_current - node_lidar.lidar_time.scan_time_record) > 1000)
						{
							/*Serial.print("full----- count=");
              Serial.print(scan_count);
              Serial.print("time=");
              Serial.print(millis());
              Serial.print("frequence=");
              Serial.println(local_scan[0].scan_frequence); */
							node_lidar.lidar_time.scan_time_record = node_lidar.lidar_time.scan_time_current;
						}
						node_lidar.lidar_time.scan_start_time = node_lidar.lidar_time.tim_scan_start;
						node_lidar.lidar_time.scan_end_time = node_lidar.lidar_time.tim_scan_end;
						if(node_lidar.lidar_time.tim_scan_start != node_lidar.lidar_time.tim_scan_end)
						{
							node_lidar.lidar_time.tim_scan_start = node_lidar.lidar_time.tim_scan_end;
						}

						memcpy(node_lidar.scan_node_buf, local_scan, scan_count * sizeof(node_info));
						node_lidar.scan_node_count = scan_count;
						node_lidar.lidar_time.scan_time_current = millis();
					}
					scan_count = 0;
				}
				local_scan[scan_count++] = local_buf[pos];
				if (scan_count == _countof(local_scan))
				{
					scan_count -= 1;
				}
			}
		}
		else{
			flushSerial();
			delay(100);
		}
		
	return RESULT_OK;
}

/*Processing the latest radar data*/
void send_lidar_data(LaserScan &outscan)
{
	size_t count = node_lidar.scan_node_count;

	if(count < MAX_SCAN_NODES && count > 0)
	{	
		//node_lidar.lidar_time.tim_scan_end = getTime();
		uint64_t scan_time = (node_lidar.lidar_time.scan_end_time - node_lidar.lidar_time.scan_start_time);

		//node_lidar.lidar_time.tim_scan_start = node_lidar.lidar_time.tim_scan_end -  scan_time ;

		node_lidar.lidar_block.lidar_zero_count = 0;


		outscan.config.angle_increment = (2.0*M_PI/count);
		outscan.config.min_angle = 0;
		outscan.config.max_angle = 2*M_PI;
		outscan.config.min_range = 0.10;
		outscan.config.max_range = 10.0; //10m
		outscan.config.scan_time =  static_cast<float>(scan_time * 1.0 / 1e9);
    outscan.config.time_increment = outscan.config.scan_time / (double)(count - 1);
		outscan.stamp = node_lidar.lidar_time.scan_start_time;
		//std::cout << "scantime:" << outscan.config.scan_time << "stamp:" << outscan.stamp << std::endl;
		//scan_msg->header.frame_id = node_lidar.lidar_general_info.frame_id;
		//scan_msg->header.stamp = ros::Time::now();

		if(node_lidar.lidar_status.isConnected)
		{
			//outscan.points.clear();
			float range = 0;
			float angle = 0.0;
			uint16_t intensity = 0;
			for (int i = 0; i < count; i++)
			{
				LaserPoint point;
				LaserPoint point_check;
				angle = static_cast<float>((node_lidar.scan_node_buf[i].angle_q6_checkbit >>
											LIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) /64.0f);
				range = (node_lidar.scan_node_buf[i].distance_q2/1000.0f);
				intensity = node_lidar.scan_node_buf[i].sync_quality;

				if(node_lidar.scan_node_buf[i].exp_m == 1)
				{
					intensity = 255;
				}else{
					if(intensity >= 255)
					{
						intensity = 254;
					}
				}

				point_check.angle = angle;
				point_check.range = range;
				point_check.intensity = intensity;
				
				if(0 <= angle && angle <= 360){
					point.range = range;
					point.angle = angle;
					point.intensity = intensity;
				}else{
					point.range = 0.0;
					point.intensity = 0;
					point.angle = 0.0;
				}

				if(range < 0.1)
				{
					node_lidar.lidar_block.lidar_zero_count++;
				}

				if(range <= 0.15 && intensity <=65)
				{
					point.range = 0.0;
					point.intensity = 0;
				}
				outscan.points.push_back(point);
			}
			if(node_lidar.data_calibration)
			{
				//lidar_calibration(outscan);
			}

			node_lidar.optimize_lidar.lidar_blocked_judge(count);


			if (node_lidar.lidar_status.FilterEnable)
			{
				node_lidar.optimize_lidar.PointCloudFilter(&outscan);
			}


		}
	}
}

bool lidar_set_port()
{
	if (node_lidar.lidar_status.isConnected)
	{
		return true;
	}

	node_lidar.lidar_status.isConnected=true;
	delay(100);
  return true;
}

bool initialize()
{
	if(node_lidar.lidar_status.optimize_enable)
	{
		node_lidar.optimize_lidar.lidar_blocked_init();
	}

	switch (node_lidar.lidar_general_info.version)
	{
	case M1C1_Mini_v1:
		Serial.println("version M1C1_Mini_v1");
		node_lidar.lidar_data_processing.PackageSampleBytes = 2;
		break;

	case M1C1_Mini_v2:
		Serial.println("version M1C1_Mini_v2");
		node_lidar.lidar_data_processing.PackageSampleBytes = 3;
		node_lidar.lidar_general_info.m_intensities = true;
		break;

	case M1CT_Coin_Plus:
		Serial.println("version M1CT_Coin_Plus");
		node_lidar.lidar_data_processing.PackageSampleBytes = 3;
		break;
	
	case M1CT_TOF:
		Serial.println("version M1CT_TOF");
		node_lidar.lidar_data_processing.PackageSampleBytes = 3;
		node_lidar.lidar_general_info.m_intensities = true;
		break;
	
	default:
		break;
	}

	if(!lidar_set_port()){
		Serial.println("lidar_set_port wrong");
		return false;
	}

	node_lidar.scan_node_buf = new node_info[1000];
	node_lidar.globalRecvBuffer = new uint8_t[sizeof(node_packages)];
	return true;
}


void setup_lidar()
{	
  Serial2.setTX(4);
  Serial2.setRX(5);
  
  // RP2350 default UART buffer is too small for lidar frames
  // must increase or available() never reaches frame size
  Serial2.setFIFOSize(256);

  Serial2.begin(230400);

	bool ret_init = initialize();

	if(!ret_init){
		Serial.println("node_lidar init error");
		while (1);
    }
	node_lidar.lidar_status.lidar_ready = true;
	memset(local_scan, 0, sizeof(local_scan));
}

const double rk = 0.196;  // polomer kvetinaca

uint16_t policka[ROZMER * ROZMER];

int pocitadlo = 0;

void analyze_data(LaserScan *scan)
{
  memset(policka, 0, 2 * ROZMER * ROZMER);
  int max_x,max_y,max_p=0;

  for (int i = 0; i < scan->points.size(); i++)
  {
		if ((scan->points[i].range < 0.35) || (scan->points[i].range > 3))
		  continue;
    double x = scan->points[i].range * sin(M_PI * (scan->points[i].angle + OTOC_LIDAR) / 180);
    double y = scan->points[i].range * cos(M_PI * (scan->points[i].angle + OTOC_LIDAR) / 180);

    int stare_ksx = -1000;
		int stare_ksy = -1000;

    int startx = POLROZMER + (DOMETRA * x + 0.5);
		int starty = POLROZMER + (DOMETRA * (y + rk) + 0.5);

		int bod_y = POLROZMER + (DOMETRA * y + 0.5);

    for (int beta = 0; beta < 360; beta += 3) // 7)
		{
      double ksx = x + rk * sin(M_PI * beta / 180);
			double ksy = y + rk * cos(M_PI * beta / 180);
			int ksx_int = POLROZMER + (DOMETRA * ksx + 0.5);
			int ksy_int = POLROZMER + (DOMETRA * ksy + 0.5);

			if ((stare_ksx == ksx_int) && (stare_ksy == ksy_int)) 
			  continue;

			if ((ksx_int == startx) && (ksy_int == starty) && (beta > 30))  // tento sme uz zapocitali na zaciatku
			  continue;

      stare_ksx = ksx_int;
			stare_ksy = ksy_int;

      if ((ksx_int >= 0) &&
			    (ksy_int >= 0) &&
			    (ksx_int < ROZMER) &&
					(ksy_int < ROZMER))
			{		 
	       for (int j = -1; j <= 1; j++)
				   for (int k = -1; k <= 1; k++)
					 {
						if ((ksx_int + k >= 0) &&
								(ksy_int + j >= 0) &&
								(ksx_int + k < ROZMER) &&
								(ksy_int + j < ROZMER))
						{	
							int delta = 3 - abs(j) - abs(k);

							int novy_pocet = policka[(ksy_int + j) * ROZMER + ksx_int + k];
							novy_pocet += delta;
							policka[(ksy_int + j) * ROZMER + ksx_int + k] = novy_pocet;

							if (novy_pocet > max_p)
							{
								max_p = novy_pocet;
								max_x = ksx_int + k;
								max_y = ksy_int + j;
							}
						}
					 }
			}
		}
  } 

	pocitadlo++;
	if (pocitadlo == 1)
	{
		Serial.print("["); Serial.print(max_x - POLROZMER); Serial.print(","); Serial.print(max_y - POLROZMER); Serial.print(",("); Serial.print(max_p); Serial.println(")]");
		pocitadlo = 0;
	}

}


void loop_lidar()
{
	while(1)
	{
   read_once();
		LaserScan scan;
		send_lidar_data(scan);
    
    analyze_data(&scan);
	}
}
