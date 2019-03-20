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
//ros::Publisher speaker("normal", &nor_msg);
Pixy2 pixy;

int color = 2;

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
  int i = 0;
  unsigned long long pre_t;

  nh.spinOnce();

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks)
  {
    for (i = 0; i < pixy.ccc.getBlocks(); ++i) {
      if (pixy.ccc.blocks[i].m_signature == color) {
        dist_msg.x = (double)pixy.ccc.blocks[i].m_y - target_position_y;
        dist_msg.y = (double)pixy.ccc.blocks[i].m_x - target_position_x;
        dist_msg.z = i + color * 10 + pixy.ccc.getBlocks() * 100;


        if (millis() - pre_t > 100) {
          chatter.publish(&dist_msg);
          pre_t = millis();
        }

        /*if (target_position_x - pixy.ccc.blocks[i].m_x < 10 && target_position_y - pixy.ccc.blocks[i].m_y < 10) {
          nor_msg.data = ok;
          speaker.publish(&nor_msg);
          }*/
      }
    }
  }
}