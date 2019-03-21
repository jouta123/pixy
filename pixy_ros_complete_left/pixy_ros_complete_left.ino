#include <ros.h>
#include <geometry_msgs/Point32.h>
#include <std_msgs/Int16.h>
#include <Pixy2.h>

#define target_position_x 80
#define target_position_y 123

ros::NodeHandle nh;
std_msgs::Int16 nor_msg;
geometry_msgs::Point32 dist_msg;
ros::Publisher chatter("distance_left", &dist_msg);
Pixy2 pixy;

int color = 1;
unsigned long long pre_t;


void color_select(const std_msgs::Int16 &msg) {
  color = msg.data;
}

ros::Subscriber<std_msgs::Int16> selecter("pixy_color", color_select);

void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  //nh.advertise(speaker);
  nh.subscribe(selecter);
  Serial.begin(115200);
  pixy.init();
}

//
void loop()
{

  nh.spinOnce();

  pixy.ccc.getBlocks(false,color,1);

  if (pixy.ccc.numBlocks){
    dist_msg.x = (int32_t)pixy.ccc.blocks[0].m_y - target_position_y;
    dist_msg.y = (int32_t)pixy.ccc.blocks[0].m_x - target_position_x;
    dist_msg.z = color;
     
     if(millis() - pre_t > 100) {
      chatter.publish(&dist_msg);
      pre_t = millis();
    }
  } 
}
