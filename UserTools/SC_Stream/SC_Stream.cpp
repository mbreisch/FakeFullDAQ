#include "SC_Stream.h"

SC_Stream::SC_Stream():Tool(){}


bool SC_Stream::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  
  std::string ip="";
  std::string port="0";
  if(!m_variables.Get("IP",ip)) ip="127.0.0.1";
  if(!m_variables.Get("Port",port)) port="3333";
  m_port = stoi(port);

  sock=new zmq::socket_t(*(m_data->context), ZMQ_PUB);
  std::string connection="tcp://"+ip+":"+port;
  sock->connect(connection.c_str());
  
  long time_sec=0;

  if(!m_variables.Get("Period",time_sec)) time_sec=1;

  period=boost::posix_time::seconds(time_sec);
  last=boost::posix_time::second_clock::local_time();

  m_util=new Utilities(m_data->context);
  if (!m_util->AddService("SlowControlMonitor",m_port,false)) return false;
  
  return true;
}


bool SC_Stream::Execute(){

  boost::posix_time::ptime current(boost::posix_time::second_clock::local_time());
  boost::posix_time::time_duration lapse(period - (current - last)); 

  if(!lapse.is_negative())
  {
    m_data->SCMonitor.Send_Mon(sock);
    last=boost::posix_time::second_clock::local_time();
  }

  m_data->SCMonitor.Print();
  m_data->SCMonitor.errorcodes.clear();
  return true;
}


bool SC_Stream::Finalise(){

  delete sock;
  sock=0; 

  bool ret= m_util->RemoveService("SlowControlMonitor");
  
  delete m_util;
  m_util=0;
  
  return ret;
}


