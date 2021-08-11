#include "SC_Receive.h"

SC_Receive::SC_Receive():Tool(){}


bool SC_Receive::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;
  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  std::string ip="";
  std::string port="0";
	
  if(!m_variables.Get("IP",ip)) ip="0.0.0.0";
  if(!m_variables.Get("Port",port)) port="4444";

  std::string connection="tcp://"+ip+":"+port;
  
  sock=new zmq::socket_t(*(m_data->context), ZMQ_DEALER);

  sock->bind(connection.c_str());
   
  items[0].socket = *sock;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLIN;
  items[0].revents =0;
  
  return true;
}


bool SC_Receive::Execute(){

  int timer;
  if(m_data->SCMonitor.recieveFlag==0)
  {
    timer = -1;
  }else
  {
    timer = 10;
  }

  zmq::poll(&items[0], 1, timer);

  if((items [0].revents & ZMQ_POLLIN)) 
  {
    m_data->SCMonitor.Receive_Config(sock);
  } 
  
  return true;
}


bool SC_Receive::Finalise(){
  delete sock;
  sock=0;

  return true;
}
