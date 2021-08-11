#include "ACC_Stream.h"

ACC_Stream::ACC_Stream():Tool(){}


bool ACC_Stream::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  std::string ip="";
  std::string port="0";
	
  if(!m_variables.Get("IP",ip)) ip="127.0.0.1";
  if(!m_variables.Get("Port",port)) port="8888";

  std::string connection="tcp://"+ip+":"+port;
  
  sock=new zmq::socket_t(*(m_data->context), ZMQ_DEALER);

  sock->connect(connection.c_str());

  return true;
}


bool ACC_Stream::Execute(){
  PsecData *pdata;
  pdata = new PsecData;
  std::map<int, PsecData> StreamMap;

  for(std::map<int, vector<unsigned short>>::iterator it=m_data->psec.ReceiveData.begin(); it!=m_data->psec.ReceiveData.end(); ++it)
  {
  	StreamMap[it->first] = *pdata;
  	StreamMap[it->first].BoardIndex = it->first;
  	StreamMap[it->first].RawWaveform = it->second;
  	StreamMap[it->first].AccInfoFrame = m_data->psec.AccInfoFrame;
  	StreamMap[it->first].errorcodes = m_data->psec.errorcodes;
  	StreamMap[it->first].FailedReadCounter = m_data->psec.FailedReadCounter;
  	StreamMap[it->first].Send(sock);
  	StreamMap[it->first].Print();
  }
  StreamMap.clear();
  m_data->psec.errorcodes.clear();
  m_data->psec.ReceiveData.clear();
  m_data->psec.AccInfoFrame.clear();
  m_data->psec.RawWaveform.clear();
	
  return true;
}


bool ACC_Stream::Finalise(){
	
  delete sock;
  sock=0; 

  return true;
}


