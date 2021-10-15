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
  vector<int> bi;
  vecetor<unsigned short> data;

  for(std::map<int, vector<unsigned short>>::iterator it=m_data->TCS.ReadFileData.begin(); it!=m_data->TCS.ReadFileData.end(); ++it)
  {
	bi.push_back(it->first);
	data.insert(data.end(),it->second.begin(),it->second.end());
  }	  

  pdata.BoardIndex = bi;
  pdata.RawWaveform = data;
  pdata.AccInfoFrame = m_data->psec.AccInfoFrame;
  pdata.errorcodes = m_data->psec.errorcodes;
  pdata.FailedReadCounter = m_data->psec.FailedReadCounter;
  pdata.Send(sock);
  pdata.Print();

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


